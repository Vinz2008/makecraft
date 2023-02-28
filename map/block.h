#include <stddef.h>
#include "raylib.h"
#include "raymath.h"

#define CUBE_SIZE 2.0f

#pragma once

typedef struct {
    float x;
    float y;
    float z;
    int material;
}Block;

typedef struct {
    Block* blockArray;
    size_t used;
    size_t size;
}BlockArray;

void initBlockArray(BlockArray* blockArray, size_t initalSize);
void addToBlockArray(BlockArray* blockArray, Block block);
void emptyBlockArray(BlockArray* blockArray);

Block*createBlock(BlockArray* blockArray, float x, float y, float z);