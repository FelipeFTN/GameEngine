#ifndef GLOBAL_H
#define GLOBAL_H

#include "render/render.h"
#include "config.h"
#include "time.h"

typedef struct global {
    Render_State render;
    Input_State input;
    Time_State time;
    Config config;
} Global;

extern Global global;

#endif
