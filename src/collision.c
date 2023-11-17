#include "collision.h"

void check_monster_hit(struct entity *monster, struct entity *player) {
    if (player->orientation.left &&
        monster->position.x+32 >= player->position.x-64 &&
        monster->position.y+32 >= player->position.y &&
        monster->position.x+32 <= player->position.x &&
        monster->position.y+32 <= player->position.y+80) {
        monster->lives--;
        monster->position.x -= 20;
    }
    if (player->orientation.right &&
        monster->position.x+32 >= player->position.x+64 &&
        monster->position.y+32 >= player->position.y &&
        monster->position.x+32 <= player->position.x+128 &&
        monster->position.y+32 <= player->position.y+80) {
        monster->lives--;
        monster->position.x += 20;
    }
    if (player->orientation.up &&
        monster->position.x+32 >= player->position.x &&
        monster->position.y+32 >= player->position.y-80 &&
        monster->position.x+32 <= player->position.x+64 &&
        monster->position.y+32 <= player->position.y) {
        monster->lives--;
        monster->position.y -= 20;
    }
    if (player->orientation.down &&
        monster->position.x+32 >= player->position.x &&
        monster->position.y+32 >= player->position.y+80 &&
        monster->position.x+32 <= player->position.x+64 &&
        monster->position.y+32 <= player->position.y+160) {
        monster->lives--;
        monster->position.y += 20;
    }
}

int is_inside_rectangle(int a, int b, int x, int y, int w, int h) {
    if (a >= x && a <= x + w && b >= y && b <= y + h)
        return TRUE;
    return FALSE;
}

int is_not_colliding(struct entity monsters[], int i, int pos_increment) {
    int x = monsters[i].position.x+32 + pos_increment;
    int y = monsters[i].position.y+32 + pos_increment;
    for (int j=0; j<MONSTER_CAP; j++) {
        if (j != i && monsters[j].alive) {
        if (is_inside_rectangle(x, y,
                                monsters[j].position.x,
                                monsters[j].position.y,
                                monsters[j].position.w,
                                monsters[j].position.h))
            return FALSE;
        }
    }
    return TRUE;
}