#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

static const size_t MAX_LINE_LENGTH = 256;
static const char* DEFAULT_PATHS[] =
{
	"/etc/geckoled.ini",
	"geckoled.ini",
	"config.ini"
};
static const int NUM_DEFAULT_PATHS =
	sizeof(DEFAULT_PATHS) / sizeof(DEFAULT_PATHS[0]);

char read_ini_line(char* key, config_t* config);

config_t* load_config()
{
	config_t* config = malloc(sizeof(config_t));
	
	memset(config, 0, sizeof(config_t));
	
	// TODO: Set possible default values here
	
	for (int i = 0; i < NUM_DEFAULT_PATHS; ++i)
	{
		if (try_load_ini(DEFAULT_PATHS[i], config))
		{
			return config;
		}
	}
	
	fprintf(stderr, "Could not find a valid config file.\n");
	exit(1);
}

void free_config(config_t* config)
{
	// TODO: Free values in config struct here
	
	free(config);
}

char try_load_ini(const char* path, config_t* config)
{
	FILE* file = fopen(path, "rt");
	
	if (!file)
	{
		return 0;
	}
	
	printf("Found config file at %s\n", path);
	
	char* line_buffer = malloc(MAX_LINE_LENGTH);
	int line_num = 0;
	
	memset(line_buffer, 0, MAX_LINE_LENGTH);
	
	while (fgets(line_buffer, MAX_LINE_LENGTH - 1, file) != NULL)
	{
		line_num += 1;
		
		if (!read_ini_line(line_buffer, config))
		{
			free(line_buffer);
			fclose(file);
			fprintf(stderr, "%s(%d): Unable to parse config value from line.\n", path, line_num);
			
			return 0;
		}
	}
	
	fclose(file);
	free(line_buffer);
	
	return 1;
}

char read_ini_line(char* key, config_t* config)
{
	int total_length = strlen(key);
	
	if (total_length == 0 || (total_length == 1 && key[0] == '\n'))
	{
		return 1;
	}
	
	char* value = strchr(key, '=');
	
	if (!value)
	{
		return 0;
	}
	
	int value_length = strlen(value);
	
	if (value_length <= 1)
	{
		return 0;
	}
	
	if (*(value + value_length - 1) == '\n')
	{
		*(value + value_length - 1) = 0;
	}
	
	*value = 0;
	value += 1;
	
	// TODO: Add options to read here
	//
	// Example:
	//
	//   if (strcmp(key, "sunset") == 0)
	//   {
	//       char* copied_value = malloc(value_length + 1);
	//       memset(copied_value, 0, value_length + 1);
	//       strcpy(copied_value, value);
	//       config->sunset = copied_value;
	//       return 1;
	//   }
	//
	
	return 0;
}
