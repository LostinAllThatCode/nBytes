#define MATH_INL inline

#define PI 3.14159265358979323846264338327950288
const double PI2 = PI + PI;

#define lerp(a, b, t) ((a) + (t) * ((b)-(a)))


typedef union v2 {
	struct { float x, y; };
	struct { float u, v; };
	struct { float w, h; };
	float t[2];
} v2;

#if __cplusplus
inline v2
operator*(float A, v2 B)
{
	v2 Result;

	Result.x = A * B.x;
	Result.y = A * B.y;

	return(Result);
}

inline v2
operator*(v2 A, v2 B)
{
	v2 Result;

	Result.x = A.x * B.x;
	Result.y = A.y * B.y;

	return(Result);
}

inline v2
operator*(v2 B, float A)
{
	v2 Result = A * B;

	return(Result);
}

inline v2 &
operator*=(v2 &B, float A)
{
	B = A * B;

	return(B);
}

inline v2
operator/(v2 B, float A)
{
	v2 Result = (1.0f / A) * B;

	return(Result);
}

inline v2 &
operator/=(v2 &B, float A)
{
	B = B / A;

	return(B);
}

inline v2
operator-(v2 A)
{
	v2 Result;

	Result.x = -A.x;
	Result.y = -A.y;

	return(Result);
}

inline v2
operator+(v2 A, v2 B)
{
	v2 Result;

	Result.x = A.x + B.x;
	Result.y = A.y + B.y;

	return(Result);
}

inline v2 &
operator+=(v2 &A, v2 B)
{
	A = A + B;

	return(A);
}

inline v2
operator-(v2 A, v2 B)
{
	v2 Result;

	Result.x = A.x - B.x;
	Result.y = A.y - B.y;

	return(Result);
}

inline v2 &
operator-=(v2 &A, v2 B)
{
	A = A - B;

	return(A);
}

#endif


MATH_INL v2 vec2(float x, float y) { v2 result = { x, y }; return result; }
MATH_INL v2 vec2_normalize(v2 A)
{
	float length = sqrt(A.x * A.x + A.y * A.y);
	v2 result = { A.x / length, A.y / length };
	return result;
}
MATH_INL float vec2_dot(v2 A, v2 B)
{
	return A.x * B.x + A.y * B.y;
}
MATH_INL v2 vec2_add(v2 A, v2 B)
{
	v2 result = { A.x + B.x, A.y + B.y};
	return result;
}
MATH_INL v2 vec2_sub(v2 A, v2 B)
{
	v2 result = { A.x - B.x, A.y - B.y};
	return result;
}
MATH_INL v2 vec2_mul(v2 A, v2 B)
{
	v2 result = { A.x * B.x, A.y * B.y};
	return result;
}
MATH_INL v2 vec2_mulf(float v, v2 B)
{
	v2 result = { v * B.x, v * B.y};
	return result;
}
MATH_INL v2 vec2_div(v2 A, v2 B)
{
	v2 result = { A.x / B.x, A.y / B.y};
	return result;
}
MATH_INL v2 vec2_divf(float v, v2 B)
{
	v2 result = {B.x / v, B.y / v};
	return result;
}
MATH_INL float vec2_lengthsq(v2 A)
{
	return (A.x * A.x + A.y * A.y);
}
MATH_INL v2 vec2_perp(v2 A)
{
	v2 result = { -A.y, A.x };  // counter-clockwise
	//v2 result = { A.y, -A.x }; // clockwise
	return result;
}



typedef union v3 {
	struct { float x, y, z; };
	struct { float r, g, b; };
	float t[3];
} v3;
MATH_INL v3 vec3(float x, float y, float z) { v3 result = { x, y, z }; return result; }
MATH_INL v3 vec3_normalize(v3 A)
{
	float length = sqrt(A.x * A.x + A.y * A.y + A.z * A.z);
	v3 result = { A.x / length, A.y / length, A.z / length };
	return result;
}
MATH_INL float vec3_dot(v3 A, v3 B)
{
	return A.x * B.x + A.y * B.y;
}
MATH_INL v3 vec3_add(v3 A, v3 B)
{
	v3 result = { A.x + B.x, A.y + B.y, A.z + B.z};
	return result;
}
MATH_INL v3 vec3_sub(v3 A, v3 B)
{
	v3 result = { A.x - B.x, A.y - B.y, A.z - B.z};
	return result;
}
MATH_INL v3 vec3_mul(v3 A, v3 B)
{
	v3 result = { A.x * B.x, A.y * B.y, A.z * B.z};
	return result;
}
MATH_INL v3 vec3_div(v3 A, v3 B)
{
	v3 result = { A.x / B.x, A.y / B.y, A.z / B.z};
	return result;
}
MATH_INL float vec3_lengthsq(v3 A)
{
	return (A.x * A.x + A.y * A.y + + A.z * A.z);
}

typedef union v4 {
	struct { float x, y, z, w; };
	struct { float r, g, b, a; };
	float t[4];
} v4;
MATH_INL v4 vec4(float r, float g, float b, float a)
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

MATH_INL mx4
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

MATH_INL mx4
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

MATH_INL mx4
mx_perspective(float fov, float aspect, float znear, float zfar)
{

	float t = znear * tan(fov * PI / 360.f);
	float r = t * aspect;
	return mx_frustum(-r, r, -t, t, znear, zfar);
}

typedef struct Rectangle2 {
	float top, left, bottom, right;
} Rectangle2;

typedef struct Rectangle3 {
	v3 min, max;
} Rectangle3;

Rectangle2
rect2_from_center(v2 center, v2 dim)
{
	Rectangle2 result = {
		center.y + dim.y * 0.5f, // TOP
		center.x - dim.x * 0.5f, // LEFT
		center.y - dim.y * 0.5f, // BOTTOM
		center.x + dim.x * 0.5f, // RIGHT
	};
	return result;
}

v2
rect2_get_center(Rectangle2 rect)
{
	v2 center = { rect.left + ((rect.right - rect.left) *.5f), rect.bottom + ((rect.top - rect.bottom) * .5f) };
	return center;
}

bool
rect2_vs_rect2_collision(Rectangle2 A, Rectangle2 B)
{
	return !((B.right <= A.left) || (B.left >= A.right) || (B.top <= A.bottom) || (B.bottom >= A.top));
}

bool
rect2_vs_circle_collision(Rectangle2 A, v2 center, float radius)
{
	float x = MAX(A.left, MIN(center.x, A.right));
	float y = MAX(A.bottom, MIN(center.y, A.top));
	float distance = sqrt( ((x - center.x) * (x - center.x)) + ((y - center.y) * (y - center.y)));
	return distance < radius;
}

bool
rect3_vs_rect3_collision(Rectangle3 A, Rectangle3 B)
{
	return !((B.max.x <= A.min.x) || (B.min.x >= A.max.x) ||
	         (B.max.y <= A.min.y) || (B.min.y >= A.max.y) ||
	         (B.max.z <= A.min.z) || (B.min.z >= A.max.z));
}

bool
rect3_vs_sphere_collision(Rectangle3 A, v3 center, float radius)
{
	float x = MAX(A.min.x, MIN(center.x, A.max.x));
	float y = MAX(A.min.y, MIN(center.y, A.max.y));
	float z = MAX(A.min.z, MIN(center.z, A.max.z));
	float distance = sqrt( ((x - center.x) * (x - center.x)) +
	                       ((y - center.y) * (y - center.y)) +
	                       ((z - center.z) * (z - center.z)));
	return distance < radius;
}

#if 0
bool
rect2_intersect(Rectangle2 A, Rectangle2 B)
{
	v2 center_a = rect2_get_center(A);
	v2 halfdim_a = vec2_mulf(0.5f, rect2_get_diameter(A));
	v2 center_b = rect2_get_center(B);
	v2 halfdim_b = vec2_mulf(0.5f, rect2_get_diameter(B));

	v2 _ab = vec2(abs(center_b.x - center_a.x), abs(center_b.y - center_a.y));
	v2 r = vec2_sub(_ab, vec2_add(halfdim_a, halfdim_b));
	return (r.x < 0 && r.y < 0);
}


bool
rect2_intersect(Rectangle2 A, Rectangle2 B)
{
	bool result = !((B.max.x <= A.min.x) ||
	                (B.min.x >= A.max.x) ||
	                (B.max.y <= A.min.y) ||
	                (B.min.y >= A.max.y)
	               );
	return(result);
}

bool
point2_inside_rect2(v2 T, Rectangle2 R)
{
	return (T.x >= R.min.x && T.x <= R.max.x && T.y >= R.min.y && T.y <= R.max.y);
}

bool
inside_of(v3 pos, AABB box)
{
	v3 min = vec3(box.center.x - box.half_dim.x,
	              box.center.y - box.half_dim.y,
	              box.center.z - box.half_dim.z);
	v3 max = vec3(box.center.x + box.half_dim.x,
	              box.center.y + box.half_dim.y,
	              box.center.z + box.half_dim.z);
	return (pos.x >= min.x && pos.x <= max.x &&
	        pos.y >= min.y && pos.y < max.y &&
	        pos.z >= min.z && pos.z < max.z);
}

bool
intersect(AABB a, AABB b)
{
	v3 ab = vec3(abs(b.center.x - a.center.x),
	             abs(b.center.y - a.center.y),
	             abs(b.center.z - a.center.z));
	v3 r = vec3_sub(ab, vec3_add(a.half_dim, b.half_dim));
	if(a.half_dim.z == 0) {
		return (r.x < 0 && r.y < 0);
	} else {
		return (r.x < 0 && r.y < 0 && r.z < 0);
	}
}

///
float w = 0.5 * ((Player.dim.x + collide_with.dim.x) * 1.05f);
float h = 0.5 * ((Player.dim.y + collide_with.dim.y) * 1.05f);
float dx = collide_with.pos.x - new_pos.x;
float dy = collide_with.pos.y - new_pos.y;

if (fabs(dx) <= w && fabs(dy) <= h)
{
	/* collision! */
	float wy = w * dy;
	float hx = h * dx;

	if (wy > hx) {
		if (wy > -hx) {
			// top
			new_vel.y = 0;
		} else {
			// left
			new_vel.x = 0;
		}
	} else if (wy > -hx) {
		// right
		new_vel.x = 0;
	} else {
		// bottom
		new_vel.y = 0;
	}
}

#endif