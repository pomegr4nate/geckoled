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
static config_t* config;

void load_config();

void free_config();

char try_load_ini(const char *path);