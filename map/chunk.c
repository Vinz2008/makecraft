#include "chunk.h"
#include "block.h"
#include "../makecraft.h"

void createChunk(BlockArray* blockArray, float x, float y, float z){
    int i;
    int i2;
    for (i2 = 0; i2 < CHUNK_LENGTH * 2; i2 = i2 + 2) {
        for (i = 0; i < CHUNK_LENGTH * 2; i = i + 2) {
            //__uint16_t noise_gen = PerlinNoise2D(1, 1, 1, 1, 1,1 );
            //printf("noise : %d\n", noise_gen);
            int temp = 1;
            //for (int temp = 1; temp < noise_gen/10000; temp++){
            /*DrawCube((Vector3){x + (float)i , y * temp, z + (float)i2}, 2.0f, 2.0f, 2.0f, RED);
            DrawCubeWires((Vector3){x + (float)i, y * temp, z + (float)i2}, 2.0f, 2.0f, 2.0f, BLACK);
            DrawCubeTexture(DirtTexture, (Vector3){x + (float)i, y * temp, z + (float)i2},2.0f,2.0f,2.0f ,WHITE);*/
            Block tempBlock;
            tempBlock.x = x + (float)i;
            tempBlock.y = y * temp;
            tempBlock.z = z + (float)i2;
            createBlock(blockArray, x + (float)i, y * temp, z + (float)i2, dirt_texture);
            addToBlockArray(blockArray, tempBlock);
            //DrawBoundingBox(PlayerHitBox, BLACK);
            //DrawLine3D(PlayerPosition,PlayerPositionFloor, BLACK);
            //}        
        }
    }
}