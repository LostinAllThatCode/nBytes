
#ifdef GLLOADER_USE_WGL
	#include <gl\gl.h>
	#define GLLOADER_GETPROC wglGetProcAddress

	#define GL_MAP_READ_BIT                   0x0001
	#define GL_MAP_WRITE_BIT                  0x0002
	#define GL_MAP_INVALIDATE_RANGE_BIT       0x0004
	#define GL_MAP_INVALIDATE_BUFFER_BIT      0x0008
	#define GL_MAP_FLUSH_EXPLICIT_BIT         0x0010
	#define GL_MAP_UNSYNCHRONIZED_BIT         0x0020

	#define GL_CLAMP_TO_BORDER                0x812D
	#define GL_CLAMP_TO_EDGE                  0x812F
	#define GL_NUM_EXTENSIONS                 0x821D
	#define GL_TEXTURE0                       0x84C0
	#define GL_ARRAY_BUFFER                   0x8892
	#define GL_ELEMENT_ARRAY_BUFFER           0x8893
	#define GL_STREAM_DRAW                    0x88E0
	#define GL_STREAM_READ                    0x88E1
	#define GL_STREAM_COPY                    0x88E2
	#define GL_STATIC_DRAW                    0x88E4
	#define GL_STATIC_READ                    0x88E5
	#define GL_STATIC_COPY                    0x88E6
	#define GL_DYNAMIC_DRAW                   0x88E8
	#define GL_DYNAMIC_READ                   0x88E9
	#define GL_DYNAMIC_COPY                   0x88EA
	#define GL_FRAGMENT_SHADER                0x8B30
	#define GL_VERTEX_SHADER                  0x8B31
	#define GL_COMPILE_STATUS                 0x8B81
	#define GL_LINK_STATUS                    0x8B82
	#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
	#define GL_TEXTURE_BUFFER                 0x8C2A

	#define GL_READ_FRAMEBUFFER               0x8CA8
	#define GL_DRAW_FRAMEBUFFER               0x8CA9
	#define GL_FRAMEBUFFER                    0x8D40
	#define GL_TEXTURE_2D_MULTISAMPLE         0x9100
	#define GL_COLOR_ATTACHMENT0              0x8CE0
	#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
	#define GL_MULTISAMPLE                    0x809D
	#define GL_RENDERBUFFER                   0x8D41
	#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E

	#define GL_BGRA                           0x80E1

	typedef char GLchar;
	typedef ptrdiff_t GLintptr;
	typedef ptrdiff_t GLsizeiptr;
	typedef unsigned int GLbitfield;
	typedef void (APIENTRY *GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

	typedef HGLRC (wglCreateContextAttribsARBProc)(HDC hDC, HGLRC hShareContext, const int *attribList); wglCreateContextAttribsARBProc *wglCreateContextAttribsARB;
	typedef char *(wglGetExtensionsStringARBProc)(HDC hdc); wglGetExtensionsStringARBProc *wglGetExtensionsStringARB;
	typedef int	(wglGetSwapIntervalEXTProc)(void); wglGetSwapIntervalEXTProc *wglGetSwapIntervalEXT;
	typedef BOOL (wglSwapIntervalEXTProc)(int interval); wglSwapIntervalEXTProc *wglSwapIntervalEXT;

#elif GLLOADER_USE_EGL

#else
	#error Failed to load opengl
#endif

// Globals
static Map opengl_loader__supported_extensions;

// Function definitions and declarations
typedef const GLubyte *glGetStringiProc(GLenum name, GLuint index); glGetStringiProc *glGetStringi;
typedef void glDebugMessageCallbackProc(GLDEBUGPROC callback, const void *user_param); glDebugMessageCallbackProc *glDebugMessageCallback;

typedef void glActiveTextureProc(GLenum texture); glActiveTextureProc *glActiveTexture;
typedef void glBindBufferProc(GLenum target, GLuint buffer); glBindBufferProc *glBindBuffer;
typedef void glDeleteBuffersProc(GLsizei n, const GLuint *buffers); glDeleteBuffersProc *glDeleteBuffers;
typedef void glGenBuffersProc(GLsizei n, GLuint *buffers); glGenBuffersProc *glGenBuffers;
typedef void glBufferDataProc(GLenum target, GLsizeiptr size, const void *data, GLenum usage); glBufferDataProc *glBufferData;
typedef void glBufferSubDataProc(GLenum target, GLintptr offset, GLsizeiptr size, const void *data); glBufferSubDataProc *glBufferSubData;
typedef void *glMapBufferRangeProc(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access); glMapBufferRangeProc *glMapBufferRange;
typedef void *glUnmapBufferProc(GLenum target); glUnmapBufferProc *glUnmapBuffer;

typedef void glAttachShaderProc(GLuint program, GLuint shader);	glAttachShaderProc *glAttachShader;
typedef void glCompileShaderProc(GLuint shader); glCompileShaderProc *glCompileShader;
typedef GLuint glCreateProgramProc(void); glCreateProgramProc *glCreateProgram;
typedef GLuint glCreateShaderProc(GLenum type);	glCreateShaderProc *glCreateShader;
typedef void glDeleteProgramProc(GLuint program); glDeleteProgramProc *glDeleteProgram;
typedef void glDeleteShaderProc(GLuint shader);	glDeleteShaderProc *glDeleteShader;
typedef void glDetachShaderProc(GLuint program, GLuint shader);	glDetachShaderProc *glDetachShader;
typedef void glGetProgramivProc(GLuint program, GLenum pname, GLint *params); glGetProgramivProc *glGetProgramiv;
typedef void glGetProgramInfoLogProc(GLuint program, GLsizei buffer_size, GLsizei *length, GLchar *source);	glGetProgramInfoLogProc *glGetProgramInfoLog;
typedef void glGetShaderInfoLogProc(GLuint shader, GLsizei buffer_size, GLsizei *length, GLchar *source); glGetShaderInfoLogProc *glGetShaderInfoLog;
typedef void glGetShaderSourceProc(GLuint shader, GLsizei buffer_size, GLsizei *length, GLchar *source); glGetShaderSourceProc *glGetShaderSource;
typedef void glGetShaderivProc(GLuint shader, GLenum pname, GLint *params);	glGetShaderivProc *glGetShaderiv;
typedef void glLinkProgramProc(GLuint program);	glLinkProgramProc *glLinkProgram;
typedef void glShaderSourceProc(GLuint shader, GLsizei count, const GLchar *const *string, const GLint *length); glShaderSourceProc *glShaderSource;
typedef void glUseProgramProc(GLuint program); glUseProgramProc *glUseProgram;
typedef void glValidateProgramProc(GLuint program); glValidateProgramProc *glValidateProgram;
typedef void glBindFragDataLocationProc(GLuint program, GLuint color_number, const char *name);	glBindFragDataLocationProc *glBindFragDataLocation;
typedef void glBindAttribLocationProc(GLuint program, GLuint index, const GLchar *name); glBindAttribLocationProc *glBindAttribLocation;
typedef GLint glGetAttribLocationProc(GLuint program, const GLchar *name); glGetAttribLocationProc *glGetAttribLocation;
typedef GLint glGetUniformLocationProc(GLuint program, const GLchar *name); glGetUniformLocationProc *glGetUniformLocation;
typedef void glUniformMatrix3fvProc(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); glUniformMatrix3fvProc *glUniformMatrix3fv;
typedef void glUniformMatrix4fvProc(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); glUniformMatrix4fvProc *glUniformMatrix4fv;
typedef void glUniform1iProc(GLint location, GLint v0);	glUniform1iProc *glUniform1i;
typedef void glUniform1fProc(GLint location, GLfloat v0); glUniform1fProc *glUniform1f;
typedef void glUniform2fProc(GLint location, GLfloat v0, GLfloat v1); glUniform2fProc *glUniform2f;
typedef void glUniform3fProc(GLint location, GLfloat v0, GLfloat v1, GLfloat v2); glUniform3fProc *glUniform3f;
typedef void glUniform4fProc(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3); glUniform4fProc *glUniform4f;
typedef void glVertexAttribPointerProc(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer); glVertexAttribPointerProc *glVertexAttribPointer;

typedef void glGenRenderbuffersProc(GLsizei n, GLuint *renderbuffers); glGenRenderbuffersProc *glGenRenderbuffers;
typedef void glBindRenderbufferProc(GLenum target, GLuint renderbuffer); glBindRenderbufferProc *glBindRenderbuffer;
typedef void glDeleteRenderbuffersProc(GLsizei n, GLuint *renderbuffers); glDeleteRenderbuffersProc *glDeleteRenderbuffers;
typedef void glRenderbufferStorageMultisampleProc(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height); glRenderbufferStorageMultisampleProc *glRenderbufferStorageMultisample;
typedef void glFramebufferRenderbufferProc(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer); glFramebufferRenderbufferProc *glFramebufferRenderbuffer;
typedef void glTexImage2DMultisampleProc(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations); glTexImage2DMultisampleProc *glTexImage2DMultisample;
typedef void glFramebufferTexture2DProc(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level); glFramebufferTexture2DProc *glFramebufferTexture2D;
typedef GLenum glCheckFramebufferStatusProc(GLenum target); glCheckFramebufferStatusProc *glCheckFramebufferStatus;
typedef void glBindFramebufferProc(GLenum target, GLuint framebuffer); glBindFramebufferProc *glBindFramebuffer;
typedef void glDeleteFramebuffersProc(GLsizei n, const GLuint *framebuffers); glDeleteFramebuffersProc *glDeleteFramebuffers;
typedef void glGenFramebuffersProc(GLsizei n, GLuint *framebuffers); glGenFramebuffersProc *glGenFramebuffers;
typedef void glBlitFramebufferProc(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter); glBlitFramebufferProc *glBlitFramebuffer;

typedef void glBindVertexArrayProc(GLuint array); glBindVertexArrayProc *glBindVertexArray;
typedef void glDeleteVertexArraysProc(GLsizei n, const GLuint *arrays); glDeleteVertexArraysProc *glDeleteVertexArrays;
typedef void glGenVertexArraysProc(GLsizei n, GLuint *arrays); glGenVertexArraysProc *glGenVertexArrays;
typedef void glDisableVertexAttribArrayProc(GLuint index); glDisableVertexAttribArrayProc *glDisableVertexAttribArray;
typedef void glEnableVertexAttribArrayProc(GLuint index); glEnableVertexAttribArrayProc *glEnableVertexAttribArray;

void
opengl_loader__fetch_extensions()
{
	#ifdef GLLOADER_USE_WGL
	if (wglGetExtensionsStringARB) {
		// Grab the win32 WGL extensions and add the to the hashmap!
		char *ext = wglGetExtensionsStringARB(wglGetCurrentDC());
		char *cursor = strtok((char *) ext, " ");
		for(; cursor != 0;) {
			char *next_space = strtok(0, " ");
			if(next_space) {
				uint64_t hash = str_hash(cursor, (next_space - cursor) - 1);
				map_put(&opengl_loader__supported_extensions, (void *) hash, (void *) cursor);
			}
			cursor = next_space;
		}
	}
	#endif

	if(glGetStringi) {
		// OpenGL 3+
		int num_ext;
		glGetIntegerv(GL_NUM_EXTENSIONS, &num_ext);
		for(int i = 0; i < num_ext; i++) {
			const GLubyte *ext = glGetStringi(GL_EXTENSIONS, i);
			if(ext) {
				uint64_t hash = str_hash((const char *) ext, strlen((const char *) ext));
				map_put(&opengl_loader__supported_extensions, (void *) hash, (void *) ext);
			}
		}
	} else {
		// Legacy opengl
		const GLubyte *ext = glGetString(GL_EXTENSIONS);
		char *cursor = strtok((char *) ext, " ");
		for(; cursor != 0;) {
			char *next_space = strtok(0, " ");
			if(next_space) {
				uint64_t hash = str_hash(cursor, (next_space - cursor) - 1);
				map_put(&opengl_loader__supported_extensions, (void *) hash, (void *) cursor);
			}
			cursor = next_space;
		}
		int a  = 2;
	}

}

bool
opengl_loader_supports_ext(const char *ext)
{
	assert(ext);
	return (map_get(&opengl_loader__supported_extensions, (void *) str_hash(ext, strlen(ext))) != NULL);
}

void
opengl_loader_init()
{
	static bool is_initialized;
	if(!is_initialized) {
#define GET_PROC_ADDRESS(fn) (fn == NULL ? fn = (fn ## Proc*) GLLOADER_GETPROC(#fn) : 0); \
	(!fn ? warn("Failed to find symbol %s", #fn) : 0)

		GET_PROC_ADDRESS(glGetStringi);
		GET_PROC_ADDRESS(glDebugMessageCallback);

		GET_PROC_ADDRESS(glActiveTexture);
		GET_PROC_ADDRESS(glBindBuffer);
		GET_PROC_ADDRESS(glDeleteBuffers);
		GET_PROC_ADDRESS(glGenBuffers);
		GET_PROC_ADDRESS(glBufferData);
		GET_PROC_ADDRESS(glBufferSubData);
		GET_PROC_ADDRESS(glMapBufferRange);
		GET_PROC_ADDRESS(glUnmapBuffer);

		GET_PROC_ADDRESS(glAttachShader);
		GET_PROC_ADDRESS(glCompileShader);
		GET_PROC_ADDRESS(glCreateProgram);
		GET_PROC_ADDRESS(glCreateShader);
		GET_PROC_ADDRESS(glDeleteProgram);
		GET_PROC_ADDRESS(glDeleteShader);
		GET_PROC_ADDRESS(glDetachShader);
		GET_PROC_ADDRESS(glGetProgramiv);
		GET_PROC_ADDRESS(glGetProgramInfoLog);
		GET_PROC_ADDRESS(glGetShaderInfoLog);
		GET_PROC_ADDRESS(glGetShaderSource);
		GET_PROC_ADDRESS(glGetShaderiv);
		GET_PROC_ADDRESS(glLinkProgram);
		GET_PROC_ADDRESS(glShaderSource);
		GET_PROC_ADDRESS(glUseProgram);
		GET_PROC_ADDRESS(glValidateProgram);
		GET_PROC_ADDRESS(glBindFragDataLocation);
		GET_PROC_ADDRESS(glBindAttribLocation);
		GET_PROC_ADDRESS(glGetAttribLocation);
		GET_PROC_ADDRESS(glGetUniformLocation);
		GET_PROC_ADDRESS(glUniformMatrix3fv);
		GET_PROC_ADDRESS(glUniformMatrix4fv);
		GET_PROC_ADDRESS(glUniform1i);
		GET_PROC_ADDRESS(glUniform1f);
		GET_PROC_ADDRESS(glUniform2f);
		GET_PROC_ADDRESS(glUniform3f);
		GET_PROC_ADDRESS(glUniform4f);
		GET_PROC_ADDRESS(glVertexAttribPointer);

		GET_PROC_ADDRESS(glGenRenderbuffers);
		GET_PROC_ADDRESS(glBindRenderbuffer);
		GET_PROC_ADDRESS(glDeleteRenderbuffers);
		GET_PROC_ADDRESS(glRenderbufferStorageMultisample);
		GET_PROC_ADDRESS(glFramebufferRenderbuffer);
		GET_PROC_ADDRESS(glTexImage2DMultisample);
		GET_PROC_ADDRESS(glFramebufferTexture2D);
		GET_PROC_ADDRESS(glCheckFramebufferStatus);
		GET_PROC_ADDRESS(glBindFramebuffer);
		GET_PROC_ADDRESS(glDeleteFramebuffers);
		GET_PROC_ADDRESS(glGenFramebuffers);
		GET_PROC_ADDRESS(glBlitFramebuffer);

		GET_PROC_ADDRESS(glBindVertexArray);
		GET_PROC_ADDRESS(glDeleteVertexArrays);
		GET_PROC_ADDRESS(glGenVertexArrays);
		GET_PROC_ADDRESS(glDisableVertexAttribArray);
		GET_PROC_ADDRESS(glEnableVertexAttribArray);

		#ifdef GLLOADER_USE_WGL
		GET_PROC_ADDRESS(wglCreateContextAttribsARB);
		GET_PROC_ADDRESS(wglGetExtensionsStringARB);
		GET_PROC_ADDRESS(wglGetSwapIntervalEXT);
		GET_PROC_ADDRESS(wglSwapIntervalEXT);
		#endif
#undef GET_PROC_ADDRESS

		// Prefetch all opengl extensions and make them avaiable to check via opengl_loader_supports_ext() function.
		opengl_loader__fetch_extensions();
		is_initialized = true;
	}
}

