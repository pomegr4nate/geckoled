#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

static const size_t MAX_LINE_LENGTH = 256;
static const char* DEFAULT_PATHS[] =
{
	"/etc/geckoled.conf",
	"geckoled.conf",
	"config.conf",
	"/etc/geckoled.ini",
	"geckoled.ini",
	"config.ini"
};
static const int NUM_DEFAULT_PATHS =
	sizeof(DEFAULT_PATHS) / sizeof(DEFAULT_PATHS[0]);

char read_ini_line(char* key, config_t* config);
void trim_inplace(char* str);

config_t* load_config()
{
	config_t* config = malloc(sizeof(config_t));
	
	memset(config, 0, sizeof(config_t));
	
	// Default values
    config->ledstring.dmanum = 10;
    config->ledstring.channel[0].gpionum = 18;
    config->ledstring.channel[0].invert = 0;
    config->ledstring.channel[0].count = 1;
    config->ledstring.channel[0].strip_type = SK6812_STRIP_GRBW
	
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
	
	*value = 0;
	value += 1;

	trim_inplace(key);
	trim_inplace(value);

    if (strcmp(key, "led_dmanum") == 0)
    {
        config->ledstring.dmanum = atoi(value);
        return 1;
    }

    if (strcmp(key, "led_gpionum") == 0)
    {
        config->ledstring.channel[0].gpionum = atoi(value);
        return 1;
    }

    if (strcmp(key, "led_invert") == 0)
    {
        if(strcmp(value, "1") == 0)
            config->ledstring.channel[0].invert = 1;
        else
            config->ledstring.channel[0].invert = 0;

        return 1;
    }

    // TODO Maybe add width and height and calculate this value
    if (strcmp(key, "led_count") == 0)
    {
        config->ledstring.channel[0].count = atoi(value);
        return 1;
    }

    // LED Strip types (We only support Strips that include White)
    if (strcmp(key, "led_type") == 0)
    {
        if(strcmp(value, "RGBW") == 0)
            config->ledstring.channel[0].strip_type = SK6812_STRIP_RGBW;
        else if(strcmp(value, "RBGW") == 0)
            config->ledstring.channel[0].strip_type = SK6812_STRIP_RBGW;
        else if(strcmp(value, "GRBW") == 0)
            config->ledstring.channel[0].strip_type = SK6812_STRIP_GRBW;
        else if(strcmp(value, "GBRW") == 0)
            config->ledstring.channel[0].strip_type = SK6812_STRIP_GBRW;
        else if(strcmp(value, "BRGW") == 0)
            config->ledstring.channel[0].strip_type = SK6812_STRIP_BRGW;
        else if(strcmp(value, "RBGW") == 0)
            config->ledstring.channel[0].strip_type = SK6812_STRIP_BGRW;
        else
            return 0;

        return 1;
    }
	
	return 0;
}

void trim_inplace(char* str)
{
	char* start = str;
	char* end = str + strlen(str) - 1;

	while (*start && (*start == ' ' || *start == '\t' || *start == '\n'))
	{
		start++;
	}

	while (end >= start && (*end == ' ' || *end == '\t' || *end == '\n'))
	{
		end--;
	}

	while (start <= end)
	{
		*(str++) = *(start++);
	}

	*str = 0;
}
