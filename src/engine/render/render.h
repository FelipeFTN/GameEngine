#ifndef RENDER_H
#define RENDER_H

#include "../types.h"
#include "../../../include/linmath.h"
#include "../include/SDL2/SDL.h"

typedef struct render_state {
    SDL_Window *window;
    u32 width;
    u32 height;
} Render_State;

void render_init(void);
void render_begin(void);
void render_end(void);
void render_quad(vec2 pos, vec2 size, vec4 color);
void render_quad_line(vec2 pos, vec2 size, vec4 color);
void render_line_segment(vec2 start, vec2 end, vec4 color);
void render_aabb(f32 *aabb, vec4 color);

#endif
