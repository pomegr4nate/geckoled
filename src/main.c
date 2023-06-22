#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "handlers.h"

// Config struct
config_t* config
// If the led renderer should be running
int running = 1;

int main()
{
    // Setup kill handlers
    setup_handlers();

	config = load_config();
	
	printf("Starting.\n");


	exit(0);
}