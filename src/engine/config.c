#include "global.h"
#include "io.h"
#include "util.h"

static const char *CONFIG_DEFAULT = 
    "[controls]\n"
    "left=A\n"
    "right=D\n"
    "up=W\n"
    "down=S\n"
    "space=Space\n"
    "escape=Escape\n"
    "\n";

static char tmp_buffer[20] = {0};

static char *config_get_value(const char *data, const char *value) {
    char *line = strstr(data, value);
    if (!line)
        ERROR_EXIT("Failed to find config value: %s %s\n", value,
            "Try deleting config.ini and restarting.\n");

    size_t len = strlen(line);
    char *end = line + len;

    char *curr = line;
    char *tmp_ptr= &tmp_buffer[0];

    // Move pointer to '='
    while (*curr != '=' && curr != end)
        curr++;

    curr++;

    // Consume any spaces
    while (*curr == ' ')
        curr++;

    // Get characters util end of line
    while (*curr != '\n' && curr != end && curr != 0)
        *tmp_ptr++ = *curr++;

    *(tmp_ptr+1) = 0;

    return tmp_buffer;
}

static void load_controls(const char *data) {
    config_key_bind(INPUT_KEY_LEFT, config_get_value(data, "left"));
    config_key_bind(INPUT_KEY_RIGHT, config_get_value(data, "right"));
    config_key_bind(INPUT_KEY_UP, config_get_value(data, "up"));
    config_key_bind(INPUT_KEY_DOWN, config_get_value(data, "down"));
    config_key_bind(INPUT_KEY_SPACE, config_get_value(data, "space"));
}

static int config_load() {
    File file = io_file_read("./config.ini");
    if (!file.is_valid)
        return -1;

    load_controls(file.data);

    free(file.data);

    return 0;
}

void config_init() {
    if (config_load() == 0)
        return;

    io_file_write((void*)CONFIG_DEFAULT, strlen(CONFIG_DEFAULT), "./config.ini");
    if (config_load() != 0) {
        ERROR_EXIT("Failed to load config file\n");
    }
}

void config_key_bind(Input_Key key, const char *key_name) {
    SDL_Scancode scan_code = SDL_GetScancodeFromName(key_name);
    if (scan_code == SDL_SCANCODE_UNKNOWN)
        ERROR_EXIT("Failed to bind key: %s\n", key_name);

    global.config.keybinds[key] = scan_code;
}
