#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "block.h"

void serialize_struct(void* structName, size_t size, char* filename){
    FILE* f = fopen(filename, "wb");
    fwrite(structName, size, 1, f);
    fclose(f);
}

void serialize_array(void* arrayName, size_t sizeMember, size_t nbMember, char* filename){
    FILE* f = fopen(filename, "wb");
    fwrite(arrayName, sizeMember, nbMember, f);
    fclose(f);
}

void serialize_block_array(BlockArray* blockArray, char* filename){
    serialize_array(blockArray->blockArray, sizeof(Block), blockArray->used, filename);
}

BlockArray* deserialize_block_array(char* filename){
    FILE* f = fopen(filename, "rb");
    BlockArray* blockArray = malloc(sizeof(BlockArray));
    int fd = fileno(f);
    struct stat buf;
    fstat(fd, &buf);
    off_t size = buf.st_size;
    fread(blockArray->blockArray, sizeof(Block), size/sizeof(Block), f);
    fclose(f);
    return blockArray;
}