#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>
#include "./constants.h"
#include "./game.h"
#include "./player.h"
#include "./ball.h"
#include "./darr.h"



int initialize_window(struct game *game) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error intializing SDL.\n");
        return FALSE;
    }
    game->window = SDL_CreateWindow(
        NULL,                   // window title
        SDL_WINDOWPOS_CENTERED, // window position x
        SDL_WINDOWPOS_CENTERED, // window position y
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS);
    if (!game->window) {
        fprintf(stderr, "Error creating SDL Window.\n");
        return FALSE;
    }
    game->renderer = SDL_CreateRenderer(
        game->window, // window to attach the render
        -1,     // driver code, -1 for default
        0       // special flags
    );
    if (!game->renderer) {
        fprintf(stderr, "Error creating SDL Renderer.\n");
        return FALSE;
    }
    if (TTF_Init() != 0) {
        printf("TTF initialization failed: %s\n", TTF_GetError());
        return FALSE;
    }
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_VIDEO initialization failed: %s\n", TTF_GetError());
        return FALSE;
    }
    if (SDL_Init(IMG_INIT_PNG) != 0) {
        printf("SDL_VIDEO initialization failed: %s\n", TTF_GetError());
        return FALSE;
    }
    return TRUE;
}

void render_attack(struct game *game, struct entity *player) {
    SDL_Surface *imageSurface = IMG_Load("./assets/imgs/attack.png");
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(game->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    SDL_Rect dstrect = {player->position.x, player->position.y, 64, 64};
    double angle;
    if (player->up) {
        angle = 0;
        dstrect.y -= 20;
    }
    if (player->down) {
        angle = 180;
        dstrect.y += 50;
    }
    if (player->left) {
        angle = 270;
        dstrect.x -= 30;
        dstrect.y += 10;
    }
    if (player->right) {
        angle = 90;
        dstrect.x += 30;
        dstrect.y += 10;
    }
    SDL_Rect srcrect = {0, 0, 64, 64};
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_RenderCopyEx(game->renderer, imageTexture, &srcrect, &dstrect, angle, NULL, flip);
    SDL_RenderPresent(game->renderer);
    SDL_Delay(50);
    srcrect.x = 64;
    SDL_RenderCopyEx(game->renderer, imageTexture, &srcrect, &dstrect, angle, NULL, flip);
    SDL_RenderPresent(game->renderer);
    SDL_Delay(50);
    srcrect.x = 128;
    SDL_RenderCopyEx(game->renderer, imageTexture, &srcrect, &dstrect, angle, NULL, flip);
    SDL_RenderPresent(game->renderer);
    SDL_Delay(50);
}

void spawn_monster(struct entity *enemy) {
    // int i=0;
    // while (enemies[i].alive == TRUE) {
    //     i++;
    // }
    // enemies[i].alive == TRUE;
    enemy->alive = TRUE;
}

void read_ui_interactions(SDL_Event event, struct game *game) {
    if (event.type == SDL_QUIT ||
       (event.type == SDL_KEYDOWN &&
        event.key.keysym.sym == SDLK_ESCAPE))
        game->is_running = FALSE;
    if (event.type == SDL_KEYDOWN &&
        event.key.keysym.sym == SDLK_SPACE) {
            switch (game->game_state) {
                case 0:
                    game->game_state = 1;
                    break;
                case 1:
                    game->game_state = 2;
                    break;
                case 2:
                    game->game_state = 1;
                    break;
            }
    }
}

void update_player_action(SDL_Keycode key, int state, struct entity *player) {
    switch (key) {
        case SDLK_w:
            player->up = state;
            break;
        case SDLK_d:
            player->right = state;
            break;
        case SDLK_s:
            player->down = state;
            break;
        case SDLK_a:
            player->left = state;
            break;
        case SDLK_j:
            player->attack = state;
            break;
    }
}

void read_player_interactions(SDL_Event event, struct entity *player) {
    SDL_Keycode key = event.key.keysym.sym;
    if (event.type == SDL_KEYDOWN)
        update_player_action(key, TRUE, player);
    if (event.type == SDL_KEYUP)
        update_player_action(key, FALSE, player);
}

void process_input(struct game *game, struct entity *player) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        read_ui_interactions(event, game);
        read_player_interactions(event, player);
    } 
}

void check_monster_hit(struct entity *monster, struct entity *player) {
    if (player->left &&
        monster->position.x+32 >= player->position.x-64 &&
        monster->position.y+32 >= player->position.y &&
        monster->position.x+32 <= player->position.x &&
        monster->position.y+32 <= player->position.y+80) {
        monster->lives--;
        monster->position.x -= 20;
    }
    if (player->right &&
        monster->position.x+32 >= player->position.x+64 &&
        monster->position.y+32 >= player->position.y &&
        monster->position.x+32 <= player->position.x+128 &&
        monster->position.y+32 <= player->position.y+80) {
        monster->lives--;
        monster->position.x += 20;
    }
    if (player->up &&
        monster->position.x+32 >= player->position.x &&
        monster->position.y+32 >= player->position.y-80 &&
        monster->position.x+32 <= player->position.x+64 &&
        monster->position.y+32 <= player->position.y) {
        monster->lives--;
        monster->position.y -= 20;
    }
    if (player->down &&
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

void update_monster_tracking(struct entity *player, struct entity monsters[], int i, int sprite, float delta_time) {
    monsters[i].texture.x = sprite * 64;
    // monster->texture.x = sprite * 0;
    monsters[i].up = FALSE;
    monsters[i].right = FALSE;
    monsters[i].down = FALSE;
    monsters[i].left = FALSE;
    int pos_increment = PLAYER_SPEED/2*delta_time;
    if (player->position.x >= monsters[i].position.x && is_not_colliding(monsters, i, pos_increment)) {
        monsters[i].right = TRUE;
        monsters[i].position.x += pos_increment;
    }
    if (player->position.x < monsters[i].position.x && is_not_colliding(monsters, i, -pos_increment)) {
        monsters[i].left = TRUE;
        monsters[i].position.x -= pos_increment;
    }
    if (player->position.y >= monsters[i].position.y && is_not_colliding(monsters, i, pos_increment)) {
        monsters[i].down = TRUE;
        monsters[i].position.y += pos_increment;
    }
    if (player->position.y < monsters[i].position.y && is_not_colliding(monsters, i, -pos_increment)) {
        monsters[i].up = TRUE;
        monsters[i].position.y -= pos_increment;
    }   
}

void update(struct game *game, struct entity *player, struct entity monsters[]) {
    // logic to keep a fixed timestamp
    // must waste some time until we reach the target time
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - game->last_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);
    // get a delta time factor to update objects
    float delta_time = (SDL_GetTicks() - game->last_frame_time) / 1000.0;
    if (player->up)
        player->position.y -= PLAYER_SPEED * delta_time;
    if (player->down)
        player->position.y += PLAYER_SPEED * delta_time;
    if (player->left)
        player->position.x -= PLAYER_SPEED * delta_time;
    if (player->right)
        player->position.x += PLAYER_SPEED * delta_time;
    if (player->attack && (player->up ^ player->down ^ player->left ^ player->right)) {
        for (int i=0; i<MONSTER_CAP; i++) {
            check_monster_hit(&monsters[i], player);
            if (monsters[i].lives <= 0) {
                monsters[i].alive = FALSE;
            }
        }
    }
    int seconds = SDL_GetTicks() / 250;
    int sprite = seconds % 3;
    if (player->up || player->right || player->down || player->left) {
        player->texture.y = sprite * 80;
        if (player->up)
            player->texture.x = 192;
        if (player->down)
            player->texture.x = 0;
        if (player->left)
            player->texture.x = 64;
        if (player->right)
            player->texture.x = 128;
    } else {
        player->texture.y = sprite * 0;
    }
    for (int i=0; i<MONSTER_CAP; i++) {
        if (monsters[i].alive == TRUE)
            update_monster_tracking(player , monsters, i, sprite, delta_time);
    }
    seconds = SDL_GetTicks() / 1000;
    sprite = seconds % 1000;
    int lost = FALSE;
    for (int i=0; i<MONSTER_CAP; i++) {
        if (is_inside_rectangle(
            player->position.x+32,
            player->position.y+32,
            monsters[i].position.x,
            monsters[i].position.y,
            monsters[i].position.w,
            monsters[i].position.h
        )) {
            lost = TRUE;
        }
    }
    if (lost == TRUE && sprite == 999) {
        player->lives--;
        lost = FALSE;
    }
}

void setup(struct entity *player, struct entity *monster) {
    player->up = FALSE;
    player->down = FALSE;
    player->left = FALSE;
    player->right = FALSE;
    player->position.x = 50;
    player->position.y = 50;
    player->position.w = 64;
    player->position.h = 80;
    player->texture.x = 0;
    player->texture.y = 0;
    player->texture.w = 64;
    player->texture.h = 80;
    player->alive = TRUE;
    player->lives = 10;
    player->texture_name = "./assets/imgs/player.png";
    monster->up = FALSE;
    monster->down = FALSE;
    monster->left = FALSE;
    monster->right = FALSE;
    monster->position.x = 100;
    monster->position.y = 100;
    monster->position.w = 64;
    monster->position.h = 64;
    monster->texture.x = 0;
    monster->texture.y = 0;
    monster->texture.w = 64;
    monster->texture.h = 64;
    monster->alive = TRUE;
    monster->lives = 5;
    monster->texture_name = "./assets/imgs/imp.png";
}

void render_message(struct game *game) {
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(game->font, game->message, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(game->renderer, textSurface);

    SDL_Rect textRect;
    textRect.x = 10; // X-coordinate
    textRect.y = 10; // Y-coordinate
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(game->renderer, textTexture, NULL, &textRect);
}

void render_entity(struct game *game, struct entity entity) {
    SDL_Surface *imageSurface = IMG_Load(entity.texture_name);
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(game->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    SDL_RenderCopy(game->renderer, imageTexture, &entity.texture, &entity.position);
}

void render(struct game *game, struct entity player, struct entity monsters[]) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer); 
    if (player.alive)
        render_entity(game, player);
    for (int i=0; i<MONSTER_CAP; i++) {
        if (monsters[i].alive)
            render_entity(game, monsters[i]);
    }
    if (player.attack && (player.up ^ player.down ^ player.left ^ player.right))
        render_attack(game, &player);
    render_message(game);
    SDL_RenderPresent(game->renderer);
}

void destroy_window(struct game *game) {
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    SDL_Quit();
}

int main() {
    struct game game = newGame();
    game.is_running = initialize_window(&game);
    game.font = TTF_OpenFont("./assets/fonts/bigblue.ttf", 24); // You need a .ttf file for your font
    srand(time(NULL));
    int randomNumber = rand() % 3;
    struct entity monsters[MONSTER_CAP];
    for (int i=0; i<MONSTER_CAP; i++) {
        randomNumber = rand() % 4;
        monsters[i] = newMonster();
        if (randomNumber == 0)
            monsters[i].texture_name = "./assets/imgs/imp.png";
        if (randomNumber == 1)
            monsters[i].texture_name = "./assets/imgs/frog.png";
        if (randomNumber == 2)
            monsters[i].texture_name = "./assets/imgs/ghost.png";
        if (randomNumber == 3)
            monsters[i].texture_name = "./assets/imgs/skeleton.png";
    }
    struct entity monster;
    for (int i=0; i<10; i++) {
        int randw = rand() % WINDOW_WIDTH + 1;
        int randh = rand() % WINDOW_HEIGHT + 1;
        monsters[i].alive = TRUE;
        monsters[i].position.x = randw;
        monsters[i].position.y = randh;
    }
    struct entity player;
    setup(&player, &monster);
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