#include <stddef.h>

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
#endif

#include "raylib.h"
#include "raymath.h"


#define CUBE_SIZE 2.0f

#pragma once


enum direction {
    direction_top,
    direction_bottom,
    direction_right,
    direction_left,
    direction_front,
    direction_back,
};
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
} BlockArray;

void initBlockArray(BlockArray* blockArray, size_t initalSize);
void addToBlockArray(BlockArray* blockArray, Block block);
void emptyBlockArray(BlockArray* blockArray);
bool isCubeNextToCube(BlockArray* blockArray, int direction, Block block);
BlockArray* removeFromBlockArray(int index, BlockArray* blockArray);
void destroyBlockArray(BlockArray* blockArray);

/*Block**/ void createBlock(BlockArray* blockArray, float x, float y, float z, int texture);
bool isCubeNextToCube(BlockArray* blockArray, int direction, Block block);

#ifdef __cplusplus
}
#endif