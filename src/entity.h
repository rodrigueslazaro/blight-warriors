#ifndef ENTITY_H
#define ENTITY_H

#include "./constants.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

enum entity_type {
    player, monster, attack, background, pod
};

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
    int damage;
    int attack;
    int ability;
} cooldown;

typedef struct {
    orientation orientation;
    animation animation;
    enum entity_type type; 
    int alive;
    int lives;
    int angle;
    int alpha;
    cooldown cooldown;
    SDL_Rect position;
    SDL_Rect texture;
} entity;

entity new_monster();
entity new_player();
entity new_attack();
entity new_background();
entity new_pod();

#endif