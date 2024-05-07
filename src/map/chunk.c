#include "chunk.h"
#include "block.h"
#include "../makecraft.h"

void createChunk(BlockArray* blockArray, float x, float y, float z){
    int i;
    int i2;
    for (i2 = 0; i2 < CHUNK_LENGTH * 2; i2 = i2 + 2) {
        for (i = 0; i < CHUNK_LENGTH * 2; i = i + 2) {
            int temp = 1;
            createBlock(blockArray, x + (float)i, y * temp, z + (float)i2, dirt_texture);
        }
    }
}