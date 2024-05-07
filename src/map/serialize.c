#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tpl.h"
#include "block.h"
#include "serialize.h"


void tpl_serialize_chunk_array(){
    tpl_node *tn;
    tn = tpl_map("A(S(A(S(fffi)fff))");
}

void tpl_serialize_block_array(BlockArray* blockArray, const char* filename){
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

BlockArray* tpl_deserialize_block_array(const char* filename){
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