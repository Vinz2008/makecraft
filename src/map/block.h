#include <stddef.h>

#include "raylib.h"
#include "raymath.h"

#include <vector>

#include "../makecraft.h"


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
    enum material material;
}Block;


bool isCubeNextToCube(std::vector<Block>& blockArray, int direction, Block block);

void drawBlock(float x, float y, float z, enum material texture);
