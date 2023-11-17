#ifndef GAME_H
#define GAME_H

#include "./constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

struct game {
    int frame;
    int seconds;
    int is_running;
    int is_active;
    float ball_multiplier;
    int last_frame_time;
    int game_state;
    char message[100];
    TTF_Font *font;
    SDL_Window *window;
    SDL_Renderer *renderer;
};

struct game new_game();
int initialize_window(struct game *game);
void destroy_window(struct game *game);

#endif