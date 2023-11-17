#include "entity.h"

entity new_monster() {
    entity newm;
    newm.orientation.up = FALSE;
    newm.orientation.right = FALSE;
    newm.orientation.down = FALSE;
    newm.orientation.left = FALSE;
    newm.attack = FALSE;
    newm.alive = FALSE;
    newm.lives = 3;
    newm.position.x = 0;
    newm.position.y = 0;
    newm.position.w = 64;
    newm.position.h = 64;
    newm.texture.x = 0;
    newm.texture.y = 0;
    newm.texture.w = 64;
    newm.texture.h = 64;   
    newm.animation.frames = 100;
    newm.animation.init = 0;
    newm.animation.sprites = 3;
    return newm;
}

entity new_player() {
    entity newp;
    newp.orientation.up = FALSE;
    newp.orientation.down = FALSE;
    newp.orientation.left = FALSE;
    newp.orientation.right = FALSE;
    newp.position.x = 50;
    newp.position.y = 50;
    newp.position.w = 64;
    newp.position.h = 80;
    newp.texture.x = 0;
    newp.texture.y = 0;
    newp.texture.w = 64;
    newp.texture.h = 80;
    newp.alive = TRUE;
    newp.lives = 10;
    newp.animation.frames = 100;
    newp.animation.init = 0;
    newp.animation.sprites = 3;
    newp.animation.texture = "./assets/imgs/player.png";
    return newp;
}

