#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tpl.h"
#include "block.h"

void bin_serialize_struct(void* structName, size_t size, char* filename){
    FILE* f = fopen(filename, "wb");
    fwrite(structName, size, 1, f);
    fclose(f);
}

void bin_serialize_array(void* arrayName, size_t sizeMember, size_t nbMember, char* filename){
    FILE* f = fopen(filename, "wb");
    fwrite(arrayName, sizeMember, nbMember, f);
    fclose(f);
}

void bin_serialize_block_array(BlockArray* blockArray, char* filename){
    serialize_array(blockArray->blockArray, sizeof(Block), blockArray->used, filename);
}

BlockArray* bin_deserialize_block_array(char* filename){
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

void tpl_serialize_block_array(BlockArray* blockArray, char* filename){
    tpl_node *tn;
    Block tmp;
    tn = tpl_map("A(S(fffi))", &tmp);
    for (int i = 0; i < blockArray->used; i++){
        tmp = blockArray->blockArray[i];
        tpl_pack(tn, 1);
    }
    tpl_dump(tn, TPL_FILE, filename);
    tpl_free(tn);
}

BlockArray* tpl_deserialize_block_array(char* filename){
    tpl_node *tn;
    Block tmp;
    tn = tpl_map("A(S(fffi))", &tmp);
    BlockArray* blockArray = malloc(sizeof(BlockArray));
    initBlockArray(blockArray, 1);
    tpl_load(tn, TPL_FILE, filename);
    while( tpl_unpack(tn,1) > 0) {
        Block newBlock = tmp;
        printf("newBlock = x : %f, y : %f, z : %f, material : %d\n", newBlock.x, newBlock.y, newBlock.z, newBlock.material);
        addToBlockArray(blockArray, newBlock);
    }
    tpl_free(tn);
    return blockArray;
}

void serialize_struct(void* structName, size_t size, char* filename){
    bin_serialize_struct(structName, size, filename);
}

void serialize_array(void* arrayName, size_t sizeMember, size_t nbMember, char* filename){
    bin_serialize_array(arrayName, sizeMember, nbMember, filename);
}

void serialize_block_array(BlockArray* blockArray, char* filename){
    bin_serialize_block_array(blockArray, filename);
}

BlockArray* deserialize_block_array(char* filename){
    return bin_deserialize_block_array(filename);
}