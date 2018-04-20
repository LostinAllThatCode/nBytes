#define PI 3.14159265358979323846264338327950288
const double PI2 = PI + PI;

// put this stuff into a math thing ?!?!?
typedef union v2 {
	struct { float x, y; };
	struct { float u, v; };
	struct { float w, h; };
	float t[2];
} v2;
__forceinline v2 vec2(float x, float y) { v2 result = { x, y }; return result; }

typedef union v3 {
	struct { float x, y, z; };
	struct { float r, g, b; };
	float t[3];
} v3;
__forceinline v3 vec3(float x, float y, float z) { v3 result = { x, y, z }; return result; }

typedef union v4 {
	struct { float x, y, z, w; };
	struct { float r, g, b, a; };
	float t[4];
} v4;
__forceinline v4 vec4(float r, float g, float b, float a)
{
	v4 result = {r, g, b, a};
	return result;
}

typedef union mx3 {
	float m[3][3];
	float t[12];
} mx3;

typedef union mx4 {
	float m[4][4];
	float t[16];
} mx4;

__forceinline mx4
mx_ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
	float rl = 2.0f / (right - left);
	float tb = 2.0f / (top - bottom);
	float fn = -2.0f / (zfar - znear);
	float tx = -((right + left) / (right - left));
	float ty = -((top + bottom) / (top - bottom));
	float tz = -((zfar + znear) / (zfar - znear));

	mx4 result = {
		rl, 	0.0f, 	0.0f, 	0.0f,
		0.0f, 	tb, 	0.0f, 	0.0f,
		0.0f, 	0.0f, 	fn, 	0.0f,
		tx, 	ty, 	tz, 	1.0f,
	};
	return (result);
}

__forceinline mx4
mx_frustum(float left, float right, float bottom, float top, float znear, float zfar)
{
	float x1 = (znear * 2.0f) / (right - left);
	float z1 = (right + left) / (right - left);
	float y2 = (znear * 2.0f) / (top - bottom);
	float z2 = (top + bottom) / (top - bottom);
	float z3 = -((zfar + znear) / (zfar - znear));
	float w3 = -((2.0f * zfar * znear) / (zfar - znear));
	mx4 result = {
		x1,  0,  0,  0,
		0,  y2,  0,  0,
		z1, z2, z3, -1,
		0,   0, w3,  0,
	};
	return result;
}

__forceinline mx4
mx_perspective(float fov, float aspect, float znear, float zfar)
{

	float t = znear * tan(fov * PI / 360.f);
	float r = t * aspect;
	return mx_frustum(-r, r, -t, t, znear, zfar);
}