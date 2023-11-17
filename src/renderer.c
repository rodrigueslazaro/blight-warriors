#include "renderer.h"

void render_message(game game) {
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(game.font, game.message, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(game.renderer, textSurface);

    SDL_Rect textRect;
    textRect.x = 10; // X-coordinate
    textRect.y = 10; // Y-coordinate
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(game.renderer, textTexture, NULL, &textRect);
}

void render_entity(game game, entity entity) {
    SDL_Surface *imageSurface = IMG_Load(entity.animation.texture);
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(game.renderer, imageSurface);
    if (entity.cooldown.damage > 0) {
        if (entity.cooldown.damage >= 75) {
            SDL_SetTextureColorMod(imageTexture, 255, 200, 200);
            SDL_SetTextureAlphaMod(imageTexture, 200);
        } else if (entity.cooldown.damage >= 50) {
            SDL_SetTextureColorMod(imageTexture, 255, 200, 200);
            SDL_SetTextureAlphaMod(imageTexture, 100);
        } else if (entity.cooldown.damage >= 25) {
            SDL_SetTextureColorMod(imageTexture, 255, 200, 200);
            SDL_SetTextureAlphaMod(imageTexture, 200);
        } else if (entity.cooldown.damage >= 0) {
            SDL_SetTextureColorMod(imageTexture, 255, 200, 200);
            SDL_SetTextureAlphaMod(imageTexture, 100);
        }
    }
    SDL_FreeSurface(imageSurface);
    if (entity.type == attack) {
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (entity.orientation.up) {
            SDL_RenderCopyEx(game.renderer, imageTexture, &entity.texture, &entity.position, 0, NULL, flip);
        } else if (entity.orientation.right) {
            SDL_RenderCopyEx(game.renderer, imageTexture, &entity.texture, &entity.position, 90, NULL, flip);
        } else if (entity.orientation.down) {
            SDL_RenderCopyEx(game.renderer, imageTexture, &entity.texture, &entity.position, 180, NULL, flip);
        } else if (entity.orientation.left) {
            SDL_RenderCopyEx(game.renderer, imageTexture, &entity.texture, &entity.position, 270, NULL, flip);
        }
    } else {
        SDL_RenderCopy(game.renderer, imageTexture, &entity.texture, &entity.position);
    }
}

void render(game game, entity player, entity attack, entity monsters[], entity background) {
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game.renderer); 
    render_entity(game, background);
    if (player.alive) {
        render_entity(game, player);
        if (player.cooldown.attack > 0) {
            render_entity(game, attack);
        }
    }
    for (int i=0; i<MONSTER_CAP; i++) {
        if (monsters[i].alive)
            render_entity(game, monsters[i]);
    }
    render_message(game);
    SDL_RenderPresent(game.renderer);
}