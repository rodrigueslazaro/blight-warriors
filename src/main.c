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
    srand(time(NULL));
    int randomNumber = rand() % 3;
    entity monsters[MONSTER_CAP];
    for (int i=0; i<MONSTER_CAP; i++) {
        randomNumber = rand() % 4;
        monsters[i] = new_monster();
        if (randomNumber == 0)
            monsters[i].animation.texture = "./assets/imgs/imp.png";
        if (randomNumber == 1)
            monsters[i].animation.texture = "./assets/imgs/frog.png";
        if (randomNumber == 2)
            monsters[i].animation.texture = "./assets/imgs/ghost.png";
        if (randomNumber == 3)
            monsters[i].animation.texture = "./assets/imgs/skeleton.png";
    }
    for (int i=0; i<20; i++) {
        int randw = rand() % WINDOW_WIDTH + 1;
        int randh = rand() % WINDOW_HEIGHT + 1;
        monsters[i].alive = TRUE;
        monsters[i].position.x = randw;
        monsters[i].position.y = randh;
    }
    entity player = new_player();
    char str[10];

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
                update(&game, &player, monsters);
                break;
            case 2:
                strcpy(game.message, "[PAUSED]");
                break;
        }
        render(&game, player, monsters);
    }

    destroy_window(&game);

    return 0;
}