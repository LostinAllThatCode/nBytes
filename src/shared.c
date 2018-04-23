#if defined __clang__
	#define COMPILER_CLANG 1
#elif defined __GNUC__
	#define COMPILER_GCC 1
#elif defined _MSC_VER
	#define COMPILER_MSVC 1
#else
	#error This compiler is not supported!
#endif

#if defined _WIN32 || defined _WIN64
	#define OS_WINDOWS 1
#elif defined __APPLE__ && defined __MACH__
	#define OS_OSX 1
#elif defined __unix__
	#if defined __ANDROID__
		#define OS_ANDROID 1
	#elif defined __linux__
		#define OS_LINUX 1
	#endif
#else
	#error This operating system!
#endif

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
	#define CPU_X86 1
#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
	#define CPU_PPC 1
#elif defined(__arm__)
	#define CPU_ARM 1
#elif defined(__MIPSEL__) || defined(__mips_isa_rev)
	#define CPU_MIPS 1
#else
	#error This cpu is not supported!
#endif

#define ISUNICODE(c) 			(((c)&0xc0)==0xc0)
#define IS_POW2(x) 				(((x) != 0) && ((x) & ((x)-1)) == 0)
#define MIN(a, b) 				(((a) < (b) ? (a) : (b)))
#define MAX(a, b) 				(((a) < (b) ? (b) : (a)))
#define SIGN(a) 				((a) > 0) ? 1 : (((a) < 0) ? -1 : 0)
#define ARRAY_LEN(a) 			(sizeof(a)/sizeof(a[0]))


#if 0
	// shared typedef
	typedef uint8_t				u8;
	typedef int8_t   			i8;
	typedef uint16_t 			u16;
	typedef int16_t 			i16;
	typedef uint32_t 			u32;
	typedef int32_t 			i32;
	typedef float				r32;
	typedef double				r64;
	typedef unsigned long long 	u64;
	typedef long long 			i64;
	typedef int32_t 			b32;
	#define true 				1
	#define false 				0
#endif

typedef union int2 {
	struct {
		int32_t x;
		int32_t y;
	};
	struct {
		int32_t width;
		int32_t height;
	};
} int2;
__forceinline int2 INT2(int32_t x, int32_t y) { int2 result = {x, y,}; return result; }

typedef union int3 {
	struct {
		int32_t x;
		int32_t y;
		int32_t z;
	};
	struct {
		int32_t width;
		int32_t height;
		int32_t depth;
	};
} int3;

typedef union int4 {
	struct {
		int32_t x;
		int32_t y;
		int32_t z;
		int32_t w;
	};
	struct {
		int32_t left;
		int32_t right;
		int32_t top;
		int32_t bottom;
	};
} int4;

// core functions
void *
xmalloc(size_t size)
{
	void *result = malloc(size);
	assert(result);
	return result;
}

void *
xcalloc(size_t elems, size_t elem_size)
{
	void *result = calloc(elems, elem_size);
	assert(result);
	return result;
}

void *
xrealloc(void *ptr, size_t new_size)
{
	void *result = realloc(ptr, new_size);
	assert(result);
	return result;
}

void
xfree(void *ptr)
{
	free(ptr);
}

void
warn(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("WARN : ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}

void
fatal(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("FATAL: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
	__debugbreak();
	exit(1);
}

char *
read_file(const char *filename, size_t *out_filesize)
{
	char *result = 0;
	FILE *file = fopen(filename, "rb");
	if(!file) {	fatal("File not found: %s\n", filename); }

	assert(fseek(file, 0, SEEK_END) != -1L);
	size_t size = ftell(file);
	fseek(file, 0, SEEK_SET);
	result = (char *) xmalloc(size + 1);
	assert(fread(result, 1, size, file) == size);
	fclose(file);
	result[size] = 0;
	if(out_filesize) { *out_filesize = size; }
	return result;
}

bool
write_file(const char *filename, void *data, size_t len)
{
	FILE *file = fopen(filename, "wb");
	assert(file);
	assert(fwrite(data, 1, len, file) == len);
	fclose(file);
	return false;
}

typedef union Rect {
	struct {
		float left;
		float right;
		float top;
		float bottom;
	};
	struct {
		float x;
		float width;
		float y;
		float height;
	};
} Rect;

// stretchy buffer (Sean Barrett)
typedef struct BufHdr {
	size_t	len;
	size_t 	cap;
	char 	buf[];
} BufHdr;

#define buf__hdr(b) 			((BufHdr*)((char*)b - offsetof(BufHdr, buf)))
#define buf_fit(b, n) 			((n) <= buf_cap(b) ? 0 : (*((void **)&(b)) = buf__grow((b), (n), sizeof(*(b)))))

#define buf_len(b) 				((b) ? buf__hdr(b)->len : 0)
#define buf_cap(b) 				((b) ? buf__hdr(b)->cap : 0)
#define buf_end(b) 				((b) + buf_len(b))

#define buf_printf(b, ...) 		((b) = buf__printf((b), __VA_ARGS__))
#define buf_push_raw(b, d, sz)	(buf_fit((b), 1 + buf_len((b)) + (sz)), memcpy((b) + buf_len((b)), (d), (sz)), buf__hdr(b)->len += (sz) )
#define buf_push(b, ...)		(buf_fit((b), 1 + buf_len((b))), (b)[buf__hdr(b)->len++] = (__VA_ARGS__))
#define buf_peek(b)				((b) && buf_len(b) != 0 ? &((b)[buf_len(b)-1]) : 0)
#define buf_clear(b) 			((b) ? buf__hdr(b)->len = 0 : 0)
#define buf_zero(b)				((b) ? memset(b, 0, buf_cap(b) * sizeof(*(b))), buf_clear(b) : 0)
#define buf_free(b) 			(xfree(buf__hdr(b)), (b) = 0)

// just swaps the last element with i's position. Do not use while iterating over the buffer!!
#define buf_remove(b, i)		((b)[i] = (b)[buf__hdr(b)->len-1], buf__hdr(b)->len--)

void *
buf__grow(const void *buf, size_t new_len, size_t elem_size)
{
	assert(buf_cap(buf) <= (SIZE_MAX - 1) / 2); // find out why that has to happen ?
	size_t new_cap 	= MAX(16, MAX( 1 + 2 * buf_cap(buf), new_len));
	assert(new_len <= new_cap);
	assert(new_cap <= (SIZE_MAX - offsetof(BufHdr, buf)) / elem_size);
	size_t new_size = offsetof(BufHdr, buf) + new_cap * elem_size;

	BufHdr *new_hdr;
	if(buf) {
		new_hdr 		= (BufHdr *) xrealloc(buf__hdr(buf), new_size);
	} else {
		new_hdr 		= (BufHdr *) xmalloc(new_size);
		new_hdr->len 	= 0;
	}
	new_hdr->cap = new_cap;
	return new_hdr->buf;
}

char *buf__printf(char *buf, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	size_t cap = buf_cap(buf) - buf_len(buf);
	size_t n = 1 + vsnprintf(buf_end(buf), cap, fmt, args);
	va_end(args);
	if (n > cap) {
		buf_fit(buf, n + buf_len(buf));
		va_start(args, fmt);
		size_t new_cap = buf_cap(buf) - buf_len(buf);
		n = 1 + vsnprintf(buf_end(buf), new_cap, fmt, args);
		assert(n <= new_cap);
		va_end(args);
	}
	buf__hdr(buf)->len += n - 1;
	return buf;
}

void
buf_test()
{
	int *a = 0;
	buf_push(a, 12);
	assert(a[0] == 12);
	buf_zero(a);
	assert(a[0] == 0);

	int n = 32;
	for(int i = 0; i < n; i++) {
		buf_push(a, n);
	}
	assert(buf_len(a) == n);

	buf_clear(a);
	assert(buf_len(a) == 0);

	buf_free(a);
	assert(a == 0);
}


// Hash map

uint64_t uint64_hash(uint64_t x)
{
	x *= 0xff51afd7ed558ccdul;
	x ^= x >> 32;
	return x;
}

uint64_t ptr_hash(void *ptr)
{
	return uint64_hash((uintptr_t)ptr);
}

uint64_t str_hash(const char *str, size_t len)
{
	uint64_t x = 0xcbf29ce484222325ull;
	for (size_t i = 0; i < len; i++) {
		x ^= str[i];
		x *= 0x100000001b3ull;
		x ^= x >> 32;
	}
	return x;
}

typedef struct Map {
	void 	**keys;
	void 	**vals;
	size_t 	len;
	size_t 	cap;
} Map;

void map_grow(Map *map, size_t new_cap);

void *map_get(Map *map, void *key)
{
	assert(key);
	if (map->len == 0) {
		return NULL;
	}
	assert(map->len < map->cap);
	assert(IS_POW2(map->cap));
	size_t i = (size_t) ptr_hash(key);
	for (;;) {
		i &= map->cap - 1;
		if (map->keys[i] == key) {
			return map->vals[i];
		} else if (!map->keys[i]) {
			return NULL;
		}
		i++;
	}
	return NULL;
}

void map_put(Map *map, void *key, void *val)
{
	assert(key);
	assert(val);
	if (2 * map->len >= map->cap) {
		map_grow(map, 2 * map->cap);
	}
	assert(2 * map->len < map->cap);
	assert(IS_POW2(map->cap));
	size_t i = (size_t)ptr_hash(key);
	for (;;) {
		i &= map->cap - 1;
		if (!map->keys[i]) {
			map->len++;
			map->keys[i] = key;
			map->vals[i] = val;
			return;
		} else if (map->keys[i] == key) {
			map->vals[i] = val;
			return;
		}
		i++;
	}
}

void map_grow(Map *map, size_t new_cap)
{
	new_cap = MAX(16, new_cap);
	Map new_map = {
		(void **) xcalloc(new_cap, sizeof(void *)),
		(void **) xmalloc(new_cap * sizeof(void *)),
		0,
		new_cap,
	};
	for (size_t i = 0; i < map->cap; i++) {
		if (map->keys[i]) {
			map_put(&new_map, map->keys[i], map->vals[i]);
		}
	}
	free(map->keys);
	free(map->vals);
	*map = new_map;
}

void map_test(void)
{
	Map map = {0};
	enum { N = 1024 };
	for (size_t i = 1; i < N; i++) {
		map_put(&map, (void *)i, (void *)(i + 1));
	}
	for (size_t i = 1; i < N; i++) {
		void *val = map_get(&map, (void *)i);
		assert(val == (void *)(i + 1));
	}
}

// serialization

typedef struct SlzEntry {
	uint64_t bitset;
	union {
		uint8_t  u8;
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;
		void  	 *ptr;
	};
} SlzEntry;

typedef struct Serializer {
	SlzEntry *entries;
} Serializer;

#define serialize_val(val, serial) (serialize__value(1ull << 60 | sizeof(val), (uint64_t) val, serial))
#define serialize_arr(val, len, serial) (serialize__value(1ull << 61 | len, (uint64_t) val, serial))
#define serialize_ptr(ptr, len, serial) (serialize__pointer(1ull << 62 | len, ptr, len, serial))

void serialize__value(uint64_t bitset, uint64_t value, Serializer *serial)
{
	SlzEntry entry = {0};
	entry.bitset = bitset;
	entry.u64 = value;
	buf_push(serial->entries, entry);
}

void serialize__pointer(uint64_t bitset, void *ptr, size_t len, Serializer *serial)
{
	SlzEntry entry = {0};
	entry.bitset = bitset;
	entry.ptr = ptr;
	buf_push(serial->entries, entry);
}

Serializer serialize_begin()
{
	Serializer result = {0};
	return result;
}

void *serialize_end(Serializer *serial)
{
	uint8_t *buf = 0;
	int magic = 0xBD010101;
	buf_push_raw(buf, &magic, sizeof(magic));

	serialize__value(1ull << 63, 0, serial);

	size_t data_offset = buf_len(buf) + buf_len(serial->entries) * sizeof(SlzEntry);
	for(int i = 0; i < buf_len(serial->entries); i++) {
		SlzEntry entry = serial->entries[i];
		int type = entry.bitset >> 60;
		int size = (entry.bitset & 0xffffffff);
		switch(type) {
			case 2: {
				// inline static array data will be stored in the structure data itself so we can just zero this!
				entry.u64 = 0ul;
			} break;
			/* POINTER */
			case 4: {
				// offset from beginning of the file to the data off the pointer
				entry.u64 = data_offset;
				data_offset += size;
			} break;
			/** garanteed last element **/
			case 8: {
				// offset from beginning of the file to the structure data
				entry.u64 = data_offset;
			} break;
		}
		buf_push_raw(buf, &entry, sizeof(entry));
	}


	for(int i = 0; i < buf_len(serial->entries); i++) {
		SlzEntry *entry = serial->entries + i;
		assert(entry);

		/** pre allocate pointer data in front of structure data! */
		int type = entry->bitset >> 60;
		int size = (entry->bitset & 0xffffffff);
		switch(type) {
			/* POINTER */
			case 4: {
				void *ptr = entry->ptr;
				buf_push_raw(buf, ptr, size);
			} break;
		}
	}

	for(int i = 0; i < buf_len(serial->entries); i++) {
		SlzEntry entry = serial->entries[i];
		int type = entry.bitset >> 60;
		int size = (entry.bitset & 0xffffffff);
		switch(type) {
			case 1: {
				buf_push_raw(buf, &entry.u8, size);
			} break;
			case 2: {
				buf_push_raw(buf, entry.ptr, size);
			} break;
			case 4: {
				int zero_ptr = 0;
				buf_push_raw(buf, &zero_ptr, sizeof(void *));
			} break;
		}
	}
	return buf;
}

void *deserialize(uint8_t *data, size_t len)
{
	assert(*(int *)data == 0xBD010101);
	SlzEntry *s = (SlzEntry *) (data + sizeof(int));

	int offset = 0;
	int count = 0;
	for(SlzEntry *entry = s;; entry++) {
		count++;
		if((entry->bitset >> 60) == 8) {
			offset = entry->u64;
			break;
		}
	}

	uint8_t *head = data + offset;
	uint8_t *cur  = head;
	for(SlzEntry *entry = s; entry < s + count; entry++) {
		int type = entry->bitset >> 60;
		int size = entry->bitset & 0xffffffff;
		if(type == 4) {
			uint64_t *ptr_offset = (uint64_t *) cur;
			*ptr_offset = (uint64_t) data + entry->u64;
			cur += sizeof(void *);
		} else {
			cur += size;
		}
	}

	return head;
}
