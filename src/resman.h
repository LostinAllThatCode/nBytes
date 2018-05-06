#define RESMAN_CLEANUP_INTERVAL_SEC 	15
#define RESMAN_CLEANUP_THRESHOLD_SEC 	5

#define RESMAN_TEXOPS_MAX_NUM 			64
#define RESMAN_TEXHANDLE_INVALID 		0xFFFE

typedef struct Texture {
	int width;
	int height;
	int channels;

	int gpu_handle;
} Texture;

typedef struct Font {
	Texture texture;
	BakedFont baked;
} Font;

typedef enum ResOpType {
	RES_OPTYPE_NOOP,
	RES_OPTYPE_FETCH_DATA,
	RES_OPTYPE_FREE_DATA,
} ResOpType;

typedef struct ResOp {
	ResOpType res_type;
	int res_id;

	union {
		struct {
			int *ranges;
			int range_count;
			int padding;
			int oversample_h;
			int oversample_v;
		} font_gen;
	} opt;
} ResOp;

typedef enum ResourceType {
	RES_TYPE_UNKNOWN,
	RES_TYPE_IMAGE,
	RES_TYPE_FONT,
	RES_TYPE_AUDIO,
} ResourceType;

typedef enum ResourceState {
	RES_STATE_UNINITIALIZED,
	RES_STATE_CPU_CACHED,
	RES_STATE_GPU_CACHED,
} ResourceState;

typedef struct ResourceFile {
	int id;
	ResourceType type;
	ResourceState state;

	const char *filename;
	const char *extension;
	void *data_ptr;

	union {
		Texture texture;
		Font font;
	} data;

	struct {
		uint64_t modification_time;
		uint64_t filesize;
	} info;

	bool was_modified;
	uint64_t last_used;
} ResourceFile;

typedef struct ResourceManager {
	const char *root_dir;
	ResourceFile *files;
	ResOp resops[RESMAN_TEXOPS_MAX_NUM];
	int resop_count;
} ResourceManager;
