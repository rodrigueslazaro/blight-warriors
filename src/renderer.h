#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "entity.h"
#include "game.h"

typedef struct {
    animation *frames;
    int capacity;
    int size;
} animastack;

void init_stack(animastack *stack, int initialCapacity);
void push_frame(animastack *stack, animation frame);
animation pop_frame(animastack *stack);
void free_stack(animastack *stack);
void render_message(game *game);
void render_entity(game *game, entity entity);
void render(game *game, entity player, entity monsters[]);
void render_attack(game *game, entity *player);

#endif