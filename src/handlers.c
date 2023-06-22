#include "handlers.h"
#include "../rpi_ws281x/ws2811.h"
#include <stdio.h>
#include <signal.h>

// Handles kill signals for cleanup
void kill_handler() {
    // Stop renderer
    running = 0;

    // LED library cleanup
    ws2811_fini(config.ledstring);

    // Cleanup config
    free_config(config);

    printf("Exited!\n");
}

void setup_handlers() {
    struct sigaction sa = {
            .sa_handler = kill_handler,
    };

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}
