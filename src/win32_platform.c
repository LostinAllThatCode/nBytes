#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "winmm.lib")

#define UNICODE
#define VC_EXTRA_LEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

#if NBYTES_DEBUGGING
	#pragma comment(lib, "dbghelp.lib")
	#include <DbgHelp.h>
	#include "win32_debug.c"
#endif

#define GLLOADER_USE_WGL
#include "opengl_loader.c"

#define NBYTES_WIN32_USE_RAWINPUT 	0
#define NBYTES_WIN32_WNDCLASS 		L"NBYTES_WIN32_WNDCLASS"
#define NBYTES_WIN32_MAX_TITLE_LEN 	512

#define NBYTES_WIN32_MAX_PATH 		1024
typedef struct Win32NotifiyIO {
	HANDLE *handles;
	const char **paths;
} Win32NotifiyIO;

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

static bool win32_dragging;
static Win32NotifiyIO win32_ionotfiy_list;
static Win32Window win32_window;
static HANDLE win32_main_fiber;
static HANDLE win32_msg_fiber;

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

uint64_t
win32__filetime_to_u64(FILETIME ft)
{
	return (uint64_t) ft.dwHighDateTime << 32 | ft.dwLowDateTime;
}

uint64_t
win32__filesize_to_u64(DWORD high, DWORD low)
{
	return (uint64_t) high << 32 | low;
}


int
win32__utf8_to_utf16(wchar_t *widestring, int wcslen, const char *multibyte, int mbs_len)
{
	int _wcs_len = MultiByteToWideChar(CP_UTF8, 0, multibyte, mbs_len, 0, 0);
	if (_wcs_len <= wcslen) { MultiByteToWideChar(CP_UTF8, 0, multibyte, mbs_len, widestring, _wcs_len); }
	return _wcs_len;
}

int
win32__utf16_to_utf8(char *multibyte, int mbs_len, wchar_t *widestring, int wcs_len)
{
	int _mbs_len = WideCharToMultiByte(CP_UTF8, 0, widestring, wcs_len, 0, 0, 0, 0);
	if (_mbs_len <= mbs_len) { WideCharToMultiByte(CP_UTF8, 0, widestring, wcs_len, multibyte, _mbs_len, 0, 0); }
	return _mbs_len;
}

LRESULT CALLBACK
win32__wndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
						win32__utf16_to_utf8(utf8name, 64, name, wcslen(name));
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
			SetTimer(hwnd, 0, 1, 0);
		} break;
		case WM_EXITMENULOOP:
		case WM_EXITSIZEMOVE: {
			win32_dragging = false;
			KillTimer(hwnd, 0);
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
win32__handle_messages(PVOID lpParameter)
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
win32__prepare_fibers()
{
	static bool is_initialized;
	if(!is_initialized) {
		win32_main_fiber = ConvertThreadToFiber(0);
		win32_msg_fiber = CreateFiber(0, win32__handle_messages, 0);
		assert(win32_main_fiber && win32_msg_fiber);

		is_initialized = true;
	}
	return is_initialized;
}

bool
win32__set_vsync(bool enable)
{
	if (!opengl_loader_supports_ext("WGL_EXT_swap_control")) { return false; }
	return wglSwapIntervalEXT(enable);
}

bool
win32__get_vsync()
{
	if (!opengl_loader_supports_ext("WGL_EXT_swap_control")) { return false; }
	return wglGetSwapIntervalEXT();
}

void
win32__set_size(Win32Window *wnd, int2 size)
{
	if(win32_dragging) { return; }
	RECT rc = {0, 0, (long) size.x, (long) size.y};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, 0);
	SetWindowPos(wnd->hwnd, 0, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE);
}

int2
win32__get_size(Win32Window *wnd)
{
	RECT rc;
	GetClientRect(wnd->hwnd, &rc);
	return INT2(rc.right, rc.bottom);
}

void
win32__set_pos(Win32Window *wnd, int2 pos)
{
	if(win32_dragging) { return; }
	SetWindowPos(wnd->hwnd, 0, pos.x, pos.y, 0, 0, SWP_NOSIZE);
}

int2
win32__get_pos(Win32Window *wnd)
{
	RECT rect;
	GetWindowRect(wnd->hwnd, &rect);
	return INT2(rect.left, rect.top);
}

bool
win32__set_title(Win32Window *wnd, const char *title, int len)
{
	size_t res = win32__utf8_to_utf16(wnd->title, NBYTES_WIN32_MAX_TITLE_LEN, title, len);
	assert(res == len);
	return SetWindowTextW(wnd->hwnd, wnd->title);
}

bool
win32__set_fullscreen(Win32Window *wnd, BOOL enable)
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
win32__swapbuffers(Win32Window *wnd)
{
	SwapBuffers(wnd->hdc);
}

bool
win32__readdirw(ResourceFile **files, wchar_t *root, wchar_t *subdir)
{
	wchar_t search_dir[MAX_PATH] = {0};
	wchar_t target_dir[MAX_PATH] = {0};

	ResourceFile resfile;
	WIN32_FIND_DATAW finddata;

	if(subdir) {
		_snwprintf(target_dir, MAX_PATH, L"%s/%s", root, subdir);
		_snwprintf(search_dir, MAX_PATH, L"%s/%s/*", root, subdir);
	} else {
		_snwprintf(target_dir, MAX_PATH, L"%s", root);
		_snwprintf(search_dir, MAX_PATH, L"%s/*", root);
	}

	HANDLE ff = FindFirstFileW(search_dir, &finddata);
	if(ff == INVALID_HANDLE_VALUE) { return false; }
	do {
		if (finddata.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
			if(finddata.cFileName[0] == '.' || (finddata.cFileName[0] == '.' && finddata.cFileName[1] == '.')) {
				continue;
			}
			win32__readdirw(files, target_dir, finddata.cFileName);
		} else {
			// Use a hashmap for this??
			char tmp[MAX_PATH] = {0};
			wchar_t wfullpath[MAX_PATH] = {0};
			wchar_t wtmp[MAX_PATH] = {0};
			_snwprintf(wtmp, sizeof(wtmp), L"%s\\%s", target_dir, finddata.cFileName);

			DWORD length = GetFullPathNameW(wtmp, MAX_PATH, wfullpath, 0);
			int ut8flen = win32__utf16_to_utf8(tmp, MAX_PATH, wfullpath, length);
			filename_format(tmp, ut8flen);

			for(int i = 0; i < buf_len(*files); i++) {
				ResourceFile *resfile = *files + i;
				if(strcmp(tmp, resfile->filename) == 0) {
					uint64_t now_time = win32__filetime_to_u64(finddata.ftLastWriteTime);
					if(now_time != resfile->info.modification_time) {

						// File did change!
						resfile->was_modified = true;
						resfile->info.modification_time = now_time;
					}
					continue;
				}
			}

			// Didn't find a match, so we add this file to our resource files.
			memset(&resfile, 0, sizeof(resfile));
			resfile.id = buf_len(*files);
			resfile.filename = str_dup(tmp, ut8flen);
			resfile.extension = utf8_find_extension(resfile.filename);
			resfile.info.filesize = win32__filesize_to_u64(finddata.nFileSizeHigh, finddata.nFileSizeLow);
			resfile.info.modification_time = win32__filetime_to_u64(finddata.ftLastWriteTime);
			buf_push(*files, resfile);
		}
	} while( FindNextFileW(ff, &finddata) );
	FindClose(ff);
	return true;
}

bool
win32__readdir(ResourceFile **files, const char *dir)
{
	wchar_t search_dir[MAX_PATH] = {0};
	wchar_t target_dir[MAX_PATH] = {0};
	WIN32_FIND_DATAW finddata;

	size_t len = strlen(dir);
	win32__utf8_to_utf16(target_dir, MAX_PATH, dir, len);
	win32__readdirw(files, target_dir, 0);

	return true;
}

bool
win32__ionotify_register(const char *path)
{
	int ionoti_count = buf_len(win32_ionotfiy_list.handles);
	for(int i = 0; i < ionoti_count; i++) {
		if(strcmp(path, win32_ionotfiy_list.paths[i]) == 0) {
			return true;
		}
	}

	wchar_t wpath[NBYTES_WIN32_MAX_PATH] = {0};
	int req_len = win32__utf8_to_utf16(wpath, NBYTES_WIN32_MAX_PATH, path, strlen(path));
	HANDLE new_handle = FindFirstChangeNotificationW(wpath, TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE);
	if(new_handle == INVALID_HANDLE_VALUE) {
		warn("Unable to register io notifiy handler for %s.", path);
		return false;
	}

	buf_push(win32_ionotfiy_list.handles, new_handle);
	buf_push(win32_ionotfiy_list.paths, path);

	return true;
}

bool
win32__ionotify_unregister(int idx)
{
	int ionoti_count = buf_len(win32_ionotfiy_list.handles);
	if(idx > ionoti_count) { return false; }

	FindCloseChangeNotification(win32_ionotfiy_list.handles[idx]);
	buf_remove(win32_ionotfiy_list.handles, idx);
	buf_remove(win32_ionotfiy_list.paths, idx);
	return true;
}

bool
win32__ionotify_wait(unsigned int timeout)
{
	int ionoti_count = buf_len(win32_ionotfiy_list.handles);
	DWORD status = WaitForMultipleObjects(ionoti_count, win32_ionotfiy_list.handles, FALSE, timeout);
	if(status == WAIT_OBJECT_0) {
		HANDLE handle = win32_ionotfiy_list.handles[status];
		win32__readdir(&app.rsm.files, app.rsm.root_dir);
		FindNextChangeNotification(handle);
		return true;
	}
	return false;
}




