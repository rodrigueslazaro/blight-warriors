#ifndef ENTITY_H
#define ENTITY_H

#include "./constants.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int up;
    int right;
    int down;
    int left;
} orientation;

typedef struct {
    int frames; // how many frames the animation takes
    int init; // when the animation should start
    int sprites; // number of animation frames in texture
    char *texture;
} animation;

typedef struct {
    orientation orientation;
    animation animation;
    int attack;
    int alive;
    int lives;
    SDL_Rect position;
    SDL_Rect texture;
} entity;

entity new_monster();
entity new_player();

#endif