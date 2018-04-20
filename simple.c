#include <nbytes.h>
App app;

typedef struct Ball {
	v2 pos;
	v2 size;
	v2 vel;
	v2 dir;
	v4 clr;
	int cs;
} Ball;
static Ball *game_balls;
static BakedFont font;
enum {
	ENTITIES_AS_RECTS,
	ENTITIES_AS_CIRCLES,
};
static int DRAWMODE = ENTITIES_AS_CIRCLES;

void
opengl_setup()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0, 1);
}

void
begin_scene2d()
{
	glViewport(0, 0, app.window.size.x, app.window.size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, app.window.size.x, app.window.size.y, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
}

void
end_scene2d()
{

}

void
effect_change_color(Ball *ball)
{
	ball->cs = (ball->cs + 1) % 4;
	switch(ball->cs) {
		case 0: { ball->clr = vec4(1, 0, 0, 1);} break;
		case 1: { ball->clr = vec4(0, 1, 0, 1);} break;
		case 2: { ball->clr = vec4(0, 0, 1, 1);} break;
		case 3: { ball->clr = vec4(1, 1, 0, 1);} break;
	}
}

void
spawn_new_ball(Ball **balls, float x, float y, float dirx, float diry, float size_x, float size_y)
{
	Ball new_ball = {0};
	new_ball.pos = vec2(x, y);
	new_ball.size = vec2(size_x, size_y);
	new_ball.dir = vec2(SIGN(dirx), SIGN(diry));
	new_ball.vel = vec2(abs(dirx / app.time.delta_secs) * .125f, abs(diry / app.time.delta_secs) * .125f);
	new_ball.cs = rand() % 4;
	effect_change_color(&new_ball);
	buf_push(*balls, new_ball);
}

void
update_balls(Ball *balls)
{
	assert(buf_len(balls) != 0);
	for(int i = 0; i < buf_len(balls); i++) {
		Ball *ball = balls + i;

		ball->pos.x += ball->dir.x * ball->vel.x * app.time.delta_secs;
		ball->pos.y += ball->dir.y * ball->vel.y * app.time.delta_secs;
		if(ball->pos.x - ball->size.x / 2.f <= 1.0f || ball->pos.x + ball->size.x / 2.f >= app.window.size.x + 1.0f ) {
			ball->dir.x *= -1.0f;
			effect_change_color(ball);
		}
		if(ball->pos.y - ball->size.y / 2.f <= 1.0f || 5 + ball->pos.y + ball->size.y / 2.f >= app.window.size.y + 1.0f) {
			ball->dir.y *= -1.0f;
			effect_change_color(ball);
		}

		if(DRAWMODE == ENTITIES_AS_CIRCLES) {
			rgl_draw_filled_circle(ball->pos, ball->clr, ball->size.x / 2);
		}	else {
			rgl_draw_colored_rect2d(RGL_CENTER, ball->pos, ball->clr, ball->size.x, ball->size.y);
		}
	}
}

void
draw_debuginfo(bool debug, int font_tex)
{
#define DBG_UPDATE_INTERVAL 0.025f
	static float dbg_update_timer = DBG_UPDATE_INTERVAL;
	static char dbg_buffer[1024];
	static int4 dbg_draw_region;

	const static char *gl_version, *gl_vendor;
	if(!gl_version) {
		gl_version = glGetString(GL_VERSION);
		gl_vendor = glGetString(GL_VENDOR);
	}

	if(debug) {
		if(dbg_update_timer > DBG_UPDATE_INTERVAL) {
			int len = snprintf(dbg_buffer, 1024, "OpenGL Version: %s\nOpenGL Vendor: %s\nVSync: %s\nWindow: %ix%i %i %i %i\n"
			                   "Mouse: %i %i %i %i %i %i %i %i\nTime: %.6fs %i/%i\n\nEntities: %i",
			                   gl_version,
			                   gl_vendor,
			                   (app.window.opengl.vsync ? "Enabled" : "Disabled"),
			                   app.window.size.x, app.window.size.y, app.window.pos.x, app.window.pos.y, app.window.focus,
			                   app.mouse.screen.x, app.mouse.screen.y, app.mouse.relative.x, app.mouse.relative.y, app.mouse.delta.x, app.mouse.delta.y, app.mouse.wheel, app.mouse.wheel_delta, app.time.delta_secs, app.time.ticks, app.time.delta_ticks, (int)buf_len(game_balls));
			dbg_draw_region = font_get_text_bounds(&font, 11, dbg_buffer, len);
			dbg_update_timer = 0.0f;
		}
		rgl_draw_colored_rect2d(RGL_TOPLEFT, vec2(6 + dbg_draw_region.left, 6 + dbg_draw_region.top), vec4(0.1f, 0.1f, 0.1f, .8f),
		                        dbg_draw_region.right + 6, dbg_draw_region.bottom + 6);
		rgl_draw_text2d(&font, font_tex, 11, RGL_LEFT_ALIGN, vec2(10, 10), vec4(1, 1, 1, 1), dbg_buffer);
	}
	dbg_update_timer += app.time.delta_secs;


}

#define HOTKEY (0xAFFF)
#define STARTING_BALLS 32

int main(int argc, const char *argv[])
{
	static int debug;
	app.window.opengl.vsync = true;

	if(!nbytes_init()) {
		fatal("init failed");
	};

	assert(nbytes_register_hotkey(HOTKEY, 'H', KEYMOD_SHIFT));
	opengl_setup();

	int ranges[] = {
		0x0020, 0x007F, 11, // Basic Latin
		0x0020, 0x007F, 22, // Basic Latin
		0x0020, 0x007F, 33, // Basic Latin
		0,
	};

	font = bake_font_ranges("Minecraftia-Regular.ttf", ranges, ARRAY_LEN(ranges), 1, 1, 1);
	int font_tex = rgl_create_linear_texture(GL_ALPHA, font.textures[0].width, font.textures[0].height, font.textures[0].pixels, GL_ALPHA);

	srand(time(0));
reset:
	for(int i = 0; i < STARTING_BALLS; i++) {
		int x = rand() % app.window.size.x;
		int y = rand() % app.window.size.y;
		float dirx = ((rand() % 512) - 255) / 256.f;
		float diry = ((rand() % 512) - 255) / 256.f;

		int s = rand() % 10 + 1;
		spawn_new_ball(&game_balls, x, y, dirx, diry, s, s);
	}

	float size = 12.f;
	float t = 0.f;
	float dt = 0.016f;

	float blendout = 1.0f;
	while(nbytes_update()) {
		for(int i = 0; i < app.num_events; i++) {
			Event e = app.events[i];
			if(e.type == EVENT_HOTKEY_PRESSED && e.hotkey.id == HOTKEY) {
				app.window.hidden = !app.window.hidden;
			}
		}

		if(app.keys['R'].pressed) {
			buf_clear(game_balls);
			goto reset;
		}

		if(app.keys[VK_ESCAPE].pressed) {
			app.quit = true;
		}

		if(app.keys['V'].pressed) {
			app.window.opengl.vsync = !app.window.opengl.vsync;
		}

		if(app.keys['M'].pressed) {
			DRAWMODE = !DRAWMODE;
		}

		size += app.mouse.wheel_delta;
		size = MAX(size, 1);
		t += app.time.delta_secs;
		if(app.keys[VK_LBUTTON].down) {
			if(t >= 0.01f) {
				if(app.mouse.relative.x > size && app.mouse.relative.x < app.window.size.x - size && app.mouse.relative.y > size && app.mouse.relative.y < app.window.size.y - size  && (SIGN(app.mouse.delta.x) || SIGN(app.mouse.delta.y))) {
					spawn_new_ball(&game_balls, app.mouse.relative.x, app.mouse.relative.y, app.mouse.delta.x, app.mouse.delta.y, size, size);
				}
				t = 0.f;
			}
		}

		begin_scene2d();

		update_balls(game_balls);

		if(app.keys['F'].pressed) { app.window.bordered_fullscreen = !app.window.bordered_fullscreen; }
		if(app.keys['D'].pressed) { debug = !debug; }
		draw_debuginfo(debug, font_tex);

		if(blendout > 0.f);
		{
			float midx = app.window.size.x / 2.f;
			float midy = app.window.size.y / 2.f;
			rgl_draw_text2d(&font, font_tex, 22, RGL_CENTERED, vec2(midx, midy - 33), vec4(1, 1, 0, blendout), "nBytes");
			rgl_draw_text2d(&font, font_tex, 33, RGL_CENTERED, vec2(midx, midy), vec4(.1f, .4f, .8f, blendout), "THE USELESS WINDOW");
			blendout -= 0.005f;
		}

		rgl_draw_filled_circle(vec2(app.mouse.relative.x, app.mouse.relative.y), vec4(1, 0, 1, .5f), size / 2);
		rgl_draw_circle(vec2(app.mouse.relative.x, app.mouse.relative.y), vec4(1, 0, 1, 1), size / 2);

		rgl_draw_text2d(&font, font_tex, 22, RGL_LEFT_ALIGN, vec2(10, app.window.size.y - 101), vec4(1, 1, 1, 1),
		                "CONTROLS");
		rgl_draw_colored_rect2d(RGL_TOPLEFT, vec2(8, app.window.size.y - 87), vec4(1, 1, 1, 1), 310, 1);
		rgl_draw_text2d(&font, font_tex, 11, RGL_LEFT_ALIGN, vec2(10, app.window.size.y - 81), vec4(1, 1, 1, 1),
		                "%-s  |  %-s\n"
		                "%-s  |  %-s\n"
		                "%-s  |  %-s\n"
		                "%-s  |  %-s\n"
		                "%-s  |  %-s\n"
		                "%-s  |  %-s\n"
		                "%-s  |  %-s",
		                "MOUSEWHEEL UP/DOWN", "Change size for new entities",
		                "LEFT MOUSE                        ", "Hold and move mouse to add new entities",
		                "V                                                 ", "Toggle VSYNC",
		                "F                                                 ", "Toggle Fullscreen",
		                "M                                                 ", "Toggle render mode(RECTS/CIRCLES)",
		                "D                                                 ", "Display debug info",
		                "R                                                 ", "Restart/Reset");
		#if 0
		"Change size for new entities  |     \n"

		#endif

		end_scene2d();
		nbytes_render_window();
	}

	return 0;
}