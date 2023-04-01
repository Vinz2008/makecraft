#include "block.h"
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "chunk.h"
#include "../engine/cube_render.h"
#include "../makecraft.h"

extern Texture2D DirtTexture;
extern Texture2D StoneTexture;
extern Texture2D WaterTexture;

void initBlockArray(BlockArray* blockArray, size_t initalSize){
    blockArray->blockArray = malloc(sizeof(Block) * initalSize);
    blockArray->used = 0;
    blockArray->size = initalSize;
}

void addToBlockArray(BlockArray* blockArray, Block block){
     if (blockArray->used == blockArray->size){
        blockArray->size *=2;
        blockArray->blockArray = realloc(blockArray->blockArray, blockArray->size * sizeof(Block));
    }
    blockArray->blockArray[blockArray->used++] = block;
}


void emptyBlockArray(BlockArray* blockArray){
    free(blockArray->blockArray);
    blockArray->blockArray = NULL;
    blockArray->used = blockArray->size = 0;
}

Block* createBlock(BlockArray* blockArray, float x, float y, float z, int texture){
    Block* tempBlock = malloc(sizeof(Block));
    tempBlock->x = x;
    tempBlock->y = y;
    tempBlock->z = z;
    //addToBlockArray(blockArray, *tempBlock);
    //Model tempCube = LoadModelFromMesh(GenMeshCube(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE));
    //DrawModel(tempCube, (Vector3){x, y, z}, 1,RED);
    //DrawCube((Vector3){x, y, z}, CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, RED);
    //DrawCubeWires((Vector3){x, y, z}, CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, BLACK);
    Texture2D* texture2dBlock = NULL;
    Color col = WHITE;
    if (texture == dirt_texture){
        texture2dBlock = &DirtTexture;
    }  else if (texture == stone_texture){
        texture2dBlock = &StoneTexture;
    } else if (texture == water_texture){
        texture2dBlock = &WaterTexture;
        col = CLITERAL(Color){ 255, 255, 255, 200 };
    }
    DrawCubeTexture(*texture2dBlock, (Vector3){x, y, z }, CUBE_SIZE , CUBE_SIZE , CUBE_SIZE , col);
    return tempBlock;
}


bool isCubeNextToCube(BlockArray* blockArray, int direction, Block block){
    for (int i = 0; i < blockArray->used; i++){
        Block* tempBlock = &blockArray->blockArray[i];
        switch (direction)
        {
        case direction_top:
            return tempBlock->y + 2*CUBE_SIZE == block.y;
        case direction_bottom:
            return tempBlock->y - 2*CUBE_SIZE == block.y;
        case direction_right:
            return tempBlock->x + 2*CUBE_SIZE == block.x;
        case direction_left:
            return tempBlock->x - 2*CUBE_SIZE == block.x;
        case direction_front:
            return tempBlock->z + 2*CUBE_SIZE == block.z;
        case direction_back:
            return tempBlock->z - 2*CUBE_SIZE == block.z;
        }
    }
    
}