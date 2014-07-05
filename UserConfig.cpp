#include "UserConfig.h"

/* Function: al_color_html_to_rgba
*/
void color_html_to_rgba(char const *string,
						float *red, float *green, float *blue, float *alpha)
{
	char const *ptr = string;
	uint32_t rgba;
	if (*ptr == '#') ptr++;
	rgba = strtol(ptr, NULL, 16);
	if(strlen(ptr) == 8)
	{
		*red = (rgba >> 24) / 255.0;
		*green = ((rgba >> 16) & 255) / 255.0;
		*blue = ((rgba >> 8) & 255) / 255.0;
		*alpha = (rgba & 255) / 255.0;
	}
	else
	{
		*red = (rgba >> 16) / 255.0;
		*green = ((rgba >> 8) & 255) / 255.0;
		*blue = (rgba & 255) / 255.0;
		*alpha = 1.0;
	}
}

/* Function: al_color_html
 */
ALLEGRO_COLOR color_html(char const *string)
{
   float r, g, b, a;
   color_html_to_rgba(string, &r, &g, &b, &a);
   return al_map_rgba_f(r, g, b, a);
}

int get_config_int(const ALLEGRO_CONFIG *config, const char *section, const char *key, int default_value)
{
	const char * buffer = al_get_config_value(config, section, key);
	if(buffer) return atoi(buffer);
	else return default_value;
}

UserConfig::UserConfig(void)
{
	res_x = 640;
	res_y = 480;
	map_x = 0;
	map_y = 0;
	map_width = 50;
	map_height = 50;
	map_shift = 100;
	map_autoload = 1;
	default_config = al_create_config();
	minimap_size = 128;

	use_ao = 1;
	tile_distance = 8;
	ray_distance = 3;

	debugmode = 0;
}

UserConfig::~UserConfig(void)
{
	save_file();
	al_destroy_config(default_config);
}

void UserConfig::save_values(void)
{
	char buffer[256];
	sprintf(buffer, "%d", res_x);
	al_set_config_value(default_config, "GRAPHICS", "width", buffer);
	sprintf(buffer, "%d", res_y);
	al_set_config_value(default_config, "GRAPHICS", "height", buffer);

	sprintf(buffer, "%d", map_x);
	al_set_config_value(default_config, "MAP", "x_location", buffer);
	sprintf(buffer, "%d", map_y);
	al_set_config_value(default_config, "MAP", "y_location", buffer);

	sprintf(buffer, "%d", map_width-2);
	al_set_config_value(default_config, "MAP", "width", buffer);
	sprintf(buffer, "%d", map_height-2);
	al_set_config_value(default_config, "MAP", "height", buffer);

	al_set_config_value(default_config, "MAP", "path", map_path.c_str());

	sprintf(buffer, "%d", map_autoload);
	al_set_config_value(default_config, "MAP", "autoload", buffer);

	sprintf(buffer, "%d", map_shift);
	al_set_config_value(default_config, "MAP", "vertical_shift", buffer);

	sprintf(buffer, "%d", minimap_size);
	al_set_config_value(default_config, "MINIMAP", "size", buffer);

	sprintf(buffer, "%s", use_ao?"yes":"no");
	al_set_config_value(default_config, "AMBIENT_OCCLUSION", "use_ambient_occlusion", buffer);

	sprintf(buffer, "%f", tile_distance);
	al_set_config_value(default_config, "AMBIENT_OCCLUSION", "relative_tile_width", buffer);

	sprintf(buffer, "%d", ray_distance);
	al_set_config_value(default_config, "AMBIENT_OCCLUSION", "ray_cast_distance", buffer);

}

void UserConfig::retrieve_values(void)
{
	res_x = atoi(al_get_config_value(default_config, "GRAPHICS", "width"));
	res_y = atoi(al_get_config_value(default_config, "GRAPHICS", "height"));

	map_x = atoi(al_get_config_value(default_config, "MAP", "x_location"));
	map_y = atoi(al_get_config_value(default_config, "MAP", "y_location"));

	map_width = atoi(al_get_config_value(default_config, "MAP", "width"))+2;
	map_height = atoi(al_get_config_value(default_config, "MAP", "height"))+2;

	map_path = al_get_config_value(default_config, "MAP", "path");

	map_autoload = atoi(al_get_config_value(default_config, "MAP", "autoload"));

	map_shift = atoi(al_get_config_value(default_config, "MAP", "vertical_shift"));

	minimap_size = atoi(al_get_config_value(default_config, "MINIMAP", "size"));

	const char * aotext = al_get_config_value(default_config, "AMBIENT_OCCLUSION", "use_ambient_occlusion");
	if(strcmp("yes", aotext) == 0)
		use_ao = 1;

	tile_distance = atof(al_get_config_value(default_config, "AMBIENT_OCCLUSION", "relative_tile_width"));

	ray_distance = atoi(al_get_config_value(default_config, "AMBIENT_OCCLUSION", "ray_cast_distance"));

}

bool UserConfig::save_file(void)
{
	return al_save_config_file("isoworld.ini", default_config);
}

bool UserConfig::load_file(void)
{
	bool success = 0;
	ALLEGRO_CONFIG * loaded_config = al_load_config_file("isoworld.ini");
	if(loaded_config)
	{
		al_merge_config_into(default_config, loaded_config);
		success = 1;
	}
	al_destroy_config(loaded_config);
	return success;
}
