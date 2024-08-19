#include "block.h"

#ifdef __cplusplus
extern "C" {
#endif

void tpl_serialize_block_array(std::vector<Block> blockArray, const char* filename);
std::vector<Block> tpl_deserialize_block_array(const char* filename);

#ifdef __cplusplus
}
#endif
