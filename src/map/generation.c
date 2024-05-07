#include "../makecraft.h"

int get_block_type(int height){
    if (height < 5 || height > 10){
        return stone_texture;
    }
    return dirt_texture;
}