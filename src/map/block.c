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

extern Texture2D DirtTexture;
extern Texture2D StoneTexture;
extern Texture2D WaterTexture;
extern Texture2D EmptyTexture;

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
    /*for (int i = 0; i < blockArray->used; i++){
        if (i != index){
            printf("TEST in loop\n");
            addToBlockArray(newBlockArray, blockArray->blockArray[posNewArray]);
            posNewArray++;
            printf("TEST in loop 2\n");
        }
    }*/
    free(blockArray);
    return newBlockArray;
}

/*Block**/ void createBlock(BlockArray* blockArray, float x, float y, float z, int texture){
    /*Block* tempBlock = malloc(sizeof(Block));
    tempBlock->x = x;
    tempBlock->y = y;
    tempBlock->z = z;*/
    //addToBlockArray(blockArray, *tempBlock);
    //Model tempCube = LoadModelFromMesh(GenMeshCube(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE));
    //DrawModel(tempCube, (Vector3){x, y, z}, 1,RED);
    //DrawCube((Vector3){x, y, z}, CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, RED);
    //DrawCubeWires((Vector3){x, y, z}, CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, BLACK);
    //Texture2D* texture2dBlock = NULL;
    Texture2D texture2dBlock = getBlockTexture(texture);
    Color col = WHITE;
    DrawCubeTexture(texture2dBlock, (Vector3){x, y, z }, CUBE_SIZE , CUBE_SIZE , CUBE_SIZE , col);
    //return tempBlock;
}


bool isCubeNextToCube(BlockArray* blockArray, int direction, Block block){
    for (int i = 0; i < blockArray->used; i++){
        Block* tempBlock = &blockArray->blockArray[i];
        switch (direction){
        case direction_top:
            //printf("pos 1 y : %f, pos 2 y : %f\n", block.y + CUBE_SIZE, blockArray->blockArray[i].y);
            return tempBlock->y == block.y + CUBE_SIZE;
        case direction_bottom:
            return block.y - CUBE_SIZE == tempBlock->y;
        case direction_right:
            return block.x + CUBE_SIZE == tempBlock->x;
        case direction_left:
            return block.x - CUBE_SIZE == tempBlock->x;
        case direction_front:
            return block.z + CUBE_SIZE == tempBlock->z;
        case direction_back:
            return block.z - CUBE_SIZE == tempBlock->z;
        }
    }
    return false;
    
}