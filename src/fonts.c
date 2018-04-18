// not even close to final.. still figuring out what the best way is to store glyph data!!

/**

	const char text[] = "Ã‚Â¶ helloÃ„Â¦world! Ã‚Â¶";

	int result 	= 0;
	r32 x 		= 0.f;
	r32 y 		= 0.f;

	int adv_csr = 0;
	unsigned char* cursor = (unsigned char*) text;
	while(*cursor)
	{
		int unicode = utf8_decode(cursor, &adv_csr);
		stbtt_packedchar character_data = glyph_lookup_get(&f->glyphs, unicode, &result);
		if(result == HT_SUCCESS)
		{
			stbtt_aligned_quad q2 = {0};
			stbtt_GetPackedQuad(&character_data, 512, 512, 0, &x, &y, &q2, 0);
			int b = 1;
		}
		else
		{
			LIBACX_LOGW("Glyph lookup failed for character '%.*s' ( unicode = 0x%04X )\n", adv_csr, cursor, unicode);
		}
		cursor += adv_csr;
	}

*/


#if 0
int ranges[] = {
	0x0020, 0x007F, // Basic Latin
	//0x00A0, 0x00FF, // Latin-1 Supplement
	//0x0100, 0x017F, // Latin Extended-A
	//0x0180, 0x024F, // Latin Extended-B
	//0x0250, 0x02AF, // IPA Extensions
	//0x02B0, 0x02FF, // Spacing Modifier Letters
	//0x0300, 0x036F, // Combining Diacritical Marks
	//0x20A0, 0x20CF, // Currency Symbols
	0,
};
#endif

#define BAKED_FONT_MAX_TEXSIZE 		4096
#define BAKED_FONT_MAGIC 			0x77FBAC
typedef struct FontTex {
	int width;
	int height;
	uint8_t	*pixels;
} FontTex;

typedef struct BakedFont {
	Map glyphs;
	FontTex *textures;
} BakedFont;

#define 		PACK_GLYPH(size, codepoint) ((uint64_t) (size) << 32 | codepoint)

int 			str_utf8_to_unicode(const char *str, int *byte_length);
BakedFont 		create_baked_font(const char *fontfile, int tex_width, int tex_height, int font_height, int *ranges, int range_count, int h_over, int v_over, int padding);

void 			write_baked_font_to_file(BakedFont font, const char *outfile);
int 			load_baked_font_from_memory(void *data, int size, BakedFont *out_font);
int 			load_baked_font_from_file(const char *binaryfile, BakedFont *out_font);

int
str_utf8_to_unicode(const char *str, int *byte_length)
{
	const uint8_t *s = (const uint8_t *)str; // Use unsigned chars
	int u = *s, l = 1;
	if (ISUNICODE(u)) {
		int a = (u & 0x20) ? ((u & 0x10) ? ((u & 0x08) ? ((u & 0x04) ? 6 : 5) : 4) : 3) : 2;
		if (a < 6 || !(u & 0x02)) {
			int b = 0;
			u = ((u << (a + 1)) & 0xff) >> (a + 1);
			for (b = 1; b < a; ++b) {
				u = (u << 6) | (s[l++] & 0x3f);
			}
		}
	}
	if (byte_length) { *byte_length = l; }
	return u;
}

Rect
font_get_text_bounds(BakedFont *font, int size, const char *text, size_t len)
{
	float dy = 0;
	float dx = 0;
	Rect result = {0, 0, 0, size};

	for(int i = 0; i < len; i++) {
		if(text[i] == '\n') {
			result.height += size;
			dx = 0;
			i++;
			continue;
		}

		int unicode = str_utf8_to_unicode(text + i, 0);
		stbtt_packedchar *character_data = map_get(&font->glyphs, (void *) PACK_GLYPH(size, unicode));
		if(character_data) {
			stbtt_aligned_quad q = {0};
			stbtt_GetPackedQuad(character_data, font->textures[0].width, font->textures[0].height, 0, &dx, &dy, &q, 0);

			if(result.width < dx) {
				result.width = dx;
			}
		}
	}
	return result;
}



BakedFont
bake_font_ranges(const char *font, int *range_array, int len, int h_over, int v_over, int padding)
{
	char *data = read_file(font, 0);
	assert(data);

	/** make a stbtt_pack_range array of all given ranges **/
	stbtt_fontinfo info  = {0};
	stbtt_InitFont(&info, (unsigned char *) data, 0);
	stbtt_pack_range *rbuffer = 0;
	for ( int i = 0; i < len; i += 3 ) {
		if(range_array[i] == 0) { break; }
		int start 				= range_array[i];
		int end 				= range_array[i + 1];
		int fontsize 			= range_array[i + 2];
		int cdata_size 			= (end - start) * sizeof(stbtt_packedchar);

		stbtt_pack_range range 					= { 0 };
		range.font_size 						= fontsize;
		range.first_unicode_codepoint_in_range 	= start;
		range.num_chars 						= end - start;
		range.chardata_for_range 				= (stbtt_packedchar *) xmalloc(cdata_size);
		buf_push(rbuffer, range);
	}

	/** pack ranges into texture **/

	FontTex tex = { 64, 64 };
	int pack_success = 0;
	while(!pack_success) {
		assert(tex.width <= 4096 && tex.height <= 4096);
		tex.pixels = (uint8_t *) xmalloc(tex.width * tex.height);
		assert(tex.pixels);

		stbtt_pack_context context = {0};
		stbtt_PackBegin(&context, tex.pixels, tex.width, tex.height, 0, MAX(padding, 1), 0);
		stbtt_PackSetOversampling(&context, (MAX(h_over, 1)), MAX(v_over, 1));
		pack_success = stbtt_PackFontRanges(&context, info.data, 0, rbuffer, buf_len(rbuffer));
		if(!pack_success) {
			tex.width <<= 1;
			tex.height <<= 1;
			xfree(tex.pixels);
		}
		stbtt_PackEnd(&context);
	}

	Map glyphs = {0};
	int c = 0;
	for ( int i = 0; i < len; i += 3 ) {
		if(range_array[i] == 0) { break; }
		stbtt_packedchar *cdata = rbuffer[c].chardata_for_range;
		int start 				= range_array[i];
		int end 				= range_array[i + 1];
		int fontsize 			= range_array[i + 2];
		for ( int j = 0; j < end - start; j++ ) {
			int codepoint = start + j;
			int idx = stbtt_FindGlyphIndex(&info, codepoint);
			uint64_t encoded_glyph = PACK_GLYPH(fontsize, codepoint);
			map_put(&glyphs, (void *) encoded_glyph, &cdata[j]);
		}
		//xfree(ranges_mem[c].chardata_for_range);
		++c;
	}

	buf_free(rbuffer);
	FontTex *textures = 0;
	buf_push(textures, tex);
	BakedFont result = { glyphs, textures };
	return result;
}
#if 0
BakedFont
create_baked_font(const char *fontfile,
                  int tex_width, int tex_height, int font_height,
                  int *ranges, int range_count,
                  int h_over, int v_over, int padding)
{
	BakedFont font 	= { 0 };
	font.height_px = font_height;
	font.tex_width 	= tex_width;
	font.tex_height = tex_height;

	char *data = read_file(fontfile, 0);
	assert(data);

	stbtt_fontinfo info  = {0 };
	stbtt_InitFont(&info, (unsigned char *) data, 0);

	stbtt_pack_range *ranges_mem = 0;
	for ( int i = 0; i < range_count; i += 2 ) {
		if(ranges[i] == 0) {
			break;
		}
		int start 				= ranges[i];
		int end 				= ranges[i + 1];
		int cdata_size 			= (end - start) * sizeof(stbtt_packedchar);
		printf("Preparing font range 0x%08X - 0x%08X.\n", start, end);

		stbtt_pack_range range 					= { 0 };
		range.font_size 						= font_height;
		range.first_unicode_codepoint_in_range 	= start;
		range.num_chars 						= end - start;
		range.chardata_for_range 				= (stbtt_packedchar *) xmalloc(cdata_size);
		buf_push(ranges_mem, range);
	}

	printf("%s\n", "Packing font ranges into texture.");

	int pack_success = 0;
	while(!pack_success) {
		if(font.tex_width >= BAKED_FONT_MAX_TEXSIZE ) {
			break;
		}

		font.tex_data   = xmalloc( font.tex_width * font.tex_height);
		if(!font.tex_data) {
			printf("Failed to allocate texture memory. Requestes memory size: %u\n", tex_width * tex_height);
		}

		stbtt_pack_context context = {0 };
		stbtt_PackBegin(&context, (unsigned char *) font.tex_data, font.tex_width, font.tex_height, 0, MAX(padding, 1), 0);
		stbtt_PackSetOversampling(&context, (MAX(h_over, 1)), MAX(v_over, 1));

		pack_success = stbtt_PackFontRanges(&context, info.data, 0, ranges_mem, buf_len(ranges_mem));
		if(!pack_success) {
			font.tex_width 	= font.tex_width 	<< 1;
			font.tex_height = font.tex_height 	<< 1;
			xfree(font.tex_data);
		}
		stbtt_PackEnd(&context);
	}

	if(!pack_success) {
		printf("Failed to pack data into texture. %ix%i is too small.\n", tex_width, tex_height);
	}

	printf("%s\n", "Generating glyph lookup table.");
	int c = 0;
	for ( int i = 0; i < range_count; i += 2 ) {
		if(ranges[i] == 0) {
			break;
		}
		stbtt_packedchar *cdata = ranges_mem[c].chardata_for_range;
		int start 				= ranges[i];
		int end 				= ranges[i + 1];
		for ( int j = 0; j < end - start; j++ ) {
			int idx = stbtt_FindGlyphIndex(&info, start + j);
			if(idx) {
				map_put(&font.glyphs, (void *) (uintptr_t)(start + j), &cdata[j]);
			}
		}
		//xfree(ranges_mem[c].chardata_for_range);
		++c;
	}

	buf_free(ranges_mem);

	#if 0
	const char text[] = "Ã‚Â¶ helloÃ„Â¦world! Ã‚Â¶";

	int result 	= 0;
	r32 x 		= 0.f;
	r32 y 		= 0.f;

	int adv_csr = 0;
	unsigned char *cursor = (unsigned char *) text;
	while(*cursor) {
		int unicode = utf8_decode(cursor, &adv_csr);
		stbtt_packedchar character_data = glyph_lookup_get(&font.glyphs, unicode, &result);
		if(result == HT_SUCCESS) {
			stbtt_aligned_quad q2 = {0 };
			stbtt_GetPackedQuad(&character_data, 512, 512, 0, &x, &y, &q2, 0);
			int b = 1;
		} else {
			LIBACX_LOGW("Glyph lookup failed for character '%.*s' ( unicode = 0x%04X )\n", adv_csr, cursor, unicode);
		}
		cursor += adv_csr;
	}
	#endif

	if(tex_width != font.tex_width) {
		printf("Texture size automatically resized. Desired textures size of %ix%i pixels was not sufficient. Generated texture size is %ix%i pixels.\n",
		       tex_width, tex_height, font.tex_width, font.tex_height);
	}

	xfree(data);
	return font;
}

void
write_baked_font(BakedFont font, const uint8_t *outfile)
{
	assert(font.glyphs.len != 0);

	FILE *fh = fopen(outfile, "wb");
	assert(fh);

	int magic = BAKED_FONT_MAGIC;
	fwrite(&magic, 1, sizeof(int), fh);
	fwrite(&font, 1, sizeof(font), fh);
	fwrite(font.tex.pixels, 1, font.tex.width * font.tex.height, fh);
	for(int i = 0; i < font.glyphs.cap; i++) {
		if(!font.glyphs.keys[i]) { continue; }
		fwrite(&font.glyphs.keys[i], 1, sizeof(void *), fh);
	}
	for(int i = 0; i < font.glyphs.cap; i++) {
		if(!font.glyphs.keys[i]) { continue; }
		fwrite(font.glyphs.vals[i], 1, sizeof(stbtt_packedchar), fh);
	}
	fclose(fh);
}

bool
load_baked_font(void *data, int size, BakedFont *out)
{
	if((*(int *) data) == BAKED_FONT_MAGIC) {
		uint8_t *cursor = (uint8_t *) data;
		cursor += 4;

		BakedFont *f			= (BakedFont *) cursor;
		f->tex.pixels 			= (uint8_t *) (cursor + (sizeof(*f)));
		void **keys 			= (void **) ((uint8_t *)f->tex.pixels + f->tex.width * f->tex.height);
		stbtt_packedchar *vals 	= (stbtt_packedchar *) (keys + f->glyphs.len);

		Map new_map = {0};
		for(int i = 0; i < f->glyphs.len; i++) {
			map_put(&new_map, keys[i], &vals[i]);
		}

		f->glyphs = new_map;

		*out = *f;
		return true;
	}
	return false;
}
#endif
#if 0
int
bde_load_baked_font_from_file(const char *binaryfile, BakedFont *out_font)
{
	bdLoadedFile file;
	bd_load_file(&file, binaryfile);
	if(file.size == 0) {
		bd_free_file(&file);
		return false;
	}

	int result = bde_load_baked_font_from_memory(file.data, file.size, out_font);
	bd_free_file(&file);
	return result;
}
#endif



#if 0

void
mprint_blob(const void *base, u64 size)
{
	unsigned char *_base 			= (unsigned char *) base;
	int _size 			= size;
	int bytes_per_line  = (_size < 16 ? _size : 16);
	for(int i = 0; i * bytes_per_line < _size; i++) {
		for(int j = 0; j < bytes_per_line; j++) {
			unsigned char *byte = _base + (i * bytes_per_line) + j;
			if(byte >= bdm_ptradd(base, size)) {
				break;
			} else {
				unsigned char *byte = _base + (i * bytes_per_line) + j;
				printf("0x%02X, ", *byte);
			}
		}
		printf("\n");
	}
}

// 	print BakedFont structure as text!!
BakedFont copy;
copy = font;
// This will actually be the offset to where the texture data will end up!
copy.tex_data 				= (void *) 	((size_t) sizeof(copy));
copy.glyphs.storage 		= (unsigned char *)  	((size_t) sizeof(copy) + copy.tex_width *copy.tex_height);
copy.glyphs.collisions 		= 0;
copy.glyphs.mutex.handle	= 0;

int magic = BDE_BAKED_FONT_MAGIC;
printf("// MAGIC\n");
mprint_blob(&magic, sizeof(magic));
printf("// BAKEDFONT HEADER\n");
mprint_blob(&copy, sizeof(copy));
printf("// TEXTURE DATA\n");
mprint_blob(font.tex_data, font.tex_width *font.tex_height);
printf("// GLYPH LOOKUP DATA\n");
mprint_blob(font.glyphs.storage, sizeof(glyph_lookup_kvpair) * (font.glyphs.max + 1));
#endif