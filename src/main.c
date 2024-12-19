#include <SDL2/SDL_video.h>
#include <stdbool.h>

#include "engine/global.h"

#include "../include/glad/glad.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#ifdef _WIN32
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return main(__argc, __argv);
}

#endif

int main(int argc, char *argv[]) {
    render_init();

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        render_begin();

        render_quad(
            (vec2){global.render.width * 0.5, global.render.height * 0.5},
            (vec2){50, 50},
            (vec4){0.0f, 1.0f, 0.0f, 1.0f}
        );

        render_end();
    }

    return 0;
}
