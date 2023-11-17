#ifndef ENTITY_H
#define ENTITY_H

#include "./constants.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

struct orientation {
    int up;
    int right;
    int down;
    int left;
};

struct animation {
    int frames; // how many frames the animation takes
    int init; // when the animation should start
    int sprites; // number of animation frames in texture
    char *texture;
};

struct entity {
    struct orientation orientation;
    struct animation animation;
    int attack;
    int alive;
    int lives;
    SDL_Rect position;
    SDL_Rect texture;
};

struct entity new_monster();
struct entity new_player();

#endif