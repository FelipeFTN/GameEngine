#pragma once

#include <stdio.h>

#define ERROR_EXIT(...) { fprintf(stderr, __VA_ARGS__); exit(1); }
#define ERROR_RETURN(R, ...) { fprintf(stderr, __VA_ARGS__); return R; }

#define WHITE (vec4){1.0f, 1.0f, 1.0f, 1.0f}
#define BLACK (vec4){0.0f, 0.0f, 0.0f, 1.0f}
#define RED (vec4){1.0f, 0.0f, 0.0f, 1.0f}
#define GREEN (vec4){0.0f, 1.0f, 0.0f, 1.0f}
#define BLUE (vec4){0.0f, 0.0f, 1.0f, 1.0f}
#define YELLOW (vec4){1.0f, 1.0f, 0.0f, 1.0f}
#define MAGENTA (vec4){1.0f, 0.0f, 1.0f, 1.0f}
#define CYAN (vec4){0.0f, 1.0f, 1.0f, 1.0f}
#define ORANGE (vec4){1.0f, 0.5f, 0.0f, 1.0f}
#define PURPLE (vec4){0.5f, 0.0f, 1.0f, 1.0f}
#define PINK (vec4){1.0f, 0.0f, 0.5f, 1.0f}
