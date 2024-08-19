#ifndef MAKECRAFT_HEADER

#define MAKECRAFT_HEADER

enum material {
    invisible = 0,
    water_texture = -1,
    dirt_texture = -2,
    stone_texture = -3,
    empty_texture = -4,
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