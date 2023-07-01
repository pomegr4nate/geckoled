#ifndef GECKOLED_CONFIG_H
#define GECKOLED_CONFIG_H

#include "../rpi_ws281x/ws2811.h"

typedef struct config_t {
    // If the renderer should be running
    int running;

    // Width and height of led matrix
    int width;
    int height;

    // 1: Back Left
    // 2: Back Right
    // 3: Front Left
    // 4: Front Right
    int start_position;

    // How many LED's each row is offset;
    int led_offset;

    ws2811_t* ledstring;
} config_t;

// Config
#ifndef _CONFIG_IMPL
extern config_t* config;
#endif

void load_config();

void free_config();

#endif