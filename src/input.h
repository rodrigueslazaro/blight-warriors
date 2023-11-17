#ifndef INPUT_H
#define INPUT_H

#include "game.h"
#include "entity.h"

void read_ui_interactions(SDL_Event event, game *game);
void read_player_interactions(SDL_Event event, entity *player);
void process_input(game *game, entity *player);
void update_player_action(SDL_Keycode key, int state, entity *player);

#endif