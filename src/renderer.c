#include "renderer.h"
#include "handlers.h"
#include <stdio.h>
#include <pthread.h>

void setup_renderer() {
    pthread_t thread;
    pthread_create(thread, NULL, renderer, NULL);
    return;
}

void* renderer(void* arg) {
    while(running) {
        if ((ret = ws2811_render(&(config->ledstring)) != WS2811_SUCCESS)
        {
            fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
            kill_handler();
            break;
        }
    }
}