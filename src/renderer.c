#include "renderer.h"

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
    SDL_Surface *imageSurface = IMG_Load(entity.animation.texture);
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
    if (player.attack && (player.orientation.up ^ player.orientation.down ^ player.orientation.left ^ player.orientation.right))
        render_attack(game, &player);
    render_message(game);
    SDL_RenderPresent(game->renderer);
}

void render_attack(struct game *game, struct entity *player) {
    SDL_Surface *imageSurface = IMG_Load("./assets/imgs/attack.png");
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(game->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    SDL_Rect dstrect = {player->position.x, player->position.y, 64, 64};
    double angle;
    if (player->orientation.up) {
        angle = 0;
        dstrect.y -= 20;
    }
    if (player->orientation.down) {
        angle = 180;
        dstrect.y += 50;
    }
    if (player->orientation.left) {
        angle = 270;
        dstrect.x -= 30;
        dstrect.y += 10;
    }
    if (player->orientation.right) {
        angle = 90;
        dstrect.x += 30;
        dstrect.y += 10;
    }
    SDL_Rect srcrect = {0, 0, 64, 64};
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if (game->frame >= 0 && game->frame < 5){
        SDL_RenderCopyEx(game->renderer, imageTexture, &srcrect, &dstrect, angle, NULL, flip);
        SDL_RenderPresent(game->renderer);
    } else if (game->frame >= 5 && game->frame < 10) {
        srcrect.x = 64;
        SDL_RenderCopyEx(game->renderer, imageTexture, &srcrect, &dstrect, angle, NULL, flip);
        SDL_RenderPresent(game->renderer);
    } else if (game->frame >= 10 && game->frame <= 15) {
        srcrect.x = 128;
        SDL_RenderCopyEx(game->renderer, imageTexture, &srcrect, &dstrect, angle, NULL, flip);
        SDL_RenderPresent(game->renderer);
    }
}