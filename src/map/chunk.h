#include "block.h"

#define CHUNK_LENGTH 100

#ifdef __cplusplus
extern "C" {
#endif


struct Chunk {
    BlockArray* blockArray;
    float x, y, z; // pos of chunk from the chunk level (1 x more is 1 chunk more)
};

struct chunkArray{
    struct Chunk* array;
    size_t used;
    size_t size;
};


void createChunk(BlockArray* blockArray, float x, float y, float z);

#ifdef __cplusplus
}
#endif
