#ifndef UPDATE_H
#define UPDATE_H

#include "game.h"
#include "entity.h"
#include "collision.h"

void update_monster_tracking(struct entity *player, struct entity monsters[], int i, int sprite, float delta_time);
void update(struct game *game, struct entity *player, struct entity monsters[]);

#endif