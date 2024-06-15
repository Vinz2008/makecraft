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

void initBlockArray(BlockArray* blockArray, size_t initalSize){
    blockArray->blockArray = malloc(sizeof(Block) * initalSize);
    blockArray->used = 0;
    blockArray->size = initalSize;
}

void addToBlockArray(BlockArray* blockArray, Block block){
    printf("TEST add to block\n");
     if (blockArray->used == blockArray->size){
        blockArray->size *=2;
        blockArray->blockArray = realloc(blockArray->blockArray, blockArray->size * sizeof(Block));
    }
    printf("TEST add to block after potential realloc\n used : %ld, block x : %f, block y : %f, block z : %f\n", blockArray->used, block.x, block.y, block.z);
    blockArray->blockArray[blockArray->used++] = block;
    printf("TEST add to block end\n");
}


void emptyBlockArray(BlockArray* blockArray){
    free(blockArray->blockArray);
    blockArray->blockArray = NULL;
    blockArray->used = blockArray->size = 0;
}

void destroyBlockArray(BlockArray* blockArray){
    emptyBlockArray(blockArray);
    free(blockArray);
}

BlockArray* removeFromBlockArray(int index, BlockArray* blockArray){
    BlockArray* newBlockArray = malloc(sizeof(BlockArray));
    initBlockArray(newBlockArray, blockArray->size);
    memmove(newBlockArray->blockArray, blockArray->blockArray, (index)*sizeof(Block)); 
    memmove(newBlockArray->blockArray+index, blockArray->blockArray+(index+1), (blockArray->used - index - 1)*sizeof(Block)); 
    newBlockArray->size = blockArray->size;
    newBlockArray->used = blockArray->used;
    free(blockArray);
    return newBlockArray;
}

void createBlock(BlockArray* blockArray, float x, float y, float z, int texture){
    Texture2D texture2dBlock = getBlockTexture(texture);
    Color col = WHITE;
    DrawCubeTexture(texture2dBlock, (Vector3){x, y, z }, CUBE_SIZE , CUBE_SIZE , CUBE_SIZE , col);
}


bool equalf(float a, float b, float epsilon){
  return fabs(a - b) < epsilon;
}

bool isCubeNextToCube(BlockArray* blockArray, int direction, Block block){
    //printf("blockArray->used : %ld\n", blockArray->used);
    for (int i = 0; i < blockArray->used; i++){
        Block* tempBlock = blockArray->blockArray + i;
        //printf("pos 1 block.y + CUBE_SIZE : %f, pos 2 blockArray->blockArray[%d].y y : %f\n", block.y + CUBE_SIZE, i, tempBlock->y); // TODO : why i is always 0 ??
        switch (direction){
        case direction_top:
            return block.y + CUBE_SIZE == tempBlock->y;
        case direction_bottom:
            return block.y - CUBE_SIZE == tempBlock->y;
        case direction_right:
            return equalf(block.x + CUBE_SIZE, tempBlock->x, CUBE_SIZE/2);
        case direction_left:
            return equalf(block.x - CUBE_SIZE, tempBlock->x, CUBE_SIZE/2);
        case direction_front:
            return equalf(block.z + CUBE_SIZE, tempBlock->z, CUBE_SIZE/2);
        case direction_back:
            return equalf(block.z - CUBE_SIZE, tempBlock->z, CUBE_SIZE/2);
        default:
            fprintf(stderr, "unknown direction");
            exit(1);
        }
    }
    return false;
    
}