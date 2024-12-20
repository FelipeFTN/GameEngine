#pragma once

#include "input.h"
#include "types.h"

typedef struct config {
    Input_Key keybinds[10];
} Config;

void config_init(void);
void config_key_bind(Input_Key key, const char *key_name);
