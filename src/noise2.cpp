#include <cstdio>
#include <vector>
#include <FastNoise/FastNoise.h>

std::vector<float> generate_noise(int size, int seed, float frequency){
    FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree( "GQATAMP1KD8NAAQAAAAAAABACQAAAAAAPwAAAAAAAQQAAAAAAAAAQEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=" );
    std::vector<float> noiseOutput(size * size);

    // Generate a size x size x size area of noise
    fnGenerator->GenUniformGrid2D(noiseOutput.data(), 0, 0, size, size, frequency, seed);
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



float get_noise_data(std::vector<float> noise, int x, int y, int size){
    return noise[y*size+x];
}

float* generate_noise3d(int size, int seed){
    FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree( "GQATAMP1KD8NAAQAAAAAAABACQAAAAAAPwAAAAAAAQQAAAAAAAAAQEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=" );
    std::vector<float> noiseOutput(size * size * size);

    // Generate a size x size x size area of noise
    fnGenerator->GenUniformGrid3D(noiseOutput.data(), 0, 0, 0, size, size, size, 0.2f, seed);
    /*int index = 0;
    for (int z = 0; z < size; z++){
    for (int y = 0; y < size; y++){
        for (int x = 0; x < size; x++){
            ProcessVoxelData(x, y, z, noiseOutput[index++]);			
        }			
    }
    }*/
    float* a = &noiseOutput[0];
    return a;
}

void write_noise_to_file3d(float* noiseData, int size, char* filename){
    FILE* f = fopen(filename, "w");
    for (int y = 0; y < size; y++){
    for (int x = 0; x < size; x++) {
        for (int z = 0; z < size; z++){
            fprintf(f, " %f ", noiseData[(y*size+x)*size+z]);
        }
        fprintf(f, "\n");
    }
    fprintf(f, "\n");
    }
    fclose(f);
}