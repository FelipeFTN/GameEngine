#include "../../../include/glad/glad.h"
#include "../../../include/SDL2/SDL.h"

#include "../util.h"
#include "../global.h"
#include "render_internal.h"

SDL_Window *render_init_window(u32 width, u32 height) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        ERROR_EXIT("SDL_Init failed: %s\n", SDL_GetError());
    }

    SDL_Window* window = SDL_CreateWindow(
        "OpenGL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        global.render.width,
        global.render.height,
        SDL_WINDOW_OPENGL
    );
    if (window == NULL) {
        ERROR_EXIT("SDL_CreateWindow failed: %s\n", SDL_GetError());
    }

    SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        ERROR_EXIT("gladLoadGLLoader failed\n");
    }

    return window;
}

void render_init_shaders(Render_State_Internal *state) {
    state->shader_default = render_shader_create("./shaders/default.vert", "./shaders/default.frag");

    mat4x4_ortho(state->projection, 0.0f, global.render.width, 0.0f, global.render.height, -2.0f, 2.0f);

    glUseProgram(state->shader_default);
    glUniformMatrix4fv(
        glGetUniformLocation(state->shader_default, "projection"),
        1,
        GL_FALSE,
        // (f32 *)state->projection // &state->projection[0][0]
        &state->projection[0][0]
    );
}

void render_init_color_texture(u32 *texture) {
    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    u8 solid_white[4] = { 255, 255, 255, 255 };
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        1,
        1,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        solid_white
    );

    glBindTexture(GL_TEXTURE_2D, 0);
}

void render_init_quad(u32 *vao, u32 *vbo, u32 *ebo) {
    f32 vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left
    };

    u32 indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    glGenVertexArrays(1, vao);
    glGenBuffers(1, vbo);
    glGenBuffers(1, ebo);

    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void render_init_line(u32 *vao, u32 *vbo) {
    // 1. Generate a vertex array object
    // 2. Bind the vertex array object
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    // 1. Generate a buffer object
    // 2. Bind the buffer object to the GL_ARRAY_BUFFER target
    // 3. Allocate memory for the buffer object
    glGenBuffers(1, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, *vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * 3 * sizeof(f32), NULL, GL_DYNAMIC_DRAW);

    // 1. Enable the vertex attribute array
    // 2. Specify the location and data format of the array
    // 3. Unbind the buffer object
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind the vertex array object
    glBindVertexArray(0);
}
