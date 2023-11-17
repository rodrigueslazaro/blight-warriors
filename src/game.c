#include "game.h"

struct game new_game() {
    struct game newG;
    newG.ball_multiplier = 1;
    newG.last_frame_time = 0;
    newG.game_state = 0;
    strcpy(newG.message, "");
    newG.is_active = FALSE;
    newG.font = NULL;
    newG.window = NULL;
    newG.renderer = NULL;
    newG.frame = 0;
    return newG;
};

int initialize_window(struct game *game) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error intializing SDL.\n");
        return FALSE;
    }
    game->window = SDL_CreateWindow(
        NULL,                   // window title
        SDL_WINDOWPOS_CENTERED, // window position x
        SDL_WINDOWPOS_CENTERED, // window position y
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS);
    if (!game->window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return FALSE;
    }
    game->renderer = SDL_CreateRenderer(
        game->window, // window to attach the render
        -1,     // driver code, -1 for default
        0       // special flags
    );
    if (!game->renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return FALSE;
    }
    if (TTF_Init() != 0) {
        printf("TTF initialization failed: %s\n", TTF_GetError());
        return FALSE;
    }
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_VIDEO initialization failed: %s\n", TTF_GetError());
        return FALSE;
    }
    if (SDL_Init(IMG_INIT_PNG) != 0) {
        printf("SDL_VIDEO initialization failed: %s\n", TTF_GetError());
        return FALSE;
    }
    return TRUE;
}

void destroy_window(struct game *game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}