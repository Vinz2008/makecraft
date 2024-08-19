#include "block.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"
#include "chunk.h"
#include "../engine/cube_render.h"
#include "../makecraft.h"
#include "../textures.h"


void drawBlock(float x, float y, float z, enum material texture){
    Texture2D texture2dBlock = getBlockTexture(texture);
    Color col = WHITE;
    DrawCubeTexture(texture2dBlock, (Vector3){x, y, z }, CUBE_SIZE , CUBE_SIZE , CUBE_SIZE , col);
}


bool equalf(float a, float b, float epsilon){
  return fabs(a - b) < epsilon;
}

bool isCubeNextToCube(std::vector<Block>& blockArray, int direction, Block block){
    //printf("blockArray->used : %ld\n", blockArray->used);
    for (int i = 0; i < blockArray.size(); i++){
        Block& tempBlock = blockArray.at(i);
        //printf("pos 1 block.y + CUBE_SIZE : %f, pos 2 blockArray->blockArray[%d].y y : %f\n", block.y + CUBE_SIZE, i, tempBlock->y); // TODO : why i is always 0 ??
        switch (direction){
        case direction_top:
            return equalf(block.y + CUBE_SIZE, tempBlock.y, CUBE_SIZE/2);
        case direction_bottom:
            return equalf(block.y - CUBE_SIZE, tempBlock.y, CUBE_SIZE/2);
        case direction_right:
            return equalf(block.x + CUBE_SIZE, tempBlock.x, CUBE_SIZE/2);
        case direction_left:
            return equalf(block.x - CUBE_SIZE, tempBlock.x, CUBE_SIZE/2);
        case direction_front:
            return equalf(block.z + CUBE_SIZE, tempBlock.z, CUBE_SIZE/2);
        case direction_back:
            return equalf(block.z - CUBE_SIZE, tempBlock.z, CUBE_SIZE/2);
        default:
            fprintf(stderr, "unknown direction");
            exit(1);
        }
    }
    return false;
    
}