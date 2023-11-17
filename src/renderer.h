#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "entity.h"
#include "game.h"

void render_message(struct game *game);
void render_entity(struct game *game, struct entity entity);
void render(struct game *game, struct entity player, struct entity monsters[]);
void render_attack(struct game *game, struct entity *player);

#endif