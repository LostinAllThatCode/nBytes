#ifndef INCLUDE_GUARD_NBYTES_H
#define INCLUDE_GUARD_NBYTES_H

// C-Standard library stuff
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

#include "shared.c"
#include "math.c"

#if defined OS_WINDOWS
	#pragma comment(lib, "user32.lib")
	#pragma comment(lib, "gdi32.lib")
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "winmm.lib")

	#define UNICODE
	#define VC_EXTRA_LEAN
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#elif defined OS_LINUX
	#error Implementation missing! @TODO: OS_LINUX
#else
	#error Fatal Error: This platform is not supported or not identified as a supported one!
#endif

enum {
	MOD_None,
	MOD_Shift,
	MOD_Ctrl,
	MOD_Alt
};

typedef struct Keystate {
	bool down;
	bool pressed;
	bool released;
} Keystate;

__forceinline void
input_reset_keystate(Keystate *key)
{
	key->pressed = 0;
	key->released = 0;
}

__forceinline void
input_update_keystate(Keystate *key, bool is_down)
{
	bool was_down = key->down;
	key->down = is_down;
	key->pressed = !was_down && is_down;
	key->released = !is_down && was_down;
}

typedef struct Mouse {
	int rel_x, rel_y, dx, dy, wheel, wheel_delta;
	int x, y;
	struct {
		int rel_x, rel_y, dx, dy, wheel, wheel_delta;
		int x, y;
	} prev_state;
} Mouse;

typedef struct Time {
	int ticks_per_sec;
	int start_ticks;

	int delta_ticks;
	int delta_nsecs;
	int delta_usecs;
	int delta_msecs;
	float delta_secs;

	int ticks;
	int nsecs;
	int usecs;
	int msecs;
	float secs;
} Time;

typedef struct Window {
	bool hidden;
	bool focus;

	bool focused;
	bool resized;
	bool moved;

	int2 size;
	int2 pos;
	const char *title;

	struct {
		bool vsync;

		/* Changes to the following variables will have no effect after initialization of the window */
		int major;
		int minor;
		int core_profile;
		int debug;

		const char *debug_msg;
		int debug_msg_len;
	} opengl;

	struct {
		bool hidden;
		bool focus;

		int2 size;
		int2 pos;
		const char *title;
		struct {
			bool vsync;
		} opengl;
	} prev_state;
} Window;

#define NBYTES_DEFAULT_OPENGL_MAJOR 		3
#define NBYTES_DEFAULT_OPENGL_MINOR 		3
#define NBYTES_DEFAULT_OPENGL_DEBUG 		1
#define NBYTES_DEFAULT_OPENGL_CORE_PROFILE	0
#define NBYTES_DEFAULT_OPENGL_VSYNC			true

#define NBYTES_DEFAULT_WINDOW_POS 			0x80000000
#define NBYTES_DEFAULT_WINDOW_WIDTH 		800
#define NBYTES_DEFAULT_WINDOW_HEIGHT 		600
#define NBYTES_DEFAULT_TITLE 				"nBytes"

#define NBYTES_NUM_MAX_KEYS 256
typedef struct App {
	bool quit;

	int keymod;
	Keystate keys[NBYTES_NUM_MAX_KEYS];
	/**
		@TODO: virtual key map!
		Create own vkeys and map them to the platform specific ones
		Right now i just use the VK_KEYS from windows directly!
	**/

	Time time;
	Mouse mouse;
	Window window;
} App;
extern App app;

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "fonts.c"

#if OS_WINDOWS
	#define GLLOADER_USE_WGL
	#include "opengl_loader.c"
	#include "win32_app.c"
#endif

#include "opengl_retro.c"

bool
nbytes_update()
{
	nbytes_update_events();
	nbytes_update_window();
	nbytes_update_time();
	return !app.quit;
}

bool
nbytes_init()
{
	if(!nbytes_init_window()) { return false; }
	nbytes_init_time();
	return nbytes_update();
}



#endif
