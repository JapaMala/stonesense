#pragma once
#include "common.h"
#include "c_imagelist.h"
#include "s_map_block.h"

class TileSet;

enum e_color_by
{
	COLOR_NONE,
	COLOR_INI,
	COLOR_BIOME,
	COLOR_PALETTE,
	COLOR_DIRECT
};

enum e_color_source
{
	SOURCE_ELEV,
	SOURCE_DEPTH
};
enum e_offset_type
{
	OFFSET_NONE,
	OFFSET_SIXTEEN,
	OFFSET_PATH,
	OFFSET_PAIR,
	OFFSET_SIX,
	OFFSET_FOUR,
	OFFSET_RANDOM,
	OFFSET_COUNT
};

enum e_layer
{
	LAYER_BASE,
	LAYER_OBJECT
};

struct s_sprite
{
	s_sprite(void);
	int x;
	int y;
	int width;
	int height;
	int index;
	int origin_x;
	int origin_y;
	int column_height;

	e_color_by color_by;
	e_color_source color_source;
	unsigned int palette_number;
	ALLEGRO_COLOR color;
	e_offset_type offset_type;
	unsigned char offset_amount;
	terrain_type border_terrain;
	structure_type border_structure;

};

class c_tile
{
	s_sprite get_from_ini(ALLEGRO_CONFIG * config, const char * section, ALLEGRO_PATH * base_path);

public:
	void draw(float x, float y, int height, int bottom, int surface, s_map_block * block, TileSet * tileset, bool flip = 0, e_layer drawlayer = LAYER_BASE);
	void load_ini(ALLEGRO_PATH * _path);
	c_tile(void);
	~c_tile(void);
	inline bool operator < (const c_tile & a) const
	{
		return this->priority < a.priority;
	};

	vector<s_sprite> top_sprites;
	vector<s_sprite> bottom_sprites;
	vector<s_sprite> surface_sprites;
	vector<s_sprite> intermediate_sprites;
	vector<s_sprite> object_sprites;


	int height_max;
	int height_min;

	int priority;

	terrain_type special_terrain;
	structure_type structure;
};
