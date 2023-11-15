#include <SDL2/SDL.h>

struct player {
    float x;
    float y;
    float w;
    float h;
    int up;
    int down;
    int left;
    int right;
    int attack;
    SDL_Rect p;
    SDL_Rect s;
};