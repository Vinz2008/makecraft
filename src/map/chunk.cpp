#include "chunk.h"
#include "block.h"
#include "../makecraft.h"
#include "../utils/list.h"
#include "../noise2.h"
#include "../makecraft.h"


struct Chunk createChunk(float x, float y){
    return (struct Chunk){ .x = x, .y = y};
}

struct Chunk get_new_perlin_chunk(float x, float y){
    std::vector<float> noise_data = generate_noise(CHUNK_SIZE*CHUNK_SIZE, NOISE_SEED, NOISE_FREQUENCY, x, y);
    struct ChunkBlocks chunkBlocks /*= {0}*/;
    for (int x_chunk = 0; x_chunk < CHUNK_SIZE; x_chunk++){
        for (int y_chunk = 0; y_chunk < CHUNK_SIZE; y_chunk++){
            int block_height = round(get_noise_data(noise_data, x_chunk, y_chunk, CHUNK_SIZE));
            chunkBlocks.blocks[block_height][x_chunk][y_chunk] = Block {.x = x+x_chunk, .y=y+y_chunk, .z = block_height};
        }
    }
    // TODO : generate the blocks under the surface (the firsts will be like the upper one, then stones and then maybe some minerals)
    return Chunk {.blocks = chunkBlocks, .x = x, .y = y};
}

list_t* get_new_perlin_chunk_list(){
    list_t* chunk_list = list_create();
    for (int x = -RENDER_DISTANCE; x < RENDER_DISTANCE; x++){
        for (int y = -RENDER_DISTANCE; y < RENDER_DISTANCE; y++){
            Chunk* chunk_temp = new Chunk(get_new_perlin_chunk(x*CHUNK_SIZE, y*CHUNK_SIZE));
            list_append(chunk_temp, chunk_list);
        }
    }
    return chunk_list;
}

void destroy_chunk_list(list_t* chunk_list){
    for (int i = 0; i < chunk_list->used; i++){
        delete (Chunk*)chunk_list->list[i].data; 
    }
    list_destroy(chunk_list);
}