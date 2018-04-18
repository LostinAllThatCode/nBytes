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

static bool PAUSED;

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
	//new_ball.vel = vec2(dirx * (rand() % 20 + 1), diry * (rand() % 20 + 1));
	new_ball.vel = vec2(abs(dirx) * (rand() % 20 + 1), abs(diry) * (rand() % 20 + 1));
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

		rgl_draw_colored_rect2d(RGL_CENTER, ball->pos, ball->clr, ball->size.x, ball->size.y);
	}
}

#define STARTING_BALLS 32

int main(int argc, const char *argv[])
{
	static int debug;
	app.window.opengl.vsync = true;

	if(!nbytes_init()) {
		fatal("init failed");
	};

	opengl_setup();

	int ranges[] = {
		0x0020, 0x007F, 11, // Basic Latin
		0x0020, 0x007F, 22, // Basic Latin
		0x0020, 0x007F, 33, // Basic Latin
		0,
	};

	BakedFont font1 = bake_font_ranges("Minecraftia-Regular.ttf", ranges, ARRAY_LEN(ranges), 1, 1, 1);
	int font_tex = rgl_create_linear_texture(GL_ALPHA, font1.textures[0].width, font1.textures[0].height, font1.textures[0].pixels, GL_ALPHA);

	srand(time(0));

reset:
	for(int i = 0; i < STARTING_BALLS; i++) {
		int x = rand() % app.window.size.x;
		int y = rand() % app.window.size.y;
		int dirx = rand() % 1 - 1;
		int diry = rand() % 1 - 1;

		spawn_new_ball(&game_balls, x, y, dirx, diry, 3, 3);
	}

	const char *gl_version = glGetString(GL_VERSION);
	const char *gl_vendor = glGetString(GL_VENDOR);

	float size = 12.f;
	float t = 0.f;
	float dt = 0.016f;

	float blendout = 1.0f;
	while(nbytes_update()) {
		t += app.time.delta_secs;
		size += app.mouse.wheel_delta;
		size = MAX(size, 3);
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

		if(app.keys[VK_LBUTTON].down) {
			if(t >= 0.0032f) {
				if(app.mouse.rel_x > size && app.mouse.rel_x < app.window.size.x - size && app.mouse.rel_y > size && app.mouse.rel_y < app.window.size.y - size  && (SIGN(app.mouse.dx) || SIGN(app.mouse.dy))) {
					spawn_new_ball(&game_balls, app.mouse.rel_x, app.mouse.rel_y, app.mouse.dx, app.mouse.dy, size, size);
				}
				t = 0.f;
			}
		}

		begin_scene2d();

		update_balls(game_balls);


		if(app.keys['D'].pressed) { debug = !debug; }
		if(debug) {
			rgl_draw_text2d(&font1, font_tex, 11, RGL_LEFT_ALIGN, vec2(21, 21), vec4(.4f, .4f, 0.4f, 1),
			                "OpenGL Version: %s\nOpenGL Vendor: %s\nVSync: %s\nWindow: %ix%i %i %i %i\n"
			                "Mouse: %i %i %i %i %i %i %i %i\nTime: %.6f ms\n\nEntities: %i",
			                gl_version,
			                gl_vendor,
			                (app.window.opengl.vsync ? "Enabled" : "Disabled"),
			                app.window.size.x, app.window.size.y, app.window.pos.x, app.window.pos.y, app.window.focus,
			                app.mouse.x, app.mouse.y, app.mouse.rel_x, app.mouse.rel_y, app.mouse.dx, app.mouse.dy, app.mouse.wheel, app.mouse.wheel_delta, app.time.delta_secs, buf_len(game_balls));
			rgl_draw_text2d(&font1, font_tex, 11, RGL_LEFT_ALIGN, vec2(20, 20), vec4(1, 1, 1, 1),
			                "OpenGL Version: %s\nOpenGL Vendor: %s\nVSync: %s\nWindow: %ix%i %i %i %i\n"
			                "Mouse: %i %i %i %i %i %i %i %i\nTime: %.6f ms\n\nEntities: %i",
			                gl_version,
			                gl_vendor,
			                (app.window.opengl.vsync ? "Enabled" : "Disabled"),
			                app.window.size.x, app.window.size.y, app.window.pos.x, app.window.pos.y, app.window.focus,
			                app.mouse.x, app.mouse.y, app.mouse.rel_x, app.mouse.rel_y, app.mouse.dx, app.mouse.dy, app.mouse.wheel, app.mouse.wheel_delta, app.time.delta_secs, buf_len(game_balls));
		}

		float midx = app.window.size.x / 2.f;
		float midy = app.window.size.y / 2.f;
		blendout -= 0.005f;
		rgl_draw_text2d(&font1, font_tex, 22, RGL_CENTERED, vec2(midx, midy - 33), vec4(1, 1, 0, blendout), "nBytes");
		rgl_draw_text2d(&font1, font_tex, 33, RGL_CENTERED, vec2(midx, midy), vec4(.1f, .4f, .8f, blendout), "THE USELESS WINDOW");

		rgl_draw_text2d(&font1, font_tex, 11, RGL_RIGHT_ALIGN, vec2(app.window.size.x - 10, app.window.size.y - 51), vec4(1, 1, 1, 1), "Change size for new entities  |     MOUSEWHEEL UP/DOWN");
		rgl_draw_text2d(&font1, font_tex, 11, RGL_RIGHT_ALIGN, vec2(app.window.size.x - 10, app.window.size.y - 41), vec4(1, 1, 1, 1), "Hold to add new entities  |                             LEFT MOUSE");
		rgl_draw_text2d(&font1, font_tex, 11, RGL_RIGHT_ALIGN, vec2(app.window.size.x - 10, app.window.size.y - 31), vec4(1, 1, 1, 1), "Toggle VSYNC   |                                                      V");
		rgl_draw_text2d(&font1, font_tex, 11, RGL_RIGHT_ALIGN, vec2(app.window.size.x - 10, app.window.size.y - 21), vec4(1, 1, 1, 1), "Display debug info  |                                                      D");
		rgl_draw_text2d(&font1, font_tex, 11, RGL_RIGHT_ALIGN, vec2(app.window.size.x - 10, app.window.size.y - 10), vec4(1, 1, 1, 1), "Restart/Reset  |                                                      R");
		end_scene2d();
		nbytes_render_window();
	}

	return 0;
}