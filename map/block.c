#include "block.h"
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "chunk.h"

extern Texture2D DirtTexture;

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

Block* createBlock(BlockArray* blockArray, float x, float y, float z){
    Block* tempBlock = malloc(sizeof(tempBlock));
    tempBlock->x = x;
    tempBlock->y = y;
    tempBlock->z = z;
    addToBlockArray(blockArray, *tempBlock);
    //Model tempCube = LoadModelFromMesh(GenMeshCube(CUBE_SIZE, CUBE_SIZE, CUBE_SIZE));
    //DrawModel(tempCube, (Vector3){x, y, z}, 1,RED);
    DrawCube((Vector3){x, y, z}, CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, RED);
    DrawCubeWires((Vector3){x, y, z}, CUBE_SIZE, CUBE_SIZE, CUBE_SIZE, BLACK);
    DrawCubeTexture(DirtTexture, (Vector3){x, y  , z }, CUBE_SIZE , CUBE_SIZE , CUBE_SIZE , WHITE);
    return tempBlock;
}
