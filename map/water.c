#include "block.h"
#include "../makecraft.h"

void createWater(BlockArray *blockArray, float x, float z, float ystart, float yend){
    for (float i = ystart; i < yend; i += 2.0f){
        createBlock(blockArray, x, i, z, 0);
    }
}