#include "renderer.h"
#include "handlers.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "../rpi_ws281x/ws2811.h"
#include "config.h"

void *renderer(void *arg);

void setup_renderer() {
	pthread_t thread;
	pthread_create(&thread, NULL, renderer, NULL);
	return;
}

void *renderer(void *arg) {
	int ret;
	while (config->running) {
		if ((ret = ws2811_render(config->ledstring) != WS2811_SUCCESS)) {
			fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
			kill_handler();
			break;
		}

		// 15 frames /sec
		usleep(1000000 / 15);
	}

	return NULL;
}