/**

Oldschool opengl helpers to draw simple stuff
Only works with old opengl versions < 3.0 or compatibility profiles

**/
struct RetroGLState {
	int a;
};

int rgl__render_mode;

typedef enum RetroGLAnchor {
	RGL_CENTER,
	RGL_TOPLEFT,
} RetroGLAnchor;

typedef enum RetroGLTextAlign {
	RGL_LEFT_ALIGN,
	RGL_CENTERED,
	RGL_RIGHT_ALIGN,
} RetroGLTextAlign;

int
rgl_create_linear_texture(int input_format, int width, int height, uint8_t *data, int gl_format)
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, gl_format, width, height, 0, input_format, GL_UNSIGNED_BYTE, data);
	assert(glGetError() != GL_INVALID_VALUE);

	glBindTexture(GL_TEXTURE_2D, 0);
	return tex;
}

void
rgl_draw_point(v2 P, v4 color, float radius_px)
{
	glPointSize(radius_px);
	glColor4f(color.r, color.g, color.b, color.a);
	glBegin(GL_POINTS);
	glVertex2f(P.x, P.y);
	glEnd();
	glPointSize(1);
}

void
rgl_draw_line2d(v2 p0, v2 p1, v4 color, float thickness)
{
	glLineWidth(thickness);
	glColor4f(color.r, color.g, color.b, color.a);
	glBegin(GL_LINES);
	glVertex2f(p0.x, p0.y);
	glVertex2f(p1.x, p1.y);
	glEnd();
	glLineWidth(1);
}

void
rgl_draw_rect2d(RetroGLAnchor origin, v2 position, v4 color, float width, float height)
{
	v2 p0, p1, p2, p3;
	switch(origin) {
		case RGL_TOPLEFT: {
			p0 = position;
			p1 = vec2(position.x, position.y + height);
			p2 = vec2(position.x + width, position.y + height);
			p3 = vec2(position.x + width, position.y);
		} break;
		case RGL_CENTER: {
			float hw = width / 2.f;
			float hh = height / 2.f;
			p0 = vec2(position.x - hw, position.y - hh);
			p1 = vec2(position.x - hw, position.y + hh);
			p2 = vec2(position.x + hw, position.y + hh);
			p3 = vec2(position.x + hw, position.y - hh);
		}
	}

	glBegin(GL_LINE_LOOP);
	{
		glColor4f(color.r, color.g, color.b, color.a);
		glVertex2f(p0.x, p0.y);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
		glVertex2f(p3.x, p3.y);
	}
	glEnd();
}

void
rgl_draw_filled_rect2d(RetroGLAnchor origin, v2 position, v4 color, float width, float height)
{
	v2 p0, p1, p2, p3;
	switch(origin) {
		case RGL_TOPLEFT: {
			p0 = position;
			p1 = vec2(position.x, position.y + height);
			p2 = vec2(position.x + width, position.y + height);
			p3 = vec2(position.x + width, position.y);
		} break;
		case RGL_CENTER: {
			float hw = width / 2.f;
			float hh = height / 2.f;
			p0 = vec2(position.x - hw, position.y - hh);
			p1 = vec2(position.x - hw, position.y + hh);
			p2 = vec2(position.x + hw, position.y + hh);
			p3 = vec2(position.x + hw, position.y - hh);
		}
	}

	glBegin(GL_QUADS);
	{
		glColor4f(color.r, color.g, color.b, color.a);
		glVertex2f(p0.x, p0.y);
		glVertex2f(p1.x, p1.y);
		glVertex2f(p2.x, p2.y);
		glVertex2f(p3.x, p3.y);
	}
	glEnd();
}

void
rgl_draw_textured_rect2d(int texture, RetroGLAnchor origin, v2 position, v4 color, float width, float height)
{
	v2 p0, p1, p2, p3;
	switch(origin) {
		case RGL_TOPLEFT: {
			p0 = position;
			p1 = vec2(position.x, position.y + height);
			p2 = vec2(position.x + width, position.y + height);
			p3 = vec2(position.x + width, position.y);
		} break;
		case RGL_CENTER: {
			float hw = width / 2.f;
			float hh = height / 2.f;
			p0 = vec2(position.x - hw, position.y - hh);
			p1 = vec2(position.x - hw, position.y + hh);
			p2 = vec2(position.x + hw, position.y + hh);
			p3 = vec2(position.x + hw, position.y - hh);
		}
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	{
		glColor4f(1, 1, 1, 1);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(p0.x, p0.y);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(p1.x, p1.y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(p2.x, p2.y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(p3.x, p3.y);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void
rgl_draw_filled_circle(v2 position, v4 color, float radius)
{
	int subdiv = MAX(radius * 4, 64);
	//glBegin(GL_LINE_LOOP);
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(color.r, color.g, color.b, color.a);
	glVertex2d(position.x, position.y);
	for(int i = 0; i <= subdiv; i++) {
		double angle = PI2 * i / subdiv;
		double x = position.x + radius *  cos(angle);
		double y = position.y + radius * sin(angle);
		glVertex2d(x, y);
	}
	glEnd();
}

void
rgl_draw_circle(v2 position, v4 color, float radius)
{
	int subdiv = MAX(radius * 4, 64);
	if(radius <= 1.0f) {
		rgl_draw_rect2d(RGL_CENTER, position, color, radius * 2, radius * 2);
		return;
	}

	glBegin(GL_LINE_LOOP);
	glColor4f(color.r, color.g, color.b, color.a);
	for(int i = 0; i <= subdiv; i++) {
		double angle = PI2 * i / subdiv;
		double x = position.x + radius *  cos(angle);
		double y = position.y + radius * sin(angle);
		glVertex2d(x, y);
	}

	glEnd();
}


void
rgl_begin_text2d()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_COLOR, GL_ZERO);


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, app.window.size.width, app.window.size.height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void
rgl_end_text2d()
{
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void
rgl_draw_text2d(BakedFont *font, int font_tex, int size, RetroGLTextAlign align, v2 position, v4 color, const char *fmt, ...)
{
	char text[4096];

	va_list args;
	va_start(args, fmt);
	int len = vsprintf(text, fmt, args);
	va_end(args);

	switch(align) {
		case RGL_CENTERED: {
			int4 rect = font_get_text_bounds(font, size, text, len);
			position.x -= rect.right / 2.0f;
			position.y -= rect.bottom / 2.0f;
		} break;
		case RGL_RIGHT_ALIGN: {
			int4 rect = font_get_text_bounds(font, size, text, len);
			position.x -= rect.right;
			position.y -= rect.bottom;
		} break;
	}


	glBindTexture(GL_TEXTURE_2D, font_tex);
	glBegin(GL_TRIANGLES);
	float x 	= position.x;
	float y 	= position.y;
	int adv_csr = 0;
	for(int i = 0; i < len;) {
		if(text[i] == '\n') {
			y += size;
			x = position.x;
			i++;
			continue;
		}



		int unicode = str_utf8_to_unicode(text + i, &adv_csr);
		stbtt_packedchar *character_data = (stbtt_packedchar *) map_get(&font->glyphs, (void *) PACK_GLYPH(size, unicode));
		if(character_data) {
			stbtt_aligned_quad q = {0};
			stbtt_GetPackedQuad(character_data, font->textures[0].width, font->textures[0].height, 0, &x, &y, &q, 0);

			float x0 = q.x0;
			float x1 = q.x1;
			float y0 = q.y0 + size;
			float y1 = q.y1 + size;

			glColor4f(color.r, color.g, color.b, color.a);

			glTexCoord2f(q.s0, q.t0);
			glVertex2f(x0, y0);

			glTexCoord2f(q.s0, q.t1);
			glVertex2f(x0, y1);

			glTexCoord2f(q.s1, q.t0);
			glVertex2f(x1, y0);

			glTexCoord2f(q.s1, q.t0);
			glVertex2f(x1, y0);

			glTexCoord2f(q.s0, q.t1);
			glVertex2f(x0, y1);

			glTexCoord2f(q.s1, q.t1);
			glVertex2f(x1, y1);

		} else {
			//warn("Glyph lookup failed for character '%.*s' ( unicode = 0x%04X )\n", adv_csr, cursor, unicode);
		}
		i += adv_csr;
	}
	glEnd();

}