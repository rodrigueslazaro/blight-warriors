#include "input.h"

void read_ui_interactions(SDL_Event event, game *game) {
    if (event.type == SDL_QUIT ||
       (event.type == SDL_KEYDOWN &&
        event.key.keysym.sym == SDLK_ESCAPE))
        game->is_running = FALSE;
    if (event.type == SDL_KEYDOWN &&
        event.key.keysym.sym == SDLK_SPACE) {
            switch (game->game_state) {
                case 0:
                    game->game_state = 1;
                    break;
                case 1:
                    game->game_state = 2;
                    break;
                case 2:
                    game->game_state = 1;
                    break;
            }
    }
}

void read_player_interactions(SDL_Event event, entity *player) {
    SDL_Keycode key = event.key.keysym.sym;
    if (event.type == SDL_KEYDOWN) {
        update_player_action(key, TRUE, player);
    }
    if (event.type == SDL_KEYUP)
        update_player_action(key, FALSE, player);
}

void process_input(game *game, entity *player) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        read_ui_interactions(event, game);
        read_player_interactions(event, player);
    } 
}


void update_player_action(SDL_Keycode key, int state, entity *player) {
    switch (key) {
        case SDLK_w:
            player->orientation.up = state;
            break;
        case SDLK_d:
            player->orientation.right = state;
            break;
        case SDLK_s:
            player->orientation.down = state;
            break;
        case SDLK_a:
            player->orientation.left = state;
            break;
        case SDLK_j:
            player->attack = TRUE;
            break;
    }
}

