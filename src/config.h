#ifndef __CONFIG_H__
#define __CONFIG_H__

typedef struct {
	char valid;
	
	// TODO: Add options here.
} config_t;

config_t* load_config();
void free_config(config_t* config);
char try_load_ini(const char* path, config_t* config);

#endif
