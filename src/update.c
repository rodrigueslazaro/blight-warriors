#include "update.h"

void update_monster_tracking(entity *player, entity monsters[], int i, int sprite, float delta_time) {
    monsters[i].texture.x = sprite * 64;
    // monster->texture.x = sprite * 0;
    monsters[i].orientation.up = FALSE;
    monsters[i].orientation.right = FALSE;
    monsters[i].orientation.down = FALSE;
    monsters[i].orientation.left = FALSE;
    int pos_increment = PLAYER_SPEED/2*delta_time;
    if (player->position.x >= monsters[i].position.x && is_not_colliding(monsters, i, pos_increment)) {
        monsters[i].orientation.right = TRUE;
        monsters[i].position.x += pos_increment;
    }
    if (player->position.x < monsters[i].position.x && is_not_colliding(monsters, i, -pos_increment)) {
        monsters[i].orientation.left = TRUE;
        monsters[i].position.x -= pos_increment;
    }
    if (player->position.y >= monsters[i].position.y && is_not_colliding(monsters, i, pos_increment)) {
        monsters[i].orientation.down = TRUE;
        monsters[i].position.y += pos_increment;
    }
    if (player->position.y < monsters[i].position.y && is_not_colliding(monsters, i, -pos_increment)) {
        monsters[i].orientation.up = TRUE;
        monsters[i].position.y -= pos_increment;
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
    if (player->orientation.up)
        player->position.y -= PLAYER_SPEED * game->delta_time;
    if (player->orientation.down)
        player->position.y += PLAYER_SPEED * game->delta_time;
    if (player->orientation.left)
        player->position.x -= PLAYER_SPEED * game->delta_time;
    if (player->orientation.right)
        player->position.x += PLAYER_SPEED * game->delta_time;
}

void update_player_attack(entity *player, game *game, entity monsters[]) {
    if (player->attack && (player->orientation.up ^ player->orientation.down ^ player->orientation.left ^ player->orientation.right)) {
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
        if (game->frame >= 15 && game->frame < 20)
            player->attack = FALSE;
    }
}

void update_player_actions(entity *player, game *game, entity monsters[]) {
    update_player_attack(player, game, monsters);
}

void update(game *game, entity *player, entity monsters[]) {
    await_frames(game);
    update_player_movement(player, game);
    update_player_actions(player, game, monsters);

    int seconds = SDL_GetTicks() / 250;
    int sprite = seconds % 3;
    if (player->orientation.up || player->orientation.right || player->orientation.down || player->orientation.left) {
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
    for (int i=0; i<MONSTER_CAP; i++) {
        if (monsters[i].alive == TRUE)
            update_monster_tracking(player , monsters, i, sprite, game->delta_time);
    }

    if (game->frame <= 0) {

    for (int i=0; i<MONSTER_CAP; i++) {
        if (is_inside_rectangle(
            player->position.x+32,
            player->position.y+32,
            monsters[i].position.x,
            monsters[i].position.y,
            monsters[i].position.w,
            monsters[i].position.h
        )) {
            player->lives --;
        }
    }
    }
}