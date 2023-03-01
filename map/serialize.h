#include "block.h"

void serialize_block_array(BlockArray* blockArray, char* filename);
BlockArray* deserialize_block_array(char* filename);
void serialize_struct(void* structName, size_t size, char* filename);
void serialize_array(void* arrayName, size_t sizeMember, size_t nbMember, char* filename);