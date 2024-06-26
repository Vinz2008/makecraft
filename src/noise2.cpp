#include "noise2.h"
#include <cstdio>
#include <optional>
#include <FastNoise/FastNoise.h>

std::optional<FastNoise::SmartNode<>> fnGenerator = std::nullopt;

// TODO : replace blockarray with a system with a list of chunks which some are visible, others not. chunks are list of list of lists of blocks (3d array) and they are generated using the xstart and y start
std::vector<float> generate_noise(int size, int seed, float frequency, int xstart, int ystart){
    if (!fnGenerator.has_value()){
        fnGenerator = std::optional<FastNoise::SmartNode<>>{FastNoise::NewFromEncodedNodeTree( "GQATAMP1KD8NAAQAAAAAAABACQAAAAAAPwAAAAAAAQQAAAAAAAAAQEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=")};
    }
    std::vector<float> noiseOutput(size * size);

    // Generate a size x size x size area of noise
    fnGenerator.value()->GenUniformGrid2D(noiseOutput.data(), xstart, ystart, size, size, frequency, seed);
    //int index = 0;

    FILE* f = fopen("noise2.txt", "w");
    for (int y = 0; y < size; y++){
    for (int x = 0; x < size; x++) {
            fprintf(f, " %f ", noiseOutput[y*size+x]);
    }
    fprintf(f, "\n");
    }
    fclose(f);
    return noiseOutput;
}

void write_noise_to_file(std::vector<float> noiseData, int size, const char* filename){
    FILE* f = fopen(filename, "w");
    for (int y = 0; y < size; y++){
    for (int x = 0; x < size; x++) {
            fprintf(f, " %f ", noiseData[y*size+x]);
    }
    fprintf(f, "\n");
    }
    fclose(f);
}


float get_noise_data(std::vector<float>& noise, int x, int y, int size){
    return noise[y*size+x];
}