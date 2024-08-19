#include "block.h"
#include "../makecraft.h"

// TODO : instead of searching each frame where are the water blocks, write them in the chunks
void drawWater(std::vector<Block> blockArray, float x, float z, float ystart, float yend){
    for (float i = ystart; i < yend - 2.0f; i += 2.0f){
        drawBlock(x, i, z, material::water_texture);
    }
}