#ifndef COLLISION_H
#define COLLISION_H

#include "entity.h"

void check_monster_hit(struct entity *monster, struct entity *player);
int is_inside_rectangle(int a, int b, int x, int y, int w, int h);
int is_not_colliding(struct entity monsters[], int i, int pos_increment);

#endif