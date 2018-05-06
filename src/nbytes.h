#ifndef INCLUDE_GUARD_NBYTES_H
#define INCLUDE_GUARD_NBYTES_H

#define NBYTES_DEBUGGING 1

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
#include <time.h>

#include "shared.c"
#include "math.c"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "fonts.c"

#include "keys.h"
#include "resman.h"

typedef enum EventType {
	EVENT_NONE,
	EVENT_KEY_DOWN,
	EVENT_KEY_UP,
	EVENT_MOUSE_DOWN,
	EVENT_MOUSE_UP,
	EVENT_MOUSE_MOVE,
	EVENT_HOTKEY_PRESSED,
	EVENT_FILE_ADDED,
	EVENT_FILE_REMOVED,
	EVENT_FILE_CONTENT_CHANGED,
} EventType;

typedef struct Event {
	EventType type;
	union {
		struct {
			int vk;
			int repeat;
			bool down;
		} key;
		struct {
			int btn;
			bool down;
			int2 pos;
		} mouse;
		struct {
			int id;
			int keymod;
		} hotkey;
		struct {
			const char *name;
		} file;
	};
} Event;

typedef struct Mouse {
	int2 screen;
	int2 relative;
	int2 delta;
	int wheel, wheel_delta;

	struct {
		int2 screen;
		int2 relative;
		int2 delta;
		int wheel, wheel_delta;
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

	uint64_t ticks;
	uint64_t nsecs;
	uint64_t usecs;
	uint64_t msecs;
	double secs;
} Time;

typedef struct Display {
	int dpi;
	int2 size;
	int refresh_rate;
} Display;

typedef struct Window {
	bool hidden;
	bool focus;
	bool borderless_fullscreen;

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
		bool borderless_fullscreen;

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

#define NBYTES_DEFAULT_WINDOW_POS 			0x80000000
#define NBYTES_DEFAULT_WINDOW_WIDTH 		800
#define NBYTES_DEFAULT_WINDOW_HEIGHT 		600
#define NBYTES_DEFAULT_TITLE 				"nBytes"

#define NBYTES_NUM_MAX_EVENTS 				512
#define NBYTES_NUM_MAX_KEYS 				256
typedef struct App {
	bool quit;
	int num_updates;

	Event events[NBYTES_NUM_MAX_EVENTS];
	int num_events;

	int keymod;
	Keystate keys[NBYTES_NUM_MAX_KEYS];

	Time time;
	Mouse mouse;
	Display display;
	Window window;

	ResourceManager rsm;

	const char *error;
} App;
extern App app;

#if defined OS_WINDOWS
	#include "win32_platform.c"
	#include "win32_app.c"
	#include "win32_resman.c"
#elif defined OS_LINUX
	#error Implementation missing! @TODO: OS_LINUX
#else
	#error Fatal Error: This platform is not supported or not identified as a supported one!
#endif

bool
nbytes_update()
{
	if(app.quit) {
		nbytes_free_window();
		nbytes_update_events();
		return false;
	} else {
		nbytes_update_rsm();
		nbytes_update_events();
		nbytes_update_window();
		nbytes_update_time();
		app.num_updates++;
		return !app.quit;
	}
}

bool
nbytes_init()
{
	#if NBYTES_DEBUGGING
	nbytes_init_debugging();
	#endif

	if(!nbytes_init_window()) { return false; }
	if(!nbytes_init_display()) { return false; };
	if(!nbytes_init_time()) { return false; };

	if(!nbytes_init_rsm()) { return false; }
	return nbytes_update();
}

void
nbytes_check_prefined_hotkeys()
{
	// exit application
	if(app.keys[KEY_ESC].pressed) {
		app.quit = true;
	}

	// toggle vsync
	if(app.keys[KEY_V].pressed) {
		app.window.opengl.vsync = !app.window.opengl.vsync;
	}

	// toggle borderless fullscreen
	if(app.keys[KEY_F].pressed) {
		app.window.borderless_fullscreen = !app.window.borderless_fullscreen;
	}
}

void
nbytes_limit_framerate(uint64_t next_tick_us)
{
	if(!app.window.opengl.vsync) {
		int64_t ms_sleep = (int64_t)(next_tick_us - app.time.usecs) / 1000;
		if (ms_sleep > 0) {
			#if OS_WINDOWS
			// TODO: @Cleanup
			timeBeginPeriod(1);
			Sleep(ms_sleep);
			timeEndPeriod(1);
			#endif
		}
	}
}
#endif
