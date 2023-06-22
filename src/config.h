#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "../rpi_ws281x/ws2811.h"

typedef struct config_t {
    char valid;

    // TODO: Add options here.

    ws2811_t ledstring = {
        .freq = WS2811_TARGET_FREQ, // Only one option, no need for user configuration
        .dmanum = 0,
        .channel = {
            {
                .gpionum = 0,
                .invert = 0,
                .count = 0,
                .strip_type = 0,
                .brightness = 255, // Lower brightness should be handeled by us
            },
            { // Needed for some reason (from Library)
                .gpionum = 0,
                .invert = 0,
                .count = 0,
                .brightness = 0,
            },
        },
    };
} config_t;

config_t *load_config();

void free_config(config_t *config);

char try_load_ini(const char *path, config_t *config);

#endif
