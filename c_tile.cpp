#include "c_tile.h"
#include "UserConfig.h"
#include "s_map_block.h"
#include "console.h"
#include "TileSet.h"

int get_border_offset_sixteen(unsigned char in)
{
	if((in&(2|8|32|128)) == (2|8|32|128))
		return 15;
	else if((in&(2|8|128)) == (2|8|128))
		return 14;
	else if((in&(2|32|128)) == (2|32|128))
		return 13;
	else if((in&(8|32|128)) == (8|32|128))
		return 12;
	else if((in&(2|8|32)) == (2|8|32))
		return 11;
	else if((in&(2|128)) == (2|128))
		return 10;
	else if((in&(32|128)) == (32|128))
		return 9;
	else if((in&(8|32)) == (8|32))
		return 8;
	else if((in&(2|8)) == (2|8))
		return 7;
	else if((in&(8|32)) == (8|32))
		return 6;
	else if((in&(2|32)) == (2|32))
		return 5;
	else if((in&128) == 128)
		return 4;
	else if((in&32) == 32)
		return 3;
	else if((in&8) == 8)
		return 2;
	else if((in&2) == 2)
		return 1;
	else if(in == 0)
		return 0;
	return -1;
}

int get_border_offset_path(unsigned char in)
{
	if((in&(2|8|32|128)) == (2|8|32|128))
		return 10;
	else if((in&(2|8|128)) == (2|8|128))
		return 9;
	else if((in&(2|32|128)) == (2|32|128))
		return 8;
	else if((in&(8|32|128)) == (8|32|128))
		return 7;
	else if((in&(2|8|32)) == (2|8|32))
		return 6;
	else if((in&(2|128)) == (2|128))
		return 5;
	else if((in&(32|128)) == (32|128))
		return 4;
	else if((in&(8|32)) == (8|32))
		return 3;
	else if((in&(2|8)) == (2|8))
		return 2;
	else if((in&(8|32)) == (8|32))
		return 1;
	else if((in&(2|32)) == (2|32))
		return 0;
	else if((in&2) == 2)
		return 0;
	else if((in&8) == 8)
		return 1;
	else if((in&32) == 32)
		return 0;
	else if((in&128) == 128)
		return 1;
	return -1;
}

int get_border_offset_six(unsigned char in)
{
	if((in&(2|32)) == (2|32))
		return 0;
	else if((in&(8|128)) == (8|128))
		return 1;
	else if((in&(8|32)) == (8|32))
		return 2;
	else if((in&(32|128)) == (32|128))
		return 3;
	else if((in&(2|8)) == (2|8))
		return 4;
	else if((in&(2|128)) == (2|128))
		return 5;
	else if((in&2) == 2)
		return 0;
	else if((in&8) == 8)
		return 1;
	else if((in&32) == 32)
		return 0;
	else if((in&128) == 128)
		return 1;
	return -1;
}

int get_border_offset_four(unsigned char in)
{
	if(((in&2) == 2) && !((in&32) == 32))
		return 3;
	else if(((in&8) == 8) && !((in&128) == 128))
		return 0;
	else if(((in&32) == 32) && !((in&2) == 2))
		return 1;
	else if(((in&128) == 128) && !((in&8) == 8))
		return 2;
	return -1;
}

int get_border_offset_pair(unsigned char in)
{
	if((in&(2|32)) == (2|32))
		return 0;
	else if((in&(8|128)) == (8|128))
		return 1;
	else if((in&2) == 2)
		return 0;
	else if((in&8) == 8)
		return 1;
	else if((in&32) == 32)
		return 0;
	else if((in&128) == 128)
		return 1;
	return -1;
}

ALLEGRO_COLOR mix_colors( ALLEGRO_COLOR lhs, ALLEGRO_COLOR rhs)
{
	lhs.a *= rhs.a;
	lhs.r *= rhs.r;
	lhs.g *= rhs.g;
	lhs.b *= rhs.b;
	return lhs;
}

void draw_sprite(TileSet * tileset, s_sprite sprite, s_map_block * block, float x, float y, int shrink, bool flip = 0, int offset = 0, bool shiftup = false)
{
	if(offset < 0)
		return;
	int target_width = al_get_bitmap_width(al_get_target_bitmap());
	int target_height = al_get_bitmap_height(al_get_target_bitmap());
	if((x + sprite.origin_x > target_width) || (y + sprite.origin_y > target_height) || 
		((x + sprite.origin_x + sprite.width) < 0) || ((y + sprite.origin_y + (sprite.height - shrink)) < 0))return;
	int flags = 0;
	if(flip)
		flags = ALLEGRO_FLIP_HORIZONTAL;
	ALLEGRO_COLOR color;
	switch(sprite.color_by)
	{
	case COLOR_NONE:
		color = block->light;
		break;
	case COLOR_INI:
		color = mix_colors(sprite.color, block->light);
		break;
	case COLOR_BIOME:
		color = mix_colors(block->color, block->light);
		break;
	case COLOR_PALETTE:
		switch(sprite.color_source)
		{
		case SOURCE_ELEV:
			color = mix_colors(tileset->get_palette_color(sprite.palette_number, block->height), block->light);
			break;
		case SOURCE_DEPTH:
			color = mix_colors(tileset->get_palette_color(sprite.palette_number,block->water_height-block->height), block->light);
			break;
		default:
			color = block->light;
		}
		break;
	default:
		color = block->light;
		break;
	}
	al_draw_tinted_bitmap_region(
		imagelist.get_image(sprite.index),
		color,
		sprite.x + (sprite.width * offset),
		sprite.y + ((!shiftup)*shrink),
		sprite.width,
		sprite.height - shrink,
		x + sprite.origin_x,
		y + sprite.origin_y,
		flags);
}

e_color_by get_color_selector(const char * text)
{
	if(strcmp(text, "none") == 0)
		return COLOR_NONE;
	if(strcmp(text, "html") == 0)
		return COLOR_INI;
	if(strcmp(text, "biome_map") == 0)
		return COLOR_BIOME;
	if(strcmp(text, "palette") == 0)
		return COLOR_PALETTE;
	return COLOR_NONE;
}

e_color_source get_color_source(const char * text)
{
	if(strcmp(text, "elevation") == 0)
		return SOURCE_ELEV;
	if(strcmp(text, "water_depth") == 0)
		return SOURCE_DEPTH;
	return SOURCE_ELEV;
}

terrain_type get_terrain_type(const char * text)
{
	if(strcmp(text, "any") == 0)
		return TERRAIN_ANY;
	if(strcmp(text, "none") == 0)
		return TERRAIN_NONE;
	if(strcmp(text, "mountain") == 0)
		return TERRAIN_MOUNTAIN;
	if(strcmp(text, "temperate_freshwater_lake") == 0)
		return TERRAIN_LAKE_TEMP_FRESH;
	if(strcmp(text, "temperate_brackish_lake") == 0)
		return TERRAIN_LAKE_TEMP_BRACK;
	if(strcmp(text, "temperate_saltwater_lake") == 0)
		return TERRAIN_LAKE_TEMP_SALT;
	if(strcmp(text, "tropical_freshwater_lake") == 0)
		return TERRAIN_LAKE_TROP_FRESH;
	if(strcmp(text, "tropical_brackish_lake") == 0)
		return TERRAIN_LAKE_TROP_BRACK;
	if(strcmp(text, "tropical_saltwater_lake") == 0)
		return TERRAIN_LAKE_TROP_SALT;
	if(strcmp(text, "arctic_ocean") == 0)
		return TERRAIN_OCEAN_ARCT;
	if(strcmp(text, "tropical_ocean") == 0)
		return TERRAIN_OCEAN_TROP;
	if(strcmp(text, "temperate_ocean") == 0)
		return TERRAIN_OCEAN_TEMP;
	if(strcmp(text, "glacier") == 0)
		return TERRAIN_GLACIER;
	if(strcmp(text, "tundra") == 0)
		return TERRAIN_TUNDRA;
	if(strcmp(text, "temperate_freshwater_swamp") == 0)
		return TERRAIN_SWAMP_TEMP_FRESH;
	if(strcmp(text, "temperate_saltwater_swamp") == 0)
		return TERRAIN_SWAMP_TEMP_SALT;
	if(strcmp(text, "temperate_freshwater_marsh") == 0)
		return TERRAIN_MARSH_TEMP_FRESH;
	if(strcmp(text, "temperate_saltwater_marsh") == 0)
		return TERRAIN_MARSH_TEMP_SALT;
	if(strcmp(text, "tropical_freshwater_swamp") == 0)
		return TERRAIN_SWAMP_TROP_FRESH;
	if(strcmp(text, "tropical_saltwater_swamp") == 0)
		return TERRAIN_SWAMP_TROP_SALT;
	if(strcmp(text, "mangrove_swamp") == 0)
		return TERRAIN_SWAMP_MANGROVE;
	if(strcmp(text, "tropical_freshwater_marsh") == 0)
		return TERRAIN_MARSH_TROP_FRESH;
	if(strcmp(text, "tropical_saltwater_marsh") == 0)
		return TERRAIN_MARSH_TROP_SALT;
	if(strcmp(text, "taiga_forest") == 0)
		return TERRAIN_FOREST_TAIGA;
	if(strcmp(text, "temperate_conifer_forest") == 0)
		return TERRAIN_FOREST_TEMP_FIR;
	if(strcmp(text, "temperate_broadleaf_forest") == 0)
		return TERRAIN_FOREST_TEMP_BROAD;
	if(strcmp(text, "tropical_conifer_forest") == 0)
		return TERRAIN_FOREST_TROP_FIR;
	if(strcmp(text, "tropical_dry_broadleaf_forest") == 0)
		return TERRAIN_FOREST_TROP_BROAD_DRY;
	if(strcmp(text, "tropical_moist_broadleaf_forest") == 0)
		return TERRAIN_FOREST_TROP_BROAD_MOIST;
	if(strcmp(text, "temperate_grassland") == 0)
		return TERRAIN_GRASS_TEMP;
	if(strcmp(text, "temperate_savanna") == 0)
		return TERRAIN_SAV_TEMP;
	if(strcmp(text, "temperate_shrubland") == 0)
		return TERRAIN_SHRUB_TEMP;
	if(strcmp(text, "tropical_grassland") == 0)
		return TERRAIN_GRASS_TROP;
	if(strcmp(text, "tropical_savanna") == 0)
		return TERRAIN_SAV_TROP;
	if(strcmp(text, "tropical_shrubland") == 0)
		return TERRAIN_SHRUB_TROP;
	if(strcmp(text, "badland_desert") == 0)
		return TERRAIN_DESERT_BAD;
	if(strcmp(text, "sand_desert") == 0)
		return TERRAIN_DESERT_SAND;
	if(strcmp(text, "rock_desert") == 0)
		return TERRAIN_DESERT_ROCK;
	if(strcmp(text, "tall_mountain") == 0)
		return TERRAIN_MOUNTAIN_TALL;
	if(strcmp(text, "temperate_beach") == 0)
		return TERRAIN_BEACH_TEMP;
	if(strcmp(text, "tropical_beach") == 0)
		return TERRAIN_BEACH_TROP;
	if(strcmp(text, "arctic_beach") == 0)
		return TERRAIN_BEACH_ARCT;
	return TERRAIN_ANY;
}


structure_type get_structure_type(const char * text)
{
	if(strcmp(text, "any") == 0)
		return STRUCTURE_ANY;
	if(strcmp(text, "none") == 0)
		return STRUCTURE_NONE;
	if(strcmp(text, "castle") == 0)
		return STRUCTURE_CASTLE;
	if(strcmp(text, "village") == 0)
		return STRUCTURE_VILLAGE;
	if(strcmp(text, "crops_1") == 0)
		return STRUCTURE_CROPS1;
	if(strcmp(text, "crops_2") == 0)
		return STRUCTURE_CROPS2;
	if(strcmp(text, "crops_3") == 0)
		return STRUCTURE_CROPS3;
	if(strcmp(text, "pasture") == 0)
		return STRUCTURE_PASTURE;
	if(strcmp(text, "meadow") == 0)
		return STRUCTURE_MEADOW;
	if(strcmp(text, "woodland") == 0)
		return STRUCTURE_WOODLAND;
	if(strcmp(text, "orchard") == 0)
		return STRUCTURE_ORCHARD;
	if(strcmp(text, "tunnel") == 0)
		return STRUCTURE_TUNNEL;
	if(strcmp(text, "stone_bridge") == 0)
		return STRUCTURE_BRIDGE_STONE;
	if(strcmp(text, "other_bridge") == 0)
		return STRUCTURE_BRIDGE_OTHER;
	if(strcmp(text, "stone_road") == 0)
		return STRUCTURE_ROAD_STONE;
	if(strcmp(text, "other_road") == 0)
		return STRUCTURE_ROAD_OTHER;
	if(strcmp(text, "stone_wall") == 0)
		return STRUCTURE_WALL_STONE;
	if(strcmp(text, "other_wall") == 0)
		return STRUCTURE_WALL_OTHER;
	if(strcmp(text, "river") == 0)
		return STRUCTURE_RIVER;
	if(strcmp(text, "brook") == 0)
		return STRUCTURE_BROOK;
	return STRUCTURE_NONE;
}

e_offset_type get_offset_type(const char * text)
{
	if(!text)
		return OFFSET_NONE;
	if(strcmp(text, "none") == 0)
		return OFFSET_NONE;
	if(strcmp(text, "path") == 0)
		return OFFSET_PATH;
	if(strcmp(text, "pair") == 0)
		return OFFSET_PAIR;
	if(strcmp(text, "six") == 0)
		return OFFSET_SIX;
	if(strcmp(text, "sixteen") == 0)
		return OFFSET_SIXTEEN;
	if(strcmp(text, "four") == 0)
		return OFFSET_FOUR;
	if(strcmp(text, "random") == 0)
		return OFFSET_RANDOM;
	return OFFSET_NONE;
}

int get_offset(e_offset_type type, char borders, s_map_block * block, unsigned char amount)
{
	switch(type)
	{
	case OFFSET_SIXTEEN:
		return get_border_offset_sixteen(borders);
	case OFFSET_PATH:
		return get_border_offset_path(borders);
	case OFFSET_PAIR:
		return get_border_offset_pair(borders);
	case OFFSET_SIX:
		return get_border_offset_six(borders);
	case OFFSET_FOUR:
		return get_border_offset_four(borders);
	case OFFSET_RANDOM:
		int off = amount*block->random;
		if(off >= amount)
			off=0;
		return off;
	}
	return 0;
}

s_sprite::s_sprite(void)
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	index = -1;
	origin_x = 0;
	origin_y = 0;
	column_height = 0;
	color_by=COLOR_NONE;
	color_source=SOURCE_ELEV;
	palette_number=0;
	offset_type =  OFFSET_NONE;
	offset_amount = 0;
	border_terrain = TERRAIN_ANY;
	border_structure = STRUCTURE_ANY;
}

c_tile::c_tile(void)
{
	height_max = 999;
	height_min = -999;
	special_terrain = TERRAIN_ANY;
	structure = STRUCTURE_NONE;
	priority = 0;
}

c_tile::~c_tile(void)
{
}

void c_tile::draw(float x, float y, int height, int bottom, int surface, s_map_block * block, TileSet * tileset, bool flip, e_layer drawlayer)
{
	switch (drawlayer)
	{
	case LAYER_BASE:
		//Draw ocean layer first.
		if(height < surface)
		{
			for(unsigned int i = 0; i < intermediate_sprites.size(); i++)
			{
				int num_sections = (surface - height) / intermediate_sprites.at(i).column_height;
				int bottom_section_height = (surface - height) % intermediate_sprites.at(i).column_height;
				int offset = get_offset(
					intermediate_sprites.at(i).offset_type,
					block->structure==STRUCTURE_NONE?
					(block->terrain_borders[intermediate_sprites.at(i).border_terrain?intermediate_sprites.at(i).border_terrain:block->terrain])
					:block->structure_borders[intermediate_sprites.at(i).border_structure?intermediate_sprites.at(i).border_structure:block->structure],
					block,
					intermediate_sprites.at(i).offset_amount);
				if(bottom_section_height)
				{
					draw_sprite(
						tileset,
						intermediate_sprites.at(i), 
						block,
						x, 
						y - surface + (num_sections * intermediate_sprites.at(i).column_height),
						intermediate_sprites.at(i).column_height - bottom_section_height,
						flip,
						offset,
						true);
				}
				for( int sec = 0; sec < num_sections; sec++)
				{
					draw_sprite(
						tileset,
						intermediate_sprites.at(i), 
						block,
						x, 
						y - surface + (sec * intermediate_sprites.at(i).column_height),
						0,
						flip,
						offset);
				}
			}
		}
		//next comes the bottom layers
		if ((height-bottom) <= 0)
		{
			for(unsigned int i = 0; i < top_sprites.size(); i++)
			{
				int offset = get_offset(
					top_sprites.at(i).offset_type,
					block->structure==STRUCTURE_NONE?
					(block->terrain_borders[top_sprites.at(i).border_terrain?top_sprites.at(i).border_terrain:block->terrain])
					:block->structure_borders[top_sprites.at(i).border_structure?top_sprites.at(i).border_structure:block->structure],
					block,
					top_sprites.at(i).offset_amount);
				draw_sprite(
					tileset,
					top_sprites.at(i),
					block,
					x,
					y - height,
					0,
					flip,
					offset);
			}
		}
		else
		{
			for(unsigned int i = 0; i < bottom_sprites.size(); i++)
			{
				int num_sections = (height - bottom) / bottom_sprites.at(i).column_height;
				int bottom_section_height = (height - bottom) % bottom_sprites.at(i).column_height;
				int offset = get_offset(
					bottom_sprites.at(i).offset_type,
					block->structure==STRUCTURE_NONE?
					(block->terrain_borders[bottom_sprites.at(i).border_terrain?bottom_sprites.at(i).border_terrain:block->terrain])
					:block->structure_borders[bottom_sprites.at(i).border_structure?bottom_sprites.at(i).border_structure:block->structure],
					block,
					bottom_sprites.at(i).offset_amount);
				if(bottom_section_height)
				{
					draw_sprite(
						tileset,
						bottom_sprites.at(i), 
						block,
						x, 
						y - height + (num_sections * bottom_sprites.at(i).column_height),
						bottom_sprites.at(i).column_height - bottom_section_height,
						flip,
						offset);
				}
				for( int sec = 0; sec < num_sections; sec++)
				{
					draw_sprite(
						tileset,
						bottom_sprites.at(i), 
						block,
						x, 
						y - height + (sec * bottom_sprites.at(i).column_height),
						0,
						flip,
						offset);
				}
			}
			for(unsigned int i = 0; i < top_sprites.size(); i++)
			{
				int offset = get_offset(
					top_sprites.at(i).offset_type,
					block->structure==STRUCTURE_NONE?
					(block->terrain_borders[top_sprites.at(i).border_terrain?top_sprites.at(i).border_terrain:block->terrain])
					:block->structure_borders[top_sprites.at(i).border_structure?top_sprites.at(i).border_structure:block->structure],
					block,
					top_sprites.at(i).offset_amount);
				draw_sprite(
					tileset,
					top_sprites.at(i),
					block,
					x,
					y - height,
					0,
					flip,
					offset);
			}
		}
		for(unsigned int i = 0; i < surface_sprites.size(); i++)
		{
			int offset = get_offset(
				surface_sprites.at(i).offset_type,
				block->structure==STRUCTURE_NONE?
				(block->terrain_borders[surface_sprites.at(i).border_terrain?surface_sprites.at(i).border_terrain:block->terrain])
				:block->structure_borders[surface_sprites.at(i).border_structure?surface_sprites.at(i).border_structure:block->structure],
				block,
				surface_sprites.at(i).offset_amount);
			draw_sprite(
				tileset,
				surface_sprites.at(i),
				block,
				x,
				y-surface,
				0,
				flip,
				offset);
		}
		break;
	case LAYER_OBJECT:
		for(unsigned int i = 0; i < object_sprites.size(); i++)
		{
			int offset = get_offset(
				object_sprites.at(i).offset_type,
				block->structure==STRUCTURE_NONE?
				(block->terrain_borders[object_sprites.at(i).border_terrain?object_sprites.at(i).border_terrain:block->terrain])
				:block->structure_borders[object_sprites.at(i).border_structure?object_sprites.at(i).border_structure:block->structure],
				block,
				object_sprites.at(i).offset_amount);
			draw_sprite(
				tileset,
				object_sprites.at(i),
				block,
				x,
				y-surface,
				0,
				flip,
				offset);
		}
		break;
	}
}

void c_tile::load_ini(ALLEGRO_PATH * path)
{
	ALLEGRO_CONFIG * config = 0;

	const char * thepath = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
	LogVerbose("Loading tile at: %s\n", thepath);
	config = al_load_config_file(thepath);
	if(!config)
	{
		LogError("Cannot load tile: %s\n", thepath);
		exit(1);
	}
	char buffer[256];

	height_max = get_config_int(config, "SPRITE", "height_max", 280);
	height_min = get_config_int(config, "SPRITE", "height_min", 0);

	priority = get_config_int(config, "SPRITE", "priority", 0);

	const char * terra = al_get_config_value(config, "SPRITE", "special_terrain");
	if(terra)
		special_terrain = get_terrain_type(terra);

	const char * cons = al_get_config_value(config, "SPRITE", "special_object");
	if(cons)
		structure = get_structure_type(cons);

	s_sprite temp;
	size_t cap_layers = get_config_int(config, "SPRITE", "cap_layers");
	for(size_t i = 0; i < cap_layers; i++)
	{
		sprintf(buffer, "CAP_IMAGE_%d", i);
		temp = get_from_ini(config, buffer, path);
		if(temp.index >= 0)
			top_sprites.push_back(temp);
	}

	size_t column_layers = get_config_int(config, "SPRITE", "column_layers");
	for(size_t i = 0; i < column_layers; i++)
	{
		sprintf(buffer, "COLUMN_IMAGE_%d", i);
		temp = get_from_ini(config, buffer, path);
		if(temp.index >= 0)
			bottom_sprites.push_back(temp);
	}

	size_t surface_layers = get_config_int(config, "SPRITE", "surface_layers");
	for(size_t i = 0; i < surface_layers; i++)
	{
		sprintf(buffer, "SURFACE_IMAGE_%d", i);
		temp = get_from_ini(config, buffer, path);
		if(temp.index >= 0)
			surface_sprites.push_back(temp);
	}

	size_t intermediate_layers = get_config_int(config, "SPRITE", "intermediate_layers");
	for(size_t i = 0; i < intermediate_layers; i++)
	{
		sprintf(buffer, "INTERMEDIATE_IMAGE_%d", i);
		temp = get_from_ini(config, buffer, path);
		if(temp.index >= 0)
			intermediate_sprites.push_back(temp);
	}

	size_t object_layers = get_config_int(config, "SPRITE", "object_layers");
	for(size_t i = 0; i < object_layers; i++)
	{
		sprintf(buffer, "OBJECT_IMAGE_%d", i);
		temp = get_from_ini(config, buffer, path);
		if(temp.index >= 0)
			object_sprites.push_back(temp);
	}

}

s_sprite c_tile::get_from_ini(ALLEGRO_CONFIG *config, const char * section, ALLEGRO_PATH * base_path)
{
	s_sprite temp;

	const char * buffer_file = al_get_config_value(config, section, "image_file");
	if(!buffer_file)
	{
		temp.index = -1;
		return temp;
	}
	ALLEGRO_PATH * imagepath = al_create_path(buffer_file);
	al_rebase_path(base_path, imagepath);
	temp.index = imagelist.load_image(al_path_cstr(imagepath, ALLEGRO_NATIVE_PATH_SEP));

	temp.x = get_config_int(config, section, "x");
	temp.y = get_config_int(config, section, "y");

	temp.width = get_config_int(config, section, "width");
	temp.height = get_config_int(config, section, "height");

	temp.origin_x = get_config_int(config, section, "origin_x");
	temp.origin_y = get_config_int(config, section, "origin_y");

	temp.origin_x = 0 - temp.origin_x;
	temp.origin_y = 0 - temp.origin_y;

	temp.palette_number = get_config_int(config, section, "palette_index");

	temp.column_height = get_config_int(config, section, "column_height");

	const char * color_selection = al_get_config_value(config, section, "color_source");
	if(color_selection)
		temp.color_by = get_color_selector(color_selection);

	const char * pal_source = al_get_config_value(config, section, "palette_source");
	if(pal_source)
		temp.color_source = get_color_source(pal_source);

	const char * color = al_get_config_value(config, section, "color_html");
	if(color)
		temp.color = color_html(color);

	const char * off = al_get_config_value(config, section, "offset_type");
	if(off)
		temp.offset_type = get_offset_type(off);

	const char * neigh = al_get_config_value(config, section, "border_terrain");
	if(neigh)
		temp.border_terrain = get_terrain_type(neigh);

	const char * neighbobj = al_get_config_value(config, section, "border_structure");
	if(neighbobj)
		temp.border_structure = get_structure_type(neighbobj);

	temp.offset_amount = get_config_int(config, section, "offset_amount");

	al_destroy_path(imagepath);
	return temp;
}