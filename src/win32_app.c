
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
	win32__swapbuffers(wnd);
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
		win32__set_fullscreen(&win32_window, false);
	}

	DestroyWindow(win32_window.hwnd);
}

bool
nbytes_init_window()
{
	win32__prepare_fibers();

	if(!app.window.title) { app.window.title = NBYTES_DEFAULT_TITLE; }
	app.window.pos.x = (app.window.pos.x == NBYTES_DEFAULT_WINDOW_POS ? CW_USEDEFAULT : app.window.pos.x);
	app.window.pos.y = (app.window.pos.y == NBYTES_DEFAULT_WINDOW_POS ? CW_USEDEFAULT : app.window.pos.y);
	app.window.size.x = (app.window.size.x == 0 ? NBYTES_DEFAULT_WINDOW_WIDTH : app.window.size.x);
	app.window.size.y = (app.window.size.y == 0 ? NBYTES_DEFAULT_WINDOW_HEIGHT : app.window.size.y);

	app.window.opengl.major = (app.window.opengl.major == 0 ? NBYTES_DEFAULT_OPENGL_MAJOR : app.window.opengl.major);
	app.window.opengl.minor = (app.window.opengl.minor == 0 ? NBYTES_DEFAULT_OPENGL_MINOR : app.window.opengl.minor);


	WNDCLASSEXW wnd_class   = { sizeof(wnd_class) };
	wnd_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wnd_class.lpfnWndProc   = win32__wndproc;
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

	opengl_loader_init();	// Initialize opengl driver... grabs function pointers and extensions

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
		win32__set_fullscreen(wnd, app.window.borderless_fullscreen);
	}
	app.window.prev_state.borderless_fullscreen = app.window.borderless_fullscreen;


	if(app.window.size.x != app.window.prev_state.size.x || app.window.size.y != app.window.prev_state.size.y) {
		win32__set_size(wnd, app.window.size);
	}
	app.window.size = win32__get_size(wnd);
	app.window.resized = (app.window.prev_state.size.x != app.window.size.x || app.window.prev_state.size.y != app.window.size.y);
	app.window.prev_state.size = app.window.size;

	if(app.window.pos.x != app.window.prev_state.pos.x || app.window.pos.y != app.window.prev_state.pos.y) {
		win32__set_pos(wnd, app.window.pos);
	}
	app.window.pos = win32__get_pos(wnd);
	app.window.moved = (app.window.prev_state.pos.x != app.window.pos.x || app.window.prev_state.pos.y != app.window.pos.y);
	app.window.prev_state.pos = app.window.pos;

	if(app.window.opengl.vsync != app.window.prev_state.opengl.vsync) {
		win32__set_vsync(app.window.opengl.vsync);
		app.window.prev_state.opengl.vsync = win32__get_vsync();
		if(app.window.prev_state.opengl.vsync != app.window.opengl.vsync) {
			warn("Failed to set VSync to \"%s\". It's either disabled by driver or not available!", app.window.opengl.vsync ? "true" : "false");
		}
		app.window.opengl.vsync = app.window.prev_state.opengl.vsync;
	}

	if(app.window.title != app.window.prev_state.title) {
		win32__set_title(&win32_window, app.window.title, strlen(app.window.title));
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
