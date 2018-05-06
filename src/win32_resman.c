CRITICAL_SECTION rsm__section;
CONDITION_VARIABLE rsm__condvar;

void
rsm_op__gl_delete_texture(ResourceFile *resfile)
{
	glDeleteTextures(1, (GLuint *) &resfile->data.texture.gpu_handle);
}

bool
rsm_op__gl_create_texture(ResourceFile *resfile)
{
	GLenum internal_format = GL_RGBA8;
	GLenum resource_format = GL_RGBA;
	switch(resfile->data.texture.channels) {
		case 1: {
			resource_format = GL_ALPHA;
		} break;
		case 2: {
			resource_format = GL_RG;
		} break;
		case 3: {
			resource_format = GL_RGB;
		} break;
		case 4: {
			resource_format = GL_RGBA;
		} break;
	}

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, resfile->data.texture.width, resfile->data.texture.height, 0,
	             resource_format, GL_UNSIGNED_BYTE, resfile->data_ptr);

	GLuint error = glGetError();
	if(error != GL_NO_ERROR) {
		warn("opengl error 0x%04X in rsm_op__gl_create_texture. glTexImage2D failed.", error);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	resfile->data.texture.gpu_handle = tex;
	resfile->state = RES_STATE_GPU_CACHED;
	return true;
}

bool
rsm_op__fetch_data(ResourceFile *resfile, ResOp op)
{
	switch(resfile->type) {
		case RES_TYPE_IMAGE: {
			stbi_uc *data = stbi_load(resfile->filename, &resfile->data.texture.width, &resfile->data.texture.height,
			                          &resfile->data.texture.channels, 0);
			if(!data) { warn("Failed to load texture data from \"%s\"", resfile->filename); }
			resfile->state = RES_STATE_CPU_CACHED;
			resfile->data_ptr = data;
			return true;
		} break;
		case RES_TYPE_FONT: {
			if(!op.opt.font_gen.ranges) {
				fatal("Font generation requested. But no font generation information was specified!");
			}
			resfile->data.font.baked = bake_font_ranges(resfile->filename,
			                           op.opt.font_gen.ranges, op.opt.font_gen.range_count,
			                           op.opt.font_gen.oversample_h, op.opt.font_gen.oversample_v,
			                           op.opt.font_gen.padding);

			resfile->data_ptr = resfile->data.font.baked.textures[0].pixels;
			resfile->data.texture.width = resfile->data.font.baked.textures[0].width;
			resfile->data.texture.height = resfile->data.font.baked.textures[0].height;
			resfile->data.texture.channels = 1;
			resfile->state = RES_STATE_CPU_CACHED;
			return true;
		} break;
	}
	return false;
}

bool
rsm_op__free_data(ResourceFile *resfile, ResOp op)
{
	switch(resfile->type) {
		case RES_TYPE_IMAGE: {
			stbi_image_free(resfile->data_ptr);
			resfile->data_ptr = 0;
			return true;
		} break;
		case RES_TYPE_FONT: {
			//xfree(resfile->data_ptr);
			//resfile->data_ptr = 0;
			return true;
		} break;
	}
	return false;

}

DWORD WINAPI
rsm_op__worker_thread(LPVOID lpParameter)
{
	for(;;) {
		EnterCriticalSection(&rsm__section);
		SleepConditionVariableCS(&rsm__condvar, &rsm__section, INFINITE);

		for(int i = 0; i < app.rsm.resop_count; i++) {
			ResOp *op = app.rsm.resops + i;
			ResourceFile *resfile = app.rsm.files + op->res_id;
			switch(op->res_type) {
				case RES_OPTYPE_FETCH_DATA: {
					rsm_op__fetch_data(resfile, *op);
				} break;
				case RES_OPTYPE_FREE_DATA: {
					rsm_op__free_data(resfile, *op);
				} break;
			}
			op->res_type = RES_OPTYPE_NOOP;
		}

		app.rsm.resop_count = 0;
		LeaveCriticalSection(&rsm__section);
	}
	return 0;
}


void
rsm__add_resop(ResOp op)
{
	EnterCriticalSection(&rsm__section);
	for(int i = 0; i < RESMAN_TEXOPS_MAX_NUM; i++) {
		if(app.rsm.resops[i].res_type == op.res_type && app.rsm.resops[i].res_id == op.res_id) {
			// Already in queue!
			warn("Resource operation already in queue.");
			break;
		}

		if(app.rsm.resops[i].res_type == RES_OPTYPE_NOOP) {
			app.rsm.resops[i] = op;
			app.rsm.resop_count++;
			break;
		}
	}
	LeaveCriticalSection(&rsm__section);
}

bool
rsm__is_image(const char *extension)
{
	return (strncmp(extension, ".png", 4) == 0 ||
	        strncmp(extension, ".jpg", 4) == 0 ||
	        strncmp(extension, ".bmp", 4) == 0 );
}

bool
rsm__is_audio(const char *extension)
{
	return (strncmp(extension, ".wav", 4) == 0 ||
	        strncmp(extension, ".mp4", 4) == 0 );
}

bool
rsm__is_font(const char *extension)
{
	return (strncmp(extension, ".ttf", 4)  == 0);
}

void
rsm__free_unused_data()
{
	for(int i = 0; i < buf_len(app.rsm.files); i++) {
		ResourceFile *resfile = app.rsm.files + i;
		if(resfile->state == RES_STATE_GPU_CACHED || resfile->state == RES_STATE_CPU_CACHED) {
			int64_t last_used_delta = app.time.secs - resfile->last_used;
			if(last_used_delta >= RESMAN_CLEANUP_THRESHOLD_SEC) {
				// If the resource was not used for two minutes we free all cached data connected to this resource!
				switch(resfile->type) {
					case RES_TYPE_FONT:
					case RES_TYPE_IMAGE: {
						if(resfile->state == RES_STATE_GPU_CACHED) {
							if(resfile->data.texture.gpu_handle != RESMAN_TEXHANDLE_INVALID) {
								EnterCriticalSection(&rsm__section);
								rsm_op__gl_delete_texture(resfile);
								LeaveCriticalSection(&rsm__section);
								warn("\"%s\" gpu cached data freed.", resfile->filename);
							}
						} else if (resfile->state == RES_STATE_CPU_CACHED) {
							ResOp op = { RES_OPTYPE_FREE_DATA, resfile->id };
							rsm__add_resop(op);
						}
						resfile->state = RES_STATE_UNINITIALIZED;
					} break;
				}
			}
		}
	}
}

void
rsm__check_modified_files()
{
	EnterCriticalSection(&rsm__section);
	if(win32__ionotify_wait(0)) {
		for(int i = 0; i < buf_len(app.rsm.files); i++) {
			ResourceFile *resfile = app.rsm.files + i;
			if(resfile->was_modified) {
				switch(resfile->state) {
					case RES_STATE_GPU_CACHED: {
						resfile->state = RES_STATE_UNINITIALIZED;

						ResOp op = {RES_OPTYPE_FETCH_DATA, resfile->id};
						rsm__add_resop(op);
					}
				}
				resfile->was_modified = false;
			}
		}
	}
	LeaveCriticalSection(&rsm__section);
}

int
nbytes_get_texture_handle(int res_id)
{
	assert(res_id < buf_len(app.rsm.files));

	ResourceFile *res = app.rsm.files + res_id;
	switch(res->type) {
		case RES_TYPE_FONT:
		case RES_TYPE_IMAGE: {
			if(res->state == RES_STATE_GPU_CACHED) {
				res->last_used = app.time.secs;
				return res->data.texture.gpu_handle;
			}

			switch(res->state) {
				case RES_STATE_UNINITIALIZED: {
					if(res->type == RES_TYPE_IMAGE) {
						ResOp op = {RES_OPTYPE_FETCH_DATA, res->id};
						rsm__add_resop(op);
					}
					return 0;
				} break;
				case RES_STATE_CPU_CACHED: {
					EnterCriticalSection(&rsm__section);
					if(res->data.texture.gpu_handle != RESMAN_TEXHANDLE_INVALID) { rsm_op__gl_delete_texture(res); }
					rsm_op__gl_create_texture(res);
					LeaveCriticalSection(&rsm__section);

					ResOp op = {RES_OPTYPE_FREE_DATA, res->id};
					rsm__add_resop(op);
					return res->data.texture.gpu_handle;
				} break;
				default: {
					return 0;
				} break;
			}
		} break;

	}
	return 0;
}

ResourceFile *
nbytes_get_resource(int res_id)
{
	assert(res_id < buf_len(app.rsm.files));
	return app.rsm.files + res_id;
}

bool
nbytes_gen_font(int res_id, int *ranges, int range_count, int oversample_h, int oversample_v, int padding)
{
	ResourceFile *resfile = nbytes_get_resource(res_id);
	if(resfile->type == RES_TYPE_FONT) {
		ResOp op = { RES_OPTYPE_FETCH_DATA, res_id, {ranges, range_count, oversample_h, oversample_v, padding} };
		rsm__add_resop(op);
		return true;
	}
	return false;
}

int
nbytes_get_resource_id(const char *path)
{
	int id = -1;
	for(int i = 0; i < buf_len(app.rsm.files); i++) {
		if(strstr(app.rsm.files[i].filename, path)) {
			return i;
		}
	}
	return id;
}


void
nbytes_update_rsm()
{
	/*  Check every second if the platform reports file changes
		in the root_dir. If so,
	*/
	static float next_update;
	next_update += app.time.delta_secs;
	if(next_update >= 1.0f) {
		rsm__check_modified_files();
		next_update = 0;
	}

	/*  Check every 60 seconds what resources are not used anymore
		and perform the required operations to reduce the memory footprint
		of the application
	*/
	static float cleanup_timer;
	cleanup_timer += app.time.delta_secs;
	if(cleanup_timer >= RESMAN_CLEANUP_INTERVAL_SEC) {
		rsm__free_unused_data();
		cleanup_timer = 0.0f;
	}

	if(app.rsm.resop_count != 0) {
		// Let the worker thread deal with the texture operations
		WakeConditionVariable(&rsm__condvar);
	}
}

bool
nbytes_init_rsm()
{
	InitializeConditionVariable(&rsm__condvar);
	InitializeCriticalSection(&rsm__section);

	app.rsm.root_dir = ".";
	win32__readdir(&app.rsm.files, app.rsm.root_dir);
	win32__ionotify_register(app.rsm.root_dir);

	for(int i = 0; i < buf_len(app.rsm.files); i++) {
		ResourceFile *resfile = app.rsm.files + i;

		if(0) {}
		else if(rsm__is_image(resfile->extension)) {
			resfile->type = RES_TYPE_IMAGE;
			resfile->data.texture.gpu_handle = RESMAN_TEXHANDLE_INVALID;
		} else if(rsm__is_audio(resfile->extension)) {
			resfile->type = RES_TYPE_AUDIO;
		} else if(rsm__is_font(resfile->extension)) {
			resfile->type = RES_TYPE_FONT;
			resfile->data.texture.gpu_handle = RESMAN_TEXHANDLE_INVALID;
		}
	}

	CreateThread(0, 0, rsm_op__worker_thread, 0, 0, 0);

	nbytes_update_rsm();
	return true;
}