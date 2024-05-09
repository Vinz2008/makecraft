#ifndef MAKECRAFT_HEADER

#define MAKECRAFT_HEADER

enum material {
    water_texture = 0,
    dirt_texture = -1,
    stone_texture = -2,
    empty_texture = -3,
};

enum game_mode {
    creative_mode = -1,
    survival_mode = -2,
};

#define LOG(x) fprintf(fp, x);

#define NUMBER_BUTTON_MENU 3

#define NOISE_SEED 164647
#define NOISE_FREQUENCY 0.05

#endif