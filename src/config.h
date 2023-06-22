#include "../rpi_ws281x/ws2811.h"

typedef struct config_t {
    char valid;
    int running;
    ws2811_t* ledstring;
} config_t;

// Config
static config_t* config;

void load_config();

void free_config();

char try_load_ini(const char *path);