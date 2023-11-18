#include "entity.h"

entity new_monster() {
    entity newm;
    newm.type = monster;
    newm.orientation.up = FALSE;
    newm.orientation.right = FALSE;
    newm.orientation.down = FALSE;
    newm.orientation.left = FALSE;
    newm.alive = FALSE;
    newm.lives = 10;
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
    newm.cooldown.damage = 0;
    newm.cooldown.attack = 0;
    newm.cooldown.ability = 0;
    newm.animation.texture = "./assets/imgs/imp.png";
    newm.alpha = 255;
    newm.angle = 0;
    return newm;
}

entity new_pod() {
    entity newm;
    newm.type = monster;
    newm.orientation.up = FALSE;
    newm.orientation.right = FALSE;
    newm.orientation.down = FALSE;
    newm.orientation.left = FALSE;
    newm.alive = FALSE;
    newm.lives = 50;
    newm.position.x = WINDOW_WIDTH/2;
    newm.position.y = WINDOW_HEIGHT/2;
    newm.position.w = 64;
    newm.position.h = 64;
    newm.texture.x = 0;
    newm.texture.y = 0;
    newm.texture.w = 64;
    newm.texture.h = 64;   
    newm.animation.frames = 100;
    newm.animation.init = 0;
    newm.animation.sprites = 3;
    newm.cooldown.damage = 0;
    newm.cooldown.attack = 0;
    newm.cooldown.ability = 0;
    newm.animation.texture = "./assets/imgs/pod.png";
    newm.alpha = 255;
    newm.angle = 0;
    return newm;
}

entity new_player() {
    entity newp;
    newp.type = player;
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
    newp.cooldown.damage = 0;
    newp.cooldown.attack = 0;
    newp.cooldown.ability = 0;
    newp.alpha = 255;
    newp.angle = 0;
    return newp;
}

entity new_attack() {
    entity newp;
    newp.type = attack;
    newp.orientation.up = FALSE;
    newp.orientation.down = FALSE;
    newp.orientation.left = FALSE;
    newp.orientation.right = FALSE;
    newp.texture.x = 0;
    newp.texture.y = 0;
    newp.texture.w = 64;
    newp.texture.h = 64;
    newp.alive = TRUE;
    newp.lives = 10;
    newp.animation.frames = 100;
    newp.animation.init = 0;
    newp.animation.sprites = 3;
    newp.animation.texture = "./assets/imgs/attack.png";
    newp.cooldown.damage = 0;
    newp.cooldown.attack = 0;
    newp.cooldown.ability = 0;
    newp.alpha = 255;
    newp.angle = 0;
    return newp;
}

entity new_background() {
    entity newp;
    newp.type = background;
    newp.texture.x = 0;
    newp.texture.y = 0;
    newp.texture.w = 1280;
    newp.texture.h = 720;
    newp.position.x = 0;
    newp.position.y = 0;
    newp.position.w = 1280;
    newp.position.h = 720;
    newp.animation.texture = "./assets/imgs/back.png";
    newp.alpha = 255;
    newp.angle = 0;
    return newp;
}
