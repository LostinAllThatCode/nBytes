#define NBYTES_WIN32_USE_RAWINPUT 	0
#define NBYTES_WIN32_WNDCLASS 		L"nbytes__win32_wndclass"
#define NBYTES_WIN32_MAX_TITLE_LEN 	512

uint8_t win32_vk_to_nbytes[NBYTES_NUM_MAX_KEYS] = {
	/* 0x0 */ 0,
	/* VK_LBUTTON */ KEY_MOUSE_LEFT,
	/* VK_RBUTTON */ KEY_MOUSE_RIGHT,
	/* VK_CANCEL */ 0,
	/* VK_MBUTTON */ KEY_MOUSE_MIDDLE,
	/* VK_XBUTTON1 */ KEY_MOUSE_EXT1,
	/* VK_XBUTTON2 */ KEY_MOUSE_EXT2,
	/* 0x07 */ 0,
	/* VK_BACKSPACE */ KEY_BACKSPACE,
	/* VK_TAB */ KEY_TAB,
	/* 0x0A-0B */ 0, 0,
	/* VK_CLEAR */ 0,
	/* VK_RETURN */ KEY_ENTER,
	/* 0x0E-0F */ 0, 0,
	/* VK_SHIFT */ KEY_SHIFT,
	/* VK_CONTROL */ KEY_CTRL,
	/* VK_MENU */ KEY_ALT,
	/* VK_PAUSE */ KEY_PAUSE,
	/* VK_CAPITAL */ KEY_CAPSLOCK,
	/* VK_KANA|VK_HANGUEL|VK_HANGUL */ 0,
	/* 0x16 */ 0,
	/* VK_JUNJA */ 0,
	/* VK_FINAL */ 0,
	/* VK_HANJA|VK_KANJI */ 0,
	/* 0x1A */ 0,
	/* VK_ESCAPE */ KEY_ESC,
	/* VK_CONVERT */ 0,
	/* VK_NONCONVERT */ 0,
	/* VK_ACCEPT */ 0,
	/* VK_MODECHANGE */ 0,
	/* VK_SPACE */ KEY_SPACE,
	/* VK_PRIOR */ 0,
	/* VK_NEXT */ 0,
	/* VK_END */ 0,
	/* VK_HOME */ 0,
	/* VK_LEFT */ KEY_LEFT,
	/* VK_UP */ KEY_UP,
	/* VK_RIGHT */ KEY_RIGHT,
	/* VK_DOWN */ KEY_DOWN,
	/* VK_SELECT */ 0,
	/* VK_PRINT */ KEY_PRINT,
	/* VK_EXECUTE */ 0,
	/* VK_SNAPSHOT */ 0,
	/* VK_INSERT */ KEY_INS,
	/* VK_DELETE */ KEY_DEL,
	/* VK_HELP */ 0,
	/* 0x30 - 0x39 | 0-9 keys*/
	KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9,
	/* 0x3A - 0x40 */
	0, 0, 0, 0, 0, 0, 0,
	/* 0x41 - 0x5A | A-Z keys*/
	KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K,
	KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
	/* VK_LWIN */ KEY_LMETA,
	/* VK_RWIN */ KEY_RMETA,
	/* VK_APPS */ 0,
	/* 0x5E */ 0,
	/* VK_SLEEP */ 0,
	/* VK_NUMPAD0 */ KEY_NUM_0,
	/* VK_NUMPAD1 */ KEY_NUM_1,
	/* VK_NUMPAD2 */ KEY_NUM_2,
	/* VK_NUMPAD3 */ KEY_NUM_3,
	/* VK_NUMPAD4 */ KEY_NUM_4,
	/* VK_NUMPAD5 */ KEY_NUM_5,
	/* VK_NUMPAD6 */ KEY_NUM_6,
	/* VK_NUMPAD7 */ KEY_NUM_7,
	/* VK_NUMPAD8 */ KEY_NUM_8,
	/* VK_NUMPAD9 */ KEY_NUM_9,
	/* VK_MULTIPLY */ KEY_NUM_MUL,
	/* VK_ADD */ KEY_NUM_ADD,
	/* VK_SEPARATOR */ 0,
	/* VK_SUBTRACT */ KEY_NUM_SUB,
	/* VK_DECIMAL */ KEY_NUM_DECIMAL,
	/* VK_DIVIDE */ KEY_NUM_DIV,
	/* VK_F1 */ KEY_F1,
	/* VK_F2 */ KEY_F2,
	/* VK_F3 */ KEY_F3,
	/* VK_F4 */ KEY_F4,
	/* VK_F5 */ KEY_F5,
	/* VK_F6 */ KEY_F6,
	/* VK_F7 */ KEY_F7,
	/* VK_F8 */ KEY_F8,
	/* VK_F9 */ KEY_F9,
	/* VK_F10 */ KEY_F10,
	/* VK_F11 */ KEY_F11,
	/* VK_F12 */ KEY_F12,
	/* VK_F13 - VK-F24 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* 0x88 - 0x8F */ 0, 0, 0, 0, 0, 0, 0, 0,
	/* VK_NUMLOCK */ KEY_NUM_LOCK,
	/* VK_SCROLL */ 0,
	/* 0x92 - 0x96 */ 0, 0, 0, 0, 0,
	/* 0x97 - 0x9F */ 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* VK_LSHIFT */ 0,
	/* VK_RSHIFT */ 0,
	/* VK_LCONTROL */ 0,
	/* VK_RCONTROL */ 0,
	/* VK_LMENU */ 0,
	/* VK_RMENU */ 0,
	/* VK_BROWSER_BACK */ 0,
	/* VK_BROWSER_FORWARD */ 0,
	/* VK_BROWSER_REFRESH */ 0,
	/* VK_BROWSER_STOP */ 0,
	/* VK_BROWSER_SEARCH */ 0,
	/* VK_BROWSER_FAVORITES */ 0,
	/* VK_BROWSER_HOME */ 0,
	/* VK_VOLUME_MUTE */ 0,
	/* VK_VOLUME_DOWN */ 0,
	/* VK_VOLUME_UP */ 0,
	/* VK_MEDIA_NEXT_TRACK */ 0,
	/* VK_MEDIA_PREV_TRACK */ 0,
	/* VK_MEDIA_STOP */ 0,
	/* VK_MEDIA_PLAY_PAUSE */ 0,
	/* VK_LAUNCH_MAIL */ 0,
	/* VK_LAUNCH_MEDIA_SELECT */ 0,
	/* VK_LAUNCH_APP1 */ 0,
	/* VK_LAUNCH_APP2 */ 0,
	/* VK_LAUNCH_APP2 */ 0,
	/* 0xB8 - 0xB9 */ 0, 0,
	/* VK_OEM_1 */ 0,
	/* VK_OEM_PLUS */ KEY_ADD,
	/* VK_OEM_COMMA */ KEY_COMMA,
	/* VK_OEM_MINUS */ KEY_SUB,
	/* VK_OEM_PERIOD */ KEY_PERIOD,
	/* VK_OEM_2 */ 0,
	/* VK_OEM_3 */ 0,
	/* 0xC1 - 0xD7 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	/* 0xD8 - 0xDA */ 0, 0, 0,
	/* VK_OEM_4 */ 0,
	/* VK_OEM_5 */ 0,
	/* VK_OEM_6 */ 0,
	/* VK_OEM_7 */ 0,
	/* VK_OEM_8 */ 0,
	/** ... more to handle if we need them !! **/
};
#define WIN32_MAPKEY(vk) &app.keys[win32_vk_to_nbytes[(vk)]]

int
nbytes__win32_utf8_to_utf16(wchar_t *widestring, int wcslen, const char *multibyte, int mbs_len)
{
	int _wcs_len = MultiByteToWideChar(CP_UTF8, 0, multibyte, mbs_len, 0, 0);
	if (_wcs_len <= wcslen) { MultiByteToWideChar(CP_UTF8, 0, multibyte, mbs_len, widestring, _wcs_len); }
	return _wcs_len;
}

int
nbytes__win32_utf16_to_utf8(char *multibyte, int mbs_len, wchar_t *widestring, int wcs_len)
{
	int _mbs_len = WideCharToMultiByte(CP_UTF8, 0, widestring, wcs_len, 0, 0, 0, 0);
	if (_mbs_len <= mbs_len) { WideCharToMultiByte(CP_UTF8, 0, widestring, wcs_len, multibyte, _mbs_len, 0, 0); }
	return _mbs_len;
}


typedef struct Win32Window {
	wchar_t title[NBYTES_WIN32_MAX_TITLE_LEN];
	HWND hwnd;
	HDC hdc;
	WINDOWPLACEMENT lastpos;
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
nbytes__win32_wndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg) {
		case WM_CREATE: {
			#if NBYTES_WIN32_USE_RAWINPUT
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
			#endif
		} break;
		case WM_HOTKEY : {
			Event event = { EVENT_HOTKEY_PRESSED };
			event.hotkey.id = wParam;
			event.hotkey.keymod = ((lParam & 0x4) != 0) << 0 |
			                      ((lParam & 0x2) != 0) << 1 |
			                      ((lParam & 0x1) != 0) << 2 |
			                      ((lParam & 0x8) != 0) << 3;
			app.events[app.num_events++] = event;
		} break;
		case WM_ACTIVATE: {
			int state = wParam & 0xffff;
			app.window.focus = (state >= 1);
		} break;
		case WM_LBUTTONUP: case WM_LBUTTONDOWN: {
			Event event = { (uMsg == WM_LBUTTONDOWN ? EVENT_MOUSE_DOWN : EVENT_MOUSE_UP) };
			event.mouse.btn = VK_LBUTTON;
			event.mouse.down = (uMsg == WM_LBUTTONDOWN);
			event.mouse.pos = INT2(lParam & 0xffff, lParam >> 16);
			app.events[app.num_events++] = event;

			nbytes__update_keystate(WIN32_MAPKEY(VK_LBUTTON), (uMsg == WM_LBUTTONDOWN));
		} break;
		case WM_MBUTTONUP: case WM_MBUTTONDOWN: {
			Event event = { (uMsg == WM_MBUTTONDOWN ? EVENT_MOUSE_DOWN : EVENT_MOUSE_UP) };
			event.mouse.btn = VK_MBUTTON;
			event.mouse.down = (uMsg == WM_MBUTTONDOWN);
			event.mouse.pos = INT2(lParam & 0xffff, lParam >> 16);
			app.events[app.num_events++] = event;

			nbytes__update_keystate(WIN32_MAPKEY(VK_MBUTTON), (uMsg == WM_MBUTTONDOWN));
		} break;
		case WM_RBUTTONUP: case WM_RBUTTONDOWN: {
			Event event = { (uMsg == WM_RBUTTONDOWN ? EVENT_MOUSE_DOWN : EVENT_MOUSE_UP) };
			event.mouse.btn = VK_RBUTTON;
			event.mouse.down = (uMsg == WM_RBUTTONDOWN);
			event.mouse.pos = INT2(lParam & 0xffff, lParam >> 16);
			app.events[app.num_events++] = event;

			nbytes__update_keystate(WIN32_MAPKEY(VK_RBUTTON), (uMsg == WM_RBUTTONDOWN));
		} break;

		case WM_XBUTTONUP: case WM_XBUTTONDOWN:	{
			Event event = { (uMsg == WM_XBUTTONDOWN ? EVENT_MOUSE_DOWN : EVENT_MOUSE_UP) };
			event.mouse.btn = (wParam == MK_XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
			event.mouse.down = (uMsg == WM_XBUTTONDOWN);
			event.mouse.pos = INT2(lParam & 0xffff, lParam >> 16);
			app.events[app.num_events++] = event;

			nbytes__update_keystate(WIN32_MAPKEY((wParam == MK_XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2)), (uMsg == WM_XBUTTONDOWN));
		} break;
		case WM_KEYUP: case WM_KEYDOWN: {
			/*
			Maybe throw away all keyboard and mouse messages while we are dragging a window?

			if(win32_dragging) { break; }
			*/
			Event event = { (uMsg == WM_KEYDOWN ? EVENT_KEY_DOWN : EVENT_KEY_UP) };
			event.key.vk = wParam;
			event.key.repeat = lParam & 0xff;
			event.key.down = (uMsg == WM_KEYDOWN);
			app.events[app.num_events++] = event;

			//unsigned int scancode = (lParam >> 16) & 0xff;
			//unsigned int vk_ex = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);

			nbytes__update_keystate(WIN32_MAPKEY(wParam), (uMsg == WM_KEYDOWN));
		} break;
		#if NBYTES_WIN32_USE_RAWINPUT
		case WM_INPUT: {
			RAWINPUT raw = {0};
			unsigned int size = sizeof(RAWINPUT);
			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &size, sizeof(RAWINPUTHEADER))) {
				switch (raw.header.dwType) {
					case RIM_TYPEKEYBOARD: {
						RAWKEYBOARD *raw_kb = &raw.data.keyboard;
						unsigned int vk = raw_kb->VKey;
						unsigned int scancode = raw_kb->MakeCode;
						unsigned int flags = raw_kb->Flags;
						bool is_e0   = (flags & RI_KEY_E0) != 0;
						bool is_e1   = (flags & RI_KEY_E1) != 0;
						bool is_up   = (flags & RI_KEY_BREAK) != 0;
						bool is_down = !is_up;

						wchar_t name[64] = {0}; char utf8name[64] = {0};
						unsigned int key = (scancode << 16) | (is_e0 << 24);
						GetKeyNameText(key, name, 64);
						nbytes__win32_utf16_to_utf8(utf8name, 64, name, wcslen(name));
						printf("%s:%X\n", utf8name, scancode);
					}
				}
			}
		} break;
		#endif
		case WM_MOUSEMOVE: {
			Event event = { EVENT_MOUSE_MOVE };
			event.mouse.btn = 0;
			event.mouse.down = 0;
			event.mouse.pos = INT2(lParam & 0xffff, lParam >> 16);
			app.events[app.num_events++] = event;

			app.mouse.relative = event.mouse.pos;
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
nbytes__win32_handle_messages(PVOID lpParameter)
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

bool
nbytes__win32_prepare_fibers()
{
	static bool is_initialized;
	if(!is_initialized) {
		win32_main_fiber = ConvertThreadToFiber(0);
		win32_msg_fiber = CreateFiber(0, nbytes__win32_handle_messages, 0);
		assert(win32_main_fiber && win32_msg_fiber);

		is_initialized = true;
	}
	return is_initialized;
}

bool
nbytes__win32_set_vsync(bool enable)
{
	if (!opengl_loader_supports_ext("WGL_EXT_swap_control")) { return false; }
	return wglSwapIntervalEXT(enable);
}

bool
nbytes__win32_get_vsync()
{
	if (!opengl_loader_supports_ext("WGL_EXT_swap_control")) { return false; }
	return wglGetSwapIntervalEXT();
}

void
nbytes__win32_set_size(Win32Window *wnd, int2 size)
{
	if(win32_dragging) { return; }
	RECT rc = {0, 0, (long) size.x, (long) size.y};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, 0);
	SetWindowPos(wnd->hwnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);
}

int2
nbytes__win32_get_size(Win32Window *wnd)
{
	RECT rc;
	GetClientRect(wnd->hwnd, &rc);
	return INT2(rc.right, rc.bottom);
}

void
nbytes__win32_set_pos(Win32Window *wnd, int2 pos)
{
	if(win32_dragging) { return; }
	SetWindowPos(wnd->hwnd, 0, pos.x, pos.y, 0, 0, SWP_NOSIZE);
}

int2
nbytes__win32_get_pos(Win32Window *wnd)
{
	RECT rect;
	GetWindowRect(wnd->hwnd, &rect);
	return INT2(rect.left, rect.top);
}

bool
nbytes__win32_set_title(Win32Window *wnd, const char *title, int len)
{
	size_t res = nbytes__win32_utf8_to_utf16(wnd->title, NBYTES_WIN32_MAX_TITLE_LEN, title, len);
	assert(res == len);
	return SetWindowTextW(wnd->hwnd, wnd->title);
}

bool
nbytes__win32_set_fullscreen(Win32Window *wnd, BOOL enable)
{
	// NOTE(casey): This follows Raymond Chen's prescription
	// for fullscreen toggling, see:
	// http://blogs.msdn.com/b/oldnewthing/archive/2010/04/12/9994016.aspx

	DWORD Style = GetWindowLong(wnd->hwnd, GWL_STYLE);
	if(Style & WS_OVERLAPPEDWINDOW && enable) {
		MONITORINFO MonitorInfo = {sizeof(MonitorInfo)};
		if(GetWindowPlacement(wnd->hwnd, &wnd->lastpos) &&
		   GetMonitorInfo(MonitorFromWindow(wnd->hwnd, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo)) {
			SetWindowLong(wnd->hwnd, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(wnd->hwnd, HWND_TOP,
			             MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
			             MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
			             MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
			             SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	} else {
		SetWindowLong(wnd->hwnd, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(wnd->hwnd, &wnd->lastpos);
		SetWindowPos(wnd->hwnd, 0, 0, 0, 0, 0,
		             SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
		             SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	return true;
}


void
nbytes__win32_swapbuffers(Win32Window *wnd)
{
	SwapBuffers(wnd->hdc);
}

bool
nbytes_register_hotkey(int id, int vk, int mod)
{
	// id must be within the region 0x0000 - 0xBFFF
	if(id >= 0xBFFF) { return false; }
	int winmod = ((mod & KEYMOD_ALT) != 0) << 0 |
	             ((mod & KEYMOD_CTRL) != 0) << 1 |
	             ((mod & KEYMOD_SHIFT) != 0 ) << 2 |
	             ((mod & KEYMOD_META) != 0)  << 3 |
	             MOD_NOREPEAT;
	return(RegisterHotKey(win32_window.hwnd, id, winmod, vk));
}

void
nbytes_update_events()
{
	app.keymod = 0;
	for(int vk = 0; vk < NBYTES_NUM_MAX_KEYS; vk++) {
		nbytes__reset_keystate(&app.keys[vk]);
	}

	#ifdef NBYTES_ZERO_EVENT_BUFFER
	memset(app.events, 0, app.num_events);
	#endif
	app.num_events = 0;

	/*
	Windows message queue workaround with Fibers to prevent the recursive calls into
	the wndproc when dragging a window. Due to fiber switches we can jump out of the
	message fiber wndproc back to the main routine whenever we want.
	*/
	SwitchToFiber(win32_msg_fiber);

	nbytes__update_keystate(&app.keys[KEY_LSHIFT], GetAsyncKeyState(VK_LSHIFT));
	nbytes__update_keystate(&app.keys[KEY_RSHIFT], GetAsyncKeyState(VK_RSHIFT));
	nbytes__update_keystate(&app.keys[KEY_LCTRL], GetAsyncKeyState(VK_LCONTROL));
	nbytes__update_keystate(&app.keys[KEY_RCTRL], GetAsyncKeyState(VK_RCONTROL));
	nbytes__update_keystate(&app.keys[KEY_LALT], GetAsyncKeyState(VK_LMENU));
	nbytes__update_keystate(&app.keys[KEY_RALT], GetAsyncKeyState(VK_RMENU));
	nbytes__update_keystate(&app.keys[KEY_LMETA], GetAsyncKeyState(VK_LWIN));
	nbytes__update_keystate(&app.keys[KEY_RMETA], GetAsyncKeyState(VK_RWIN));

	app.keymod |= (app.keys[KEY_LSHIFT].down ? KEYMOD_LSHIFT : 0);
	app.keymod |= (app.keys[KEY_RSHIFT].down ? KEYMOD_RSHIFT : 0);
	app.keymod |= (app.keys[KEY_LCTRL].down  ? KEYMOD_LCTRL : 0);
	app.keymod |= (app.keys[KEY_RCTRL].down  ? KEYMOD_RCTRL : 0);
	app.keymod |= (app.keys[KEY_LALT].down   ? KEYMOD_LALT : 0);
	app.keymod |= (app.keys[KEY_RALT].down   ? KEYMOD_RALT : 0);
	app.keymod |= (app.keys[KEY_LMETA].down  ? KEYMOD_LMETA : 0);
	app.keymod |= (app.keys[KEY_RMETA].down  ? KEYMOD_RMETA : 0);

	if(win32_dragging) {
		// win32 hack to reset keystate when the window is currently in dragmode.
		app.keymod = 0;
		for(int vk = 0; vk < NBYTES_NUM_MAX_KEYS; vk++) {
			nbytes__reset_keystate(&app.keys[vk]);
			app.keys[vk].down = false;
		}
	}

	POINT cursor;
	GetCursorPos(&cursor);
	app.mouse.screen = INT2(cursor.x, cursor.y);
	app.mouse.delta = INT2(app.mouse.screen.x - app.mouse.prev_state.screen.x, app.mouse.screen.y - app.mouse.prev_state.screen.y);
	app.mouse.wheel_delta = app.mouse.wheel - app.mouse.prev_state.wheel;
	app.mouse.prev_state.screen = app.mouse.screen;
	app.mouse.prev_state.wheel = app.mouse.wheel;
}

void
nbytes_render_window()
{
	Win32Window *wnd = &win32_window;
	nbytes__win32_swapbuffers(wnd);
}


void
nbytes_free_window()
{
	if(app.window.borderless_fullscreen) {
		/*
		Fullscreen Hack

		Sometimes if the user manually quit the appication and we are in fullscreen mode the previous windows were
		not clickable anymore. You had to minimize and maximize them to get the active again. Dunno why this happens
		but this hack might fix it!
		*/
		nbytes__win32_set_fullscreen(&win32_window, false);
	}

	DestroyWindow(win32_window.hwnd);
}

bool
nbytes_init_window()
{
	nbytes__win32_prepare_fibers();

	if(!app.window.title) { app.window.title = NBYTES_DEFAULT_TITLE; }
	app.window.pos.x = (app.window.pos.x == NBYTES_DEFAULT_WINDOW_POS ? CW_USEDEFAULT : app.window.pos.x);
	app.window.pos.y = (app.window.pos.y == NBYTES_DEFAULT_WINDOW_POS ? CW_USEDEFAULT : app.window.pos.y);
	app.window.size.x = (app.window.size.x == 0 ? NBYTES_DEFAULT_WINDOW_WIDTH : app.window.size.x);
	app.window.size.y = (app.window.size.y == 0 ? NBYTES_DEFAULT_WINDOW_HEIGHT : app.window.size.y);

	app.window.opengl.major = (app.window.opengl.major == 0 ? NBYTES_DEFAULT_OPENGL_MAJOR : app.window.opengl.major);
	app.window.opengl.minor = (app.window.opengl.minor == 0 ? NBYTES_DEFAULT_OPENGL_MINOR : app.window.opengl.minor);


	WNDCLASSEXW wnd_class   = { sizeof(wnd_class) };
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wnd_class.lpfnWndProc   = nbytes__win32_wndproc;
	wnd_class.lpszClassName = NBYTES_WIN32_WNDCLASS;
	wnd_class.hCursor       = LoadCursor(0, IDC_ARROW);

	DWORD last_error = 0;
	ATOM registered 		= RegisterClassExW(&wnd_class);
	last_error 				= GetLastError();
	if(last_error != 0 && last_error != 1410) { fatal("RegisterClassExW() failed. GetLastError = %i", last_error); }

	RECT rc = {0, 0, (long) app.window.size.x, (long) app.window.size.y};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, 0);


	DWORD style = WS_OVERLAPPEDWINDOW;
	HWND hwnd = CreateWindowExW(0, wnd_class.lpszClassName, L"", style, app.window.pos.x, app.window.pos.y, rc.right - rc.left, rc.bottom - rc.top, 0, 0, wnd_class.hInstance, 0);
	if(!hwnd) { fatal("CreateWindowExW() failed. GetLastError = %i", GetLastError()); }

	HGLRC ctx 		= 0;
	HDC hdc 		= GetDC(hwnd);
	if (!hdc) { fatal("GetDC() failed. GetLastError = %i", GetLastError()); }

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

	if(app.window.borderless_fullscreen != app.window.prev_state.borderless_fullscreen) {
		nbytes__win32_set_fullscreen(wnd, app.window.borderless_fullscreen);
	}
	app.window.prev_state.borderless_fullscreen = app.window.borderless_fullscreen;


	if(app.window.size.x != app.window.prev_state.size.x || app.window.size.y != app.window.prev_state.size.y) {
		nbytes__win32_set_size(wnd, app.window.size);
	}
	app.window.size = nbytes__win32_get_size(wnd);
	app.window.resized = (app.window.prev_state.size.x != app.window.size.x || app.window.prev_state.size.y != app.window.size.y);
	app.window.prev_state.size = app.window.size;

	if(app.window.pos.x != app.window.prev_state.pos.x || app.window.pos.y != app.window.prev_state.pos.y) {
		nbytes__win32_set_pos(wnd, app.window.pos);
	}
	app.window.pos = nbytes__win32_get_pos(wnd);
	app.window.moved = (app.window.prev_state.pos.x != app.window.pos.x || app.window.prev_state.pos.y != app.window.pos.y);
	app.window.prev_state.pos = app.window.pos;

	if(app.window.opengl.vsync != app.window.prev_state.opengl.vsync) {
		nbytes__win32_set_vsync(app.window.opengl.vsync);
		app.window.prev_state.opengl.vsync = nbytes__win32_get_vsync();
		if(app.window.prev_state.opengl.vsync != app.window.opengl.vsync) {
			warn("Failed to set VSync to \"%s\". It's either disabled by driver or not available!", app.window.opengl.vsync ? "true" : "false");
		}
		app.window.opengl.vsync = app.window.prev_state.opengl.vsync;
	}

	if(app.window.title != app.window.prev_state.title) {
		nbytes__win32_set_title(&win32_window, app.window.title, strlen(app.window.title));
		app.window.prev_state.title = app.window.title;
	}

	if(app.window.focus && app.window.borderless_fullscreen) {
		RECT r = {app.window.pos.x, app.window.pos.y, app.window.pos.x + app.window.size.width, app.window.pos.y + app.window.size.height};
		ClipCursor(&r);
	} else {
		ClipCursor(0);
	}
}

bool
nbytes_init_display()
{
	DEVMODEW display_settings;
	if(!EnumDisplaySettingsW(0, ENUM_CURRENT_SETTINGS, &display_settings)) { return false; }
	app.display.size = INT2(display_settings.dmPelsWidth, display_settings.dmPelsHeight);
	app.display.refresh_rate = display_settings.dmDisplayFrequency;
	app.display.dpi = GetDeviceCaps(win32_window.hdc, LOGPIXELSX);
	return true;
}

bool
nbytes_init_time()
{
	LARGE_INTEGER frequency;
	LARGE_INTEGER counter;
	if(!QueryPerformanceFrequency(&frequency) || !QueryPerformanceCounter(&counter)) { return false; }
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
	uint64_t delta_ticks = ticks - app.time.ticks;

	app.time.nsecs = (ticks * 1000 * 1000 * 1000) / app.time.ticks_per_sec;
	app.time.usecs = (ticks * 1000 * 1000) / app.time.ticks_per_sec;
	app.time.msecs = (ticks * 1000) / app.time.ticks_per_sec;
	app.time.secs = (double) ticks / (double) app.time.ticks_per_sec;

	app.time.delta_nsecs = (int)((delta_ticks * 1000 * 1000 * 1000) / app.time.ticks_per_sec);
	app.time.delta_usecs = (int)((delta_ticks * 1000 * 1000) / app.time.ticks_per_sec);
	app.time.delta_msecs = (int)((delta_ticks * 1000) / app.time.ticks_per_sec);
	app.time.delta_secs = (float)delta_ticks / (float)app.time.ticks_per_sec;

	app.time.ticks = ticks;
	app.time.delta_ticks = delta_ticks;
	return true;
}
