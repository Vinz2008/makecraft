#include <stdio.h>
#include <stdlib.h>
#include "block.h"

void serialize_block_array(BlockArray* blockArray, char* filename){
    FILE* f = fopen(filename, "wb");
    fwrite(blockArray, sizeof(blockArray), 1, f);
    fclose(f);
}

BlockArray* deserialize_block_array(char* filename){
    FILE* f = fopen(filename, "rb");
    BlockArray* blockArray = malloc(sizeof(BlockArray));
    fread(blockArray, sizeof(blockArray), 1, f);
    fclose(f);
    return blockArray;
}