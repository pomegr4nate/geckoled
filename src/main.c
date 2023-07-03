#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "handlers.h"
#include "renderer.h"
#include "ws2811.h"
#include "cycle.h"

int main() {
	// Setup kill handlers
	setup_handlers();

	// Load config
	load_config();

	// Initialize library
	ws2811_init(config->ledstring);

	printf("Starting.\n");

	config->running = 1;

	setup_renderer();
	run_daynight_cycle();

	exit(0);
}
