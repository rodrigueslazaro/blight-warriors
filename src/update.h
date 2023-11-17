#ifndef UPDATE_H
#define UPDATE_H

#include "game.h"
#include "entity.h"
#include "collision.h"
#include "renderer.h"

void await_frames(game *game);
void update_player_movement(entity *player, game *game);
void update_player_attack(entity *player, game *game, entity monsters[], entity pods[]);
void update_attack_animation(entity *player, entity *attack);
void update_player_actions(entity *player, entity *attack, game *game, entity monsters[], entity pods[]);
void update_player_movement_animation(entity *player);
void check_player_monster_hit(entity *player, game *game, entity monsters[]);
void update_monster_tracking(entity *player, entity monsters[], float delta_time);
void update(game *game, entity *player, entity *attack, entity monsters[], entity pods[]);

#endif