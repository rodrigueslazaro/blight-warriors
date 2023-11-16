#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>
#include "./constants.h"

struct entity {
    int up;
    int right;
    int down;
    int left;
    int attack;
    int alive;
    int lives;
    char *texture_name;
    SDL_Rect position;
    SDL_Rect texture;
};

struct entity newMonster() {
    struct entity newM;
    newM.up = FALSE;
    newM.right = FALSE;
    newM.down = FALSE;
    newM.left = FALSE;
    newM.attack = FALSE;
    newM.alive = FALSE;
    newM.lives = 3;
    newM.position.x = 0;
    newM.position.y = 0;
    newM.position.w = 64;
    newM.position.h = 64;
    newM.texture.x = 0;
    newM.texture.y = 0;
    newM.texture.w = 64;
    newM.texture.h = 64;   

    return newM;
}