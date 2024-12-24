#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/glad/glad.h"

#include "engine/global.h"
#include "engine/physics.h"
#include "engine/config.h"
#include "engine/input.h"
#include "engine/time.h"
#include "engine/util.h"

#define SDL_MAIN_HANDLED

static bool should_quit = false;
static vec2 pos = {0.f, 0.f};

static void input_handle(void) {
    // if (global.input.left == KS_PRESSED || global.input.left == KS_HELD)
    //     pos[0] -= 500 * global.time.delta;
    // if (global.input.right == KS_PRESSED || global.input.right == KS_HELD)
    //     pos[0] += 500 * global.time.delta;
    // if (global.input.up == KS_PRESSED || global.input.up == KS_HELD)
    //     pos[1] += 500 * global.time.delta;
    // if (global.input.down == KS_PRESSED || global.input.down == KS_HELD)
    //     pos[1] -= 500 * global.time.delta;
    if (global.input.escape == KS_PRESSED || global.input.escape == KS_HELD)
        should_quit = true;

    i32 x, y;
    SDL_GetMouseState(&x, &y);
    pos[0] = (f32)x;
    pos[1] = global.render.height - y;
}

int main(int argc, char *argv[]) {
    time_init(60);
    config_init();
    render_init();
    physics_init();

    SDL_ShowCursor(false);

    AABB test_aabb = {
        .position = {global.render.width * 0.5, global.render.height * 0.5},
        .half_size = {100, 100}
    };

    while (!should_quit) {
        time_update();

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    should_quit = true;
                    break;
                default:
                    break;
            }
        }

        input_update();
        input_handle();
        physics_update();

        render_begin();

        render_aabb((f32*)&test_aabb, (vec4){1, 1, 1, 0.5});
        
        if (physics_point_intersect_aabb(pos, test_aabb)) {
            render_quad(pos, (vec2){5, 5}, RED);
        } else {
            render_quad(pos, (vec2){5, 5}, WHITE);
        }

        render_end();
        time_update_late();
    }

    return 0;
}

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}

#endif
