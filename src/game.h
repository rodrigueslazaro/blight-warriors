#ifndef GAME_H
#define GAME_H

#include "./constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct {
    int frame;
    float delta_time;
    int seconds;
    int is_running;
    int is_active;
    int last_frame_time;
    int game_state;
    char message[100];
    TTF_Font *font;
    SDL_Window *window;
    SDL_Renderer *renderer;
} game;

game new_game();
int initialize_window(game *game);
void destroy_window(game *game);

#endif