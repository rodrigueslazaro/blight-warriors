#include <SDL2/SDL.h>
#include "./constants.h"

struct entity {
    int up;
    int down;
    int left;
    int right;
    int attack;
    int alive;
    int lives;
    char *texture_name;
    SDL_Rect position;
    SDL_Rect texture;
};