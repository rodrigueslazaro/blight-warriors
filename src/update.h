#ifndef UPDATE_H
#define UPDATE_H

#include "game.h"
#include "entity.h"
#include "collision.h"
#include "renderer.h"

void await_frames(game *game);
void update_player_movement(entity *player, game *game);
void update_player_attack(entity *player, game *game, entity monsters[]);
void update_player_actions(entity *player, game *game, entity monsters[]);
void update_monster_tracking(entity *player, entity monsters[], int i, int sprite, float delta_time);
void update(game *game, entity *player, entity monsters[]);

#endif