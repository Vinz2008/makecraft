#include "block.h"

#ifdef __cplusplus
extern "C" {
#endif

void tpl_serialize_block_array(BlockArray* blockArray, const char* filename);
BlockArray* tpl_deserialize_block_array(const char* filename);

#ifdef __cplusplus
}
#endif
