#define NBYTES_WIN32_WNDCLASS 		L"nbytes_win32_wndclass"
#define NBYTES_WIN32_MAX_TITLE_LEN 	512
typedef struct Win32Window {
	wchar_t title[NBYTES_WIN32_MAX_TITLE_LEN];
	HWND hwnd;
	HDC hdc;
	union {
		struct {
			HGLRC ctx;
		} gl;
	};
} Win32Window;

bool win32_dragging;
Win32Window win32_window;
HANDLE win32_main_fiber;
HANDLE win32_msg_fiber;

LRESULT CALLBACK
nbytes_win32_wndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg) {
		case WM_CREATE: {
			RAWINPUTDEVICE rid[2] = {0};
			// Keyboard
			rid[0].usUsagePage = 0x01;
			rid[0].usUsage     = 0x06;
			rid[0].dwFlags     = 0;			/* 0x00000030 RIDEV_NOLEGACY | to skip WM_KEYDOWN/UP messages */;
			rid[0].hwndTarget  = hwnd;
			// Mouse
			rid[1].usUsagePage = 0x01;
			rid[1].usUsage     = 0x02;
			rid[1].dwFlags     = 0x0;
			rid[1].hwndTarget  = hwnd;
			assert(RegisterRawInputDevices(rid, ARRAY_LEN(rid), sizeof(rid[0])));

		} break;
		case WM_ACTIVATE: {
			int state = wParam & 0xffff;
			app.window.focus = (state >= 1);
		} break;
		case WM_LBUTTONUP: case WM_LBUTTONDOWN: {
			input_update_keystate(&app.keys[VK_LBUTTON], (uMsg == WM_LBUTTONDOWN));
		} break;
		case WM_MBUTTONUP: case WM_MBUTTONDOWN: {
			input_update_keystate(&app.keys[VK_MBUTTON], (uMsg == WM_MBUTTONDOWN));
		} break;
		case WM_RBUTTONUP: case WM_RBUTTONDOWN: {
			input_update_keystate(&app.keys[VK_RBUTTON], (uMsg == WM_RBUTTONDOWN));
		} break;
		case WM_XBUTTONUP: case WM_XBUTTONDOWN:	{
			input_update_keystate(&app.keys[(wParam == MK_XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2)], (uMsg == WM_XBUTTONDOWN));
		} break;
		case WM_KEYUP: case WM_KEYDOWN: {
			input_update_keystate(&app.keys[wParam], (uMsg == WM_KEYDOWN));
		} break;
		case WM_MOUSEMOVE: {
			app.mouse.rel_x = lParam & 0xffff;
			app.mouse.rel_y = lParam >> 16;
		} break;
		case WM_MOUSEWHEEL: {
			short wheel_delta = (short) (wParam >> 16);
			app.mouse.wheel += wheel_delta / 120;
		} break;
		case WM_TIMER: {
			SwitchToFiber(win32_main_fiber);
		} break;
		case WM_ENTERMENULOOP:
		case WM_ENTERSIZEMOVE: {
			win32_dragging = true;
			SetTimer(hwnd, 1, 1, 0);
		} break;
		case WM_EXITMENULOOP:
		case WM_EXITSIZEMOVE: {
			win32_dragging = false;
			KillTimer(hwnd, 1);
		} break;
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP: { } break;
		case WM_DESTROY: {
			PostQuitMessage(0);
			app.quit = true;
			return 0;
		}
		case WM_SYSCOMMAND: {
			if (wParam == SC_KEYMENU) {
				//app.keys[VK_MENU].down = true;
				return 0;
			}
		}
		default: {
			result = DefWindowProcW(hwnd, uMsg, wParam, lParam);
		}
	}
	return result;
}

VOID CALLBACK
nbytes_win32_handle_messages(PVOID lpParameter)
{
	for(;;) {
		MSG message;
		while(PeekMessageW(&message, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}
		SwitchToFiber(win32_main_fiber);
	}
}

int
nbytes_win32_utf8_to_utf16(wchar_t *widestring, int wcslen, const char *multibyte, int mbs_len)
{
	int _wcs_len = MultiByteToWideChar(CP_UTF8, 0, multibyte, mbs_len, 0, 0);
	if (_wcs_len <= wcslen) { MultiByteToWideChar(CP_UTF8, 0, multibyte, mbs_len, widestring, _wcs_len); }
	return _wcs_len;
}

int
nbytes_win32_utf16_to_utf8(char *multibyte, int mbs_len, wchar_t *widestring, int wcs_len)
{
	int _mbs_len = WideCharToMultiByte(CP_UTF8, 0, widestring, wcs_len, 0, 0, 0, 0);
	if (_mbs_len <= mbs_len) { WideCharToMultiByte(CP_UTF8, 0, widestring, wcs_len, multibyte, _mbs_len, 0, 0); }
	return _mbs_len;
}


bool
nbytes_win32_prepare_fibers()
{
	static bool is_initialized;
	if(!is_initialized) {
		win32_main_fiber = ConvertThreadToFiber(0);
		win32_msg_fiber = CreateFiber(0, nbytes_win32_handle_messages, 0);
		assert(win32_main_fiber && win32_msg_fiber);

		is_initialized = true;
	}
	return is_initialized;
}

bool
nbytes_win32_set_vsync(bool enable)
{
	if (!opengl_loader_supports_ext("WGL_EXT_swap_control")) { return !enable; }
	return wglSwapIntervalEXT(enable);
}

bool
nbytes_win32_get_vsync()
{
	if (!opengl_loader_supports_ext("WGL_EXT_swap_control")) { return false; }
	return wglGetSwapIntervalEXT();
}

void
nbytes_win32_set_size(Win32Window *wnd, int2 size)
{
	if(win32_dragging) { return; }
	RECT rc = {0, 0, (long) size.x, (long) size.y};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, 0);
	SetWindowPos(wnd->hwnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);
}

int2
nbytes_win32_get_size(Win32Window *wnd)
{
	RECT rc;
	GetClientRect(wnd->hwnd, &rc);
	return (int2) {rc.right, rc.bottom};
}

void
nbytes_win32_set_pos(Win32Window *wnd, int2 pos)
{
	if(win32_dragging) { return; }
	SetWindowPos(wnd->hwnd, 0, pos.x, pos.y, 0, 0, SWP_NOSIZE);
}

int2
nbytes_win32_get_pos(Win32Window *wnd)
{
	RECT rect;
	GetWindowRect(wnd->hwnd, &rect);
	return (int2) {rect.left, rect.top};
}

bool
nbytes_win32_set_title(Win32Window *wnd, const char *title, int len)
{
	size_t res = nbytes_win32_utf8_to_utf16(wnd->title, NBYTES_WIN32_MAX_TITLE_LEN, title, len);
	assert(res == len);
	return SetWindowTextW(wnd->hwnd, wnd->title);
}

void
nbytes_win32_swapbuffers(Win32Window *wnd)
{
	SwapBuffers(wnd->hdc);
}

void
nbytes_update_events()
{
	app.keymod = 0;
	for(int vk = 0; vk < NBYTES_NUM_MAX_KEYS; vk++) {
		input_reset_keystate(&app.keys[vk]);
	}

	/*
	Windows message queue workaround with Fibers to prevent the recursive calls into
	the wndproc when dragging a window. Due to fiber switches we can jump out of the
	message fiber wndproc back to the main routine whenever we want.
	*/
	SwitchToFiber(win32_msg_fiber);

	if(app.keys[VK_SHIFT].down) {
		app.keymod |= 1 << 0;
	}
	if(app.keys[VK_CONTROL].down) {
		app.keymod |= 1 << 1;
	}
	if(app.keys[VK_MENU].down) {
		app.keymod |= 1 << 2;
	}

	if(win32_dragging) {
		// win32 hack to reset keystate when the window is currently in dragmode.
		app.keymod = 0;
		for(int vk = 0; vk < NBYTES_NUM_MAX_KEYS; vk++) {
			input_reset_keystate(&app.keys[vk]);
			app.keys[vk].down = false;
		}
	}

	POINT cursor;
	GetCursorPos(&cursor);
	app.mouse.x = cursor.x;
	app.mouse.y = cursor.y;
	app.mouse.dx = app.mouse.x - app.mouse.prev_state.x;
	app.mouse.dy = app.mouse.y - app.mouse.prev_state.y;
	app.mouse.wheel_delta = app.mouse.wheel - app.mouse.prev_state.wheel;
	app.mouse.prev_state.x = app.mouse.x;
	app.mouse.prev_state.y = app.mouse.y;
	app.mouse.prev_state.wheel = app.mouse.wheel;
}

void
nbytes_render_window()
{
	Win32Window *wnd = &win32_window;
	nbytes_win32_swapbuffers(wnd);
}

bool
nbytes_init_window()
{
	nbytes_win32_prepare_fibers();

	if(!app.window.title) { app.window.title = NBYTES_DEFAULT_TITLE; }
	app.window.pos.x = (app.window.pos.x == NBYTES_DEFAULT_WINDOW_POS ? CW_USEDEFAULT : app.window.pos.x);
	app.window.pos.y = (app.window.pos.y == NBYTES_DEFAULT_WINDOW_POS ? CW_USEDEFAULT : app.window.pos.y);
	app.window.size.x = (app.window.size.x == 0 ? NBYTES_DEFAULT_WINDOW_WIDTH : app.window.size.x);
	app.window.size.y = (app.window.size.y == 0 ? NBYTES_DEFAULT_WINDOW_HEIGHT : app.window.size.y);

	app.window.opengl.major = (app.window.opengl.major == 0 ? NBYTES_DEFAULT_OPENGL_MAJOR : app.window.opengl.major);
	app.window.opengl.minor = (app.window.opengl.minor == 0 ? NBYTES_DEFAULT_OPENGL_MINOR : app.window.opengl.minor);
	app.window.opengl.vsync = (app.window.opengl.vsync == 0 ? NBYTES_DEFAULT_OPENGL_VSYNC : app.window.opengl.vsync);
	app.window.opengl.core_profile = (app.window.opengl.core_profile == 0 ? NBYTES_DEFAULT_OPENGL_CORE_PROFILE : app.window.opengl.core_profile);
	app.window.opengl.debug = (app.window.opengl.debug == 0 ? NBYTES_DEFAULT_OPENGL_DEBUG : app.window.opengl.minor);

	WNDCLASSEXW wnd_class   = { sizeof(wnd_class) };
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wnd_class.lpfnWndProc   = nbytes_win32_wndproc;
	wnd_class.lpszClassName = NBYTES_WIN32_WNDCLASS;
	wnd_class.hCursor       = LoadCursor(0, IDC_CROSS);
	//wnd_class.cbClsExtra	= sizeof(int);

	DWORD last_error = 0;
	ATOM registered 		= RegisterClassExW(&wnd_class);
	last_error 				= GetLastError();
	if(last_error != 0 && last_error != 1410) {
		fatal("RegisterClassExW() failed. GetLastError = %i", last_error);
	}

	RECT rc = {0, 0, (long) app.window.size.x, (long) app.window.size.y};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, 0);


	DWORD style = WS_OVERLAPPEDWINDOW;
	HWND hwnd = CreateWindowExW(0, wnd_class.lpszClassName, L"", style, app.window.pos.x, app.window.pos.y, rc.right - rc.left, rc.bottom - rc.top, 0, 0, wnd_class.hInstance, 0);
	if(!hwnd) {
		fatal("CreateWindowExW() failed. GetLastError = %i", GetLastError());
	}

	HGLRC ctx 		= 0;
	HDC hdc 		= GetDC(hwnd);
	if (!hdc) {
		fatal("GetDC() failed. GetLastError = %i", GetLastError());
	}

	PIXELFORMATDESCRIPTOR pfd = {0};
	pfd.nSize                 = sizeof(pfd);
	pfd.nVersion              = 1;
	pfd.iPixelType            = PFD_TYPE_RGBA;
	pfd.dwFlags               = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.cColorBits            = 32;
	pfd.cDepthBits            = 24;
	pfd.cStencilBits          = 8;
	pfd.iLayerType            = PFD_MAIN_PLANE;

	int pf = ChoosePixelFormat(hdc, &pfd);
	if(!pf) { fatal("ChoosePixelFormat() failed. GetLastError = %i", GetLastError()); }

	BOOL set_pixel_format = SetPixelFormat(hdc, pf, &pfd);
	if(!set_pixel_format) { fatal("SetPixelFormat() failed. GetLastError = %i", GetLastError()); }

	ctx = wglCreateContext(hdc);
	if(!ctx) { fatal("wglCreateContext() failed. GetLastError = %i", GetLastError()); }

	BOOL make_current = wglMakeCurrent(hdc, ctx);
	if(!make_current) { fatal("wglMakeCurrent() failed. GetLastError = %i", GetLastError()); }

	opengl_loader_init();	// Initialize opengl driver... grabs function pointer and extension strings

	if (opengl_loader_supports_ext("WGL_ARB_create_context")) {
		HGLRC modern_context = 0;
		int attribs[] = {
			0x2091 /* WGL_CONTEXT_MAJOR_VERSION_ARB */, app.window.opengl.major,
			0x2092 /* WGL_CONTEXT_MINOR_VERSION_ARB */, app.window.opengl.minor,
			0x9126 /* WGL_CONTEXT_PROFILE_MASK_ARB */, (app.window.opengl.core_profile ? 0x00000001 : 0x00000002),
			/* WGL_CONTEXT_CORE_PROFILE_BIT_ARB | WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB */
			0x2094 /* WGL_CONTEXT_FLAGS_ARB */, (app.window.opengl.debug ? 1 : 0),
			0
		};

		modern_context = wglCreateContextAttribsARB(hdc, 0, attribs);
		if (modern_context) {
			wglMakeCurrent(0, 0);
			wglMakeCurrent(hdc, modern_context);
			wglDeleteContext(ctx);
			ctx = modern_context;
		}
	}

	ShowWindow(hwnd, SW_SHOW);

	win32_window.hwnd = hwnd;
	win32_window.hdc = hdc;
	win32_window.gl.ctx = ctx;

	glGetIntegerv(0x821B /* GL_MAJOR_VERSION */, &app.window.opengl.major);
	glGetIntegerv(0x821C /* GL_MINOR_VERSION */, &app.window.opengl.minor);

	app.window.prev_state.size = app.window.size;
	app.window.prev_state.pos = app.window.pos;


	return true;
}

void
nbytes_update_window()
{
	Win32Window *wnd = &win32_window;

	if(app.window.hidden != app.window.prev_state.hidden) {
		ShowWindow(wnd->hwnd, (app.window.hidden ? SW_HIDE : SW_SHOW));
	}
	app.window.hidden = !IsWindowVisible(wnd->hwnd);
	app.window.prev_state.hidden = app.window.hidden;

	if(app.window.focus != app.window.prev_state.focus) {
		if(app.window.focus) { SetForegroundWindow(wnd->hwnd); }
	}
	app.window.focus = (GetForegroundWindow() == wnd->hwnd);
	app.window.focused = (app.window.focus != app.window.prev_state.focus);
	app.window.prev_state.focus = app.window.focus;

	if(app.window.size.x != app.window.prev_state.size.x || app.window.size.y != app.window.prev_state.size.y) {
		nbytes_win32_set_size(wnd, app.window.size);
	}
	app.window.size = nbytes_win32_get_size(wnd);
	app.window.resized = (app.window.prev_state.size.x != app.window.size.x || app.window.prev_state.size.y != app.window.size.y);
	app.window.prev_state.size = app.window.size;

	if(app.window.pos.x != app.window.prev_state.pos.x || app.window.pos.y != app.window.prev_state.pos.y) {
		nbytes_win32_set_pos(wnd, app.window.pos);
	}
	app.window.pos = nbytes_win32_get_pos(wnd);
	app.window.moved = (app.window.prev_state.pos.x != app.window.pos.x || app.window.prev_state.pos.y != app.window.pos.y);
	app.window.prev_state.pos = app.window.pos;

	if(app.window.opengl.vsync != app.window.prev_state.opengl.vsync) {
		nbytes_win32_set_vsync(app.window.opengl.vsync);
		app.window.opengl.vsync = app.window.prev_state.opengl.vsync = nbytes_win32_get_vsync();
	}

	if(app.window.title != app.window.prev_state.title) {
		nbytes_win32_set_title(&win32_window, app.window.title, strlen(app.window.title));
		app.window.prev_state.title = app.window.title;
	}
}

bool
nbytes_init_time()
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	app.time.ticks_per_sec = frequency.QuadPart;
	app.time.start_ticks = counter.QuadPart;
	return true;
}

bool
nbytes_update_time()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);

	uint64_t ticks = now.QuadPart - app.time.start_ticks;

	app.time.nsecs = (ticks * 1000 * 1000 * 1000) / app.time.ticks_per_sec;
	app.time.usecs = (ticks * 1000 * 1000) / app.time.ticks_per_sec;
	app.time.msecs = (ticks * 1000) / app.time.ticks_per_sec;
	app.time.secs = (double) ticks / (double) app.time.ticks_per_sec;

	app.time.delta_ticks = (int)(ticks - app.time.ticks);
	app.time.delta_nsecs = (int)((app.time.delta_ticks * 1000 * 1000 * 1000) / app.time.ticks_per_sec);
	app.time.delta_usecs = (int)((app.time.delta_ticks * 1000 * 1000) / app.time.ticks_per_sec);
	app.time.delta_msecs = (int)((app.time.delta_ticks * 1000) / app.time.ticks_per_sec);
	app.time.delta_secs = (float)app.time.delta_ticks / (float)app.time.ticks_per_sec;
	app.time.ticks = ticks;
	return true;
}
