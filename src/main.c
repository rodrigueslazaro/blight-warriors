#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
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
    SDL_Delay(100);
    srcrect.x = 64;
    SDL_RenderCopyEx(game->renderer, imageTexture, &srcrect, &dstrect, angle, NULL, flip);
    SDL_RenderPresent(game->renderer);
    SDL_Delay(100);
    srcrect.x = 128;
    SDL_RenderCopyEx(game->renderer, imageTexture, &srcrect, &dstrect, angle, NULL, flip);
    SDL_RenderPresent(game->renderer);
    SDL_Delay(100);
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

void update(struct game *game, struct entity *player, struct entity *monster) {
    // logic to keep a fixed timestamp
    // must waste some time until we reach the target time
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - game->last_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);
    // get a delta time factor to update objects
    float delta_time = (SDL_GetTicks() - game->last_frame_time) / 1000.0;
    char str[10];
    sprintf(str, "%d", monster->lives);
    strcpy(game->message, str);
    if (player->up)
        player->position.y -= PLAYER_SPEED * delta_time;
    if (player->down)
        player->position.y += PLAYER_SPEED * delta_time;
    if (player->left)
        player->position.x -= PLAYER_SPEED * delta_time;
    if (player->right)
        player->position.x += PLAYER_SPEED * delta_time;
    if (player->attack && (player->up ^ player->down ^ player->left ^ player->right)) {
        render_attack(game, player);
        check_monster_hit(monster, player);
        if (monster->lives <= 0) {
            monster->alive = FALSE;
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
    if (monster->alive) {
        monster->texture.x = sprite * 64;
    } else {
        monster->texture.x = sprite * 0;
    }
    monster->up = FALSE;
    monster->right = FALSE;
    monster->down = FALSE;
    monster->left = FALSE;
    if (player->position.x >= monster->position.x) {
        monster->right = TRUE;
        monster->position.x += PLAYER_SPEED/2 * delta_time;
    }
    if (player->position.x < monster->position.x) {
        monster->left = TRUE;
        monster->position.x -= PLAYER_SPEED/2 * delta_time;
    }
    if (player->position.y >= monster->position.y) {
        monster->down = TRUE;
        monster->position.y += PLAYER_SPEED/2 * delta_time;
    }
    if (player->position.y < monster->position.y) {
        monster->up = TRUE;
        monster->position.y -= PLAYER_SPEED/2 * delta_time;
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
    textRect.x = WINDOW_WIDTH/2-textSurface->w/2; // X-coordinate
    textRect.y = WINDOW_HEIGHT/2; // Y-coordinate
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

void render(struct game *game, struct entity player, struct entity monster) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer); 
    if (player.alive)
        render_entity(game, player);
    if (monster.alive)
        render_entity(game, monster);
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
    // dynamic_array *enemies = dynamic_array_create(sizeof(struct enemy));
    // SDL_Rect p = {50, 50, 192, 64};
    // SDL_Rect t = {0, 0, 192, 64};
    // struct enemy e = newEnemy(p, t);
    // dynamic_array_push_back(enemies, &e);
    // SDL_Rect p = {50, 50, 192, 64};
    // SDL_Rect t = {0, 0, 192, 64};
    // struct enemy enemies[100];
    // for (int i=0; i<100; i++) {
    //     struct enemy e = newEnemy(p, t);
    //     enemies[i] = e;
    // }
    struct entity monster;

    struct entity player;
    setup(&player, &monster);

    while (game.is_running) {
        process_input(&game, &player);
        switch (game.game_state) {
            case 0:
                strcpy(game.message, "Welcome to the Blight!");
                break;
            case 1:
                strcpy(game.message, "");
                game.last_frame_time = SDL_GetTicks(); // time since game began
                update(&game, &player, &monster);
                break;
            case 2:
                strcpy(game.message, "[PAUSED]");
                break;
        }
        render(&game, player, monster);
    }

    destroy_window(&game);

    return 0;
}