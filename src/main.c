#include <stdio.h>
#include <stdlib.h>
#include "config.h"

int main()
{
	config_t* config = load_config();
	
	printf("Starting.\n");
	
	printf("Exiting.\n");
	
	free_config(config);
	exit(0);
}