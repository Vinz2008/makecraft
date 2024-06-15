#include "block.h"
#include "../utils/list.h"

#define CHUNK_LENGTH 100

#ifdef __cplusplus
extern "C" {
#endif

#define CHUNK_SIZE 16
#define CHUNK_HEIGHT 1 // TODO : replace this with 384

#define RENDER_DISTANCE 3 // TODO : make this a variable that can be set (will need to use a slider : TODO : use a real gui framework)

struct ChunkBlocks {
    Block blocks[CHUNK_HEIGHT][CHUNK_SIZE][CHUNK_SIZE];
};

struct Chunk {
    //BlockArray* blockArray;
    struct ChunkBlocks* blocks;
    float x, y; // pos of chunk from the chunk level (1 x more is 1 chunk more)
};

/*struct chunkArray {
    struct Chunk* array;
    size_t used;
    size_t size;
};*/


//void createChunk(BlockArray* blockArray, float x, float y, float z);
struct Chunk createChunk(float x, float y);
list_t* get_new_perlin_chunk_list();
void destroy_chunk_list(list_t* chunk_list);

#ifdef __cplusplus
}
#endif
