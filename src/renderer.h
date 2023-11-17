#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "entity.h"
#include "game.h"

void render_message(game game);
void render_entity(game game, entity entity);
void render(game game, entity player, entity attack, entity monsters[], entity background);
void render_attack(game game, entity player);

#endif