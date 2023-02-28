#include <stdlib.h>
#include <stdio.h>

float** create_noise(){
    /*fnl_state noise = fnlCreateState();
    noise.noise_type = FNL_NOISE_OPENSIMPLEX2;*/
    float** noiseData = malloc(128 * 128 * sizeof(float));
    for (int y = 0; y < 128; y++){
    for (int x = 0; x < 128; x++) {
        //noiseData[y][x] = fnlGetNoise2D(&noise, x, y);
    }
    }
    return noiseData;
}

void write_noise_to_file(float** noiseData, char* filename){
    FILE* f = fopen(filename, "w");
    for (int y = 0; y < 128; y++){
    for (int x = 0; x < 128; x++) {
        //fprintf(f, "%f", noiseData[y][x]);
    }
    fprintf(f, "\n");
    }
    fclose(f);
}