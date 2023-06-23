#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "handlers.h"
#include "renderer.h"

int main() {
	// Setup kill handlers
	setup_handlers();

	// Load config
	load_config();

	// Initialize library
	ws2811_init(config->ledstring);

	printf("Starting.\n");

	setup_renderer();

	exit(0);
}