#pragma once

#include "common.h"


int get_config_int(const ALLEGRO_CONFIG *config, const char *section, const char *key, int default_value = 0);
ALLEGRO_COLOR color_html(char const *string);
void color_html_to_rgba(char const *string,
						float *red, float *green, float *blue, float *alpha);
using namespace std;

class UserConfig
{
	ALLEGRO_CONFIG * default_config;
public:
	void save_values(void);
	void retrieve_values(void);
	bool load_file(void);
	bool save_file(void);
	UserConfig(void);
	~UserConfig(void);

	//screen resolution
	unsigned int res_x;
	unsigned int res_y;

	//map location
	unsigned int map_x;
	unsigned int map_y;

	unsigned int map_width;
	unsigned int map_height;

	int map_shift;

	//last used map file
	string map_path;
	bool map_autoload;

	int minimap_size;

	bool showgrid;

	//ao lighting
	bool use_ao;
	double tile_distance;
	unsigned int ray_distance;

	//system stuff
	ALLEGRO_FONT *font;

	//misc
	bool debugmode;
};

extern UserConfig user_config;
