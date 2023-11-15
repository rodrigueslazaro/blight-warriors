#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "./constants.h"
#include "./game.h"
#include "./player.h"
#include "./ball.h"

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

void render_attack(struct game *game, struct player *player) {
    SDL_Surface *imageSurface = IMG_Load("./assets/imgs/attack.png");
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(game->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    SDL_Rect dstrect = {player->p.x, player->p.y, 64, 64};
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

void spawn_monster(struct game *game, struct player player) {
    SDL_Surface *imageSurface = IMG_Load("./assets/imgs/monster.png");
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(game->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    SDL_RenderCopy(game->renderer, imageTexture, &player.s, &player.p);

    SDL_RenderPresent(game->renderer); // swaps back buffer with front buffer   
}

void process_input(struct game *game, struct player *player) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        game->is_running = FALSE;
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            game->is_running = FALSE;
            break;
        case SDLK_w:
            player->up = TRUE;
            player->is_moving = TRUE;
            break;
        case SDLK_s:
            player->down = TRUE;
            player->is_moving = TRUE;
            break;
        case SDLK_a:
            player->left = TRUE;
            player->is_moving = TRUE;
            break;
        case SDLK_d:
            player->right = TRUE;
            player->is_moving = TRUE;
            break;
        case SDLK_j:
            render_attack(game, player);
            break;
        case SDLK_k:
            struct player pcopy = *player;
            spawn_monster(game, pcopy);
            break;
        case SDLK_SPACE:
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
            break;
        }
        break;
    case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_w:
            player->up = FALSE;
            player->is_moving = FALSE;
            break;
        case SDLK_s:
            player->down = FALSE;
            player->is_moving = FALSE;
            break;
        case SDLK_a:
            player->left = FALSE;
            player->is_moving = FALSE;
            break;
        case SDLK_d:
            player->right = FALSE;
            player->is_moving = FALSE;
            break;
        }
        break;
    default:
        break;
    }
}

int check_collision(struct player *p1, struct player *p2, struct ball *ball) {
    // xpos + width 
    if (ball->x+ball->w >= p2->x && ball->y >= p2->y && ball->y <= p2->y+p2->h){
        ball->xspeed *= -1;
        return TRUE;
    }
    if (ball->x <= p1->x+p1->w && ball->y >= p1->y && ball->y <= p1->y+p1->h) {
        ball->xspeed *= -1;
        return TRUE;
    }
    if (ball->y+ball->h >= WINDOW_HEIGHT) {
        ball->yspeed *= -1;
        return TRUE;
    }
    if (ball->y <= 0) {
        ball->yspeed *= -1;
        return TRUE;
    }
    return FALSE;
}

void update(struct game *game, struct player *player) {
    // logic to keep a fixed timestamp
    // must waste some time until we reach the target time
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - game->last_frame_time);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);
    // get a delta time factor to update objects
    float delta_time = (SDL_GetTicks() - game->last_frame_time) / 1000.0f;

    if (player->up)
        player->p.y -= 100 * delta_time;
    if (player->down)
        player->p.y += 100 * delta_time;
    if (player->left)
        player->p.x -= 100 * delta_time;
    if (player->right)
        player->p.x += 100 * delta_time;

    int seconds = SDL_GetTicks() / 250;
    int sprite = seconds % 3;
    if (player->is_moving) {
        player->s.y = sprite * 80;
        if (player->up)
            player->s.x = 192;
        if (player->down)
            player->s.x = 0;
        if (player->left)
            player->s.x = 64;
        if (player->right)
            player->s.x = 128;
    } else {
        player->s.y = sprite * 0;
    }
        
}

void setup(struct player *player) {
    player->x = 20;
    player->y = 20;
    player->w = 30;
    player->h = 30;
    player->up = FALSE;
    player->down = FALSE;
    player->left = FALSE;
    player->right = FALSE;
    player->p.x = 50;
    player->p.y = 50;
    player->p.w = 64;
    player->p.h = 80;
    player->s.x = 0;
    player->s.y = 0;
    player->s.w = 64;
    player->s.h = 80;
    player->is_moving = FALSE;
}



void render(struct game *game, struct player player) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);

    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(game->font, game->message, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(game->renderer, textSurface);

    SDL_Rect textRect;
    textRect.x = WINDOW_WIDTH/2-textSurface->w/2; // X-coordinate
    textRect.y = WINDOW_HEIGHT/2; // Y-coordinate
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(game->renderer, textTexture, NULL, &textRect);

    SDL_Surface *imageSurface = IMG_Load("./assets/imgs/player.png");
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(game->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    SDL_RenderCopy(game->renderer, imageTexture, &player.s, &player.p);

    SDL_RenderPresent(game->renderer); // swaps back buffer with front buffer
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

    struct player player;
    setup(&player);

    while (game.is_running) {
        process_input(&game, &player);
        switch (game.game_state) {
            case 0:
                game.message = "Welcome to the Blight!";
                break;
            case 1:
                game.message = " ";
                game.last_frame_time = SDL_GetTicks(); // time since game began
                update(&game, &player);
                break;
            case 2:
                game.message = "[PAUSED]";
                break;
        }
        render(&game, player);
    }

    destroy_window(&game);

    return 0;
}