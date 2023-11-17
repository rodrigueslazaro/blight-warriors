#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>
#include "constants.h"
#include "game.h"
#include "entity.h"
#include "renderer.h"
#include "update.h"
#include "input.h"

int main() {
    game game = new_game();
    game.is_running = initialize_window(&game);
    game.font = TTF_OpenFont("./assets/fonts/bigblue.ttf", 24); // You need a .ttf file for your font

    entity player = new_player();
    entity attack = new_attack();
    entity background = new_background();
    entity monsters[MONSTER_CAP];
    for (int i=0; i<MONSTER_CAP; i++)
        monsters[i] = new_monster();
    entity pods[MONSTER_CAP];
    for (int i=0; i<MONSTER_CAP; i++)
        pods[i] = new_pod();
    char str[10];
    pods[0].alive = TRUE;

    while (game.is_running) {
        process_input(&game, &player);
        switch (game.game_state) {
            case 0:
                strcpy(game.message, "Welcome to the Blight!");
                break;
            case 1:
                sprintf(str, "%d", player.lives);
                strcpy(game.message, str);
                game.last_frame_time = SDL_GetTicks(); // time since game began
                update(&game, &player, &attack, monsters, pods);
                break;
            case 2:
                strcpy(game.message, "[PAUSED]");
                break;
        }
        render(game, player, attack, monsters, pods, background);
    }

    destroy_window(&game);

    return 0;
}