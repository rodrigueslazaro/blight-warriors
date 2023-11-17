#ifndef INPUT_H
#define INPUT_H

#include "game.h"
#include "entity.h"

void read_ui_interactions(SDL_Event event, struct game *game);
void read_player_interactions(SDL_Event event, struct entity *player);
void process_input(struct game *game, struct entity *player);
void update_player_action(SDL_Keycode key, int state, struct entity *player);

#endif