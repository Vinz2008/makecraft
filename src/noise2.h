#define NB_BLOCK_NOISE 100

std::vector<float> generate_noise(int size, int seed,float frequency);
void write_noise_to_file(std::vector<float> noiseData, int size, const char* filename);

float get_noise_data(std::vector<float> noise, int x, int y, int size);