#include "update.h"

void update_monster_tracking(entity *player, entity monsters[], float delta_time) {
    int seconds = SDL_GetTicks() / 250;
    int sprite = seconds % 3;
    for (int i=0; i<MONSTER_CAP; i++) {
        if (monsters[i].alive == TRUE) {
            monsters[i].texture.x = sprite * 64;
            monsters[i].orientation.up = FALSE;
            monsters[i].orientation.right = FALSE;
            monsters[i].orientation.down = FALSE;
            monsters[i].orientation.left = FALSE;
            int pos_increment = PLAYER_SPEED/2*delta_time;
            if (player->position.x >= monsters[i].position.x) {
                monsters[i].orientation.right = TRUE;
                monsters[i].position.x += pos_increment;
            }
            if (player->position.x < monsters[i].position.x) {
                monsters[i].orientation.left = TRUE;
                monsters[i].position.x -= pos_increment;
            }
            if (player->position.y >= monsters[i].position.y) {
                monsters[i].orientation.down = TRUE;
                monsters[i].position.y += pos_increment;
            }
            if (player->position.y < monsters[i].position.y) {
                monsters[i].orientation.up = TRUE;
                monsters[i].position.y -= pos_increment;
            }   
        }
    }
}

void await_frames(game *game) {
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - game->last_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
        SDL_Delay(time_to_wait);
        game->frame++;
        game->seconds++;
        if (game->frame >= 100)
            game->frame = 0;
    }
    game->delta_time = (SDL_GetTicks() - game->last_frame_time) / 1000.0;
}

void update_player_movement(entity *player, game *game) {
    if (player->orientation.up &&
        !player->orientation.right &&
        !player->orientation.down &&
        !player->orientation.left)
        player->position.y -= PLAYER_SPEED * game->delta_time;
    if (!player->orientation.up &&
        player->orientation.right &&
        !player->orientation.down &&
        !player->orientation.left)
        player->position.x += PLAYER_SPEED * game->delta_time;
    if (!player->orientation.up &&
        !player->orientation.right &&
        player->orientation.down &&
        !player->orientation.left)
        player->position.y += PLAYER_SPEED * game->delta_time;
    if (!player->orientation.up &&
        !player->orientation.right &&
        !player->orientation.down &&
        player->orientation.left)
        player->position.x -= PLAYER_SPEED * game->delta_time;
    if (player->orientation.up &&
        player->orientation.right &&
        !player->orientation.down &&
        !player->orientation.left) {
        player->position.y -= PLAYER_SPEED_D * game->delta_time;
        player->position.x += PLAYER_SPEED_D * game->delta_time;
    }
    if (player->orientation.up &&
        !player->orientation.right &&
        !player->orientation.down &&
        player->orientation.left) {
        player->position.y -= PLAYER_SPEED_D * game->delta_time;
        player->position.x -= PLAYER_SPEED_D * game->delta_time;
    }
    if (!player->orientation.up &&
        player->orientation.right &&
        player->orientation.down &&
        !player->orientation.left) {
        player->position.y += PLAYER_SPEED_D * game->delta_time;
        player->position.x += PLAYER_SPEED_D * game->delta_time;
    }
    if (!player->orientation.up &&
        !player->orientation.right &&
        player->orientation.down &&
        player->orientation.left) {
        player->position.y += PLAYER_SPEED_D * game->delta_time;
        player->position.x -= PLAYER_SPEED_D * game->delta_time;
    }
}

void update_player_attack(entity *player, game *game, entity monsters[]) {
    if (game->frame > 15)
        game->frame = 0;
    if (game->frame <= 2) {
        for (int i=0; i<MONSTER_CAP; i++) {
            if (monsters[i].alive) {
                check_monster_hit(&monsters[i], player);
                if (monsters[i].lives <= 0) {
                    monsters[i].alive = FALSE;
                }
            }
        }
    }
}

void update_attack_animation(entity *player, entity *attack) {
    if (player->orientation.up) {
        attack->orientation.up = TRUE;
        attack->position.x = player->position.x;
        attack->position.y = player->position.y - 20;
    }
    if (player->orientation.down) {
        attack->orientation.down = TRUE;
        attack->position.x = player->position.x;
        attack->position.y = player->position.y + 50;
    }
    if (player->orientation.left) {
        attack->orientation.left = TRUE;
        attack->position.x = player->position.x - 30;
        attack->position.y = player->position.y + 10;
    }
    if (player->orientation.right) {
        attack->orientation.right = TRUE;
        attack->position.x = player->position.x + 30;
        attack->position.y = player->position.y + 10;
    }
    if (player->cooldown.attack > 10) {
        attack->texture.x = 0;
    } else if (player->cooldown.attack > 5) {
        attack->texture.x = 64;
    } else if (player->cooldown.attack > 0) {
        attack->texture.x = 128;
    } else {
        attack->orientation.up = FALSE;
        attack->orientation.right = FALSE;
        attack->orientation.down = FALSE;
        attack->orientation.left = FALSE;
    }
}

void update_player_actions(entity *player, entity *attack, game *game, entity monsters[]) {
    if (player->cooldown.damage > 0) {
        player->cooldown.damage--;
    }
    if (player->cooldown.attack > 0) {
        player->cooldown.attack--;
        update_player_attack(player, game, monsters);
        update_attack_animation(player, attack);
    }
}

void update_player_movement_animation(entity *player) {
    int seconds = SDL_GetTicks() / 250;
    int sprite = seconds % 3;
    if (player->orientation.up ||
        player->orientation.right ||
        player->orientation.down ||
        player->orientation.left) {
        player->texture.y = sprite * 80;
        if (player->orientation.up)
            player->texture.x = 192;
        if (player->orientation.down)
            player->texture.x = 0;
        if (player->orientation.left)
            player->texture.x = 64;
        if (player->orientation.right)
            player->texture.x = 128;
    } else {
        player->texture.y = sprite * 0;
    }
}

void check_player_monster_hit(entity *player, game *game, entity monsters[]) {
    for (int i=0; i<MONSTER_CAP && player->cooldown.damage == 0; i++) {
        if (monsters[i].alive) {
            if (is_inside_rectangle(
                player->position.x+32,
                player->position.y+32,
                monsters[i].position.x,
                monsters[i].position.y,
                monsters[i].position.w,
                monsters[i].position.h
            )) {
                player->lives --;
                player->cooldown.damage = 100;
            }
        }
    }
}

void update(game *game, entity *player, entity *attack, entity monsters[]) {
    await_frames(game);
    if (player->cooldown.attack == 0) {
        update_player_movement(player, game);
        update_player_movement_animation(player);
    }
    update_player_actions(player, attack, game, monsters);
    update_monster_tracking(player , monsters, game->delta_time);
    check_player_monster_hit(player, game, monsters);
}

