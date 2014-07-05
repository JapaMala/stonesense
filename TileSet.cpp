#include "TileSet.h"
#include "UserConfig.h"

TileSet::TileSet(void)
{
	snap_height = 1;
	palette.resize(1);
	palette[0].resize(1);
}

//TileSet::~TileSet(void)
//{
//}

void TileSet::load_ini(ALLEGRO_PATH * path)
{
	ALLEGRO_CONFIG * config = 0;

	//const char * figgin = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);
	config = al_load_config_file(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
	if(!config)
		return;
    tileset_folder = al_get_path_tail(path);
    LogVerbose("loading tileset from: %s\n", tileset_folder.c_str());

	tile_width = get_config_int(config, "TILESET_PROPERTIES", "tile_width");
	tile_height = get_config_int(config, "TILESET_PROPERTIES", "tile_height");
	snap_height = get_config_int(config, "TILESET_PROPERTIES", "snap_height");

    snap_height = get_config_int(config, "DETAILED_TILES", "elevation_palette", -1);
    water_depth_palette = get_config_int(config, "DETAILED_TILES", "water_depth_palette_index", -1);

    if(const char * mode = al_get_config_value(config, "DETAILED_TILES", "draw_mode")) {
        if(strcmp(mode, "normal") == 0) draw_mode = NORMAL;
        if(strcmp(mode, "max_elevation") == 0) draw_mode = MAX_ELEVATION;
        if(strcmp(mode, "current_elevation") == 0) draw_mode = CURRENT_ELEVATION;
    }
    else draw_mode = NORMAL;

    if(const char * water = al_get_config_value(config, "DETAILED_TILES", "skip_water")) {
        if(strcmp(water, "yes") == 0) draw_water = false;
        if(strcmp(water, "no") == 0) draw_water = true;
    }
    else draw_water = true;

	const char * file = al_get_config_value(config, "TILESET_PROPERTIES", "grid_tile");
	if(file)
	{
		ALLEGRO_PATH * tilepath = al_create_path(file);
		al_rebase_path(path, tilepath);
		grid_tile.load_ini(tilepath);
		al_destroy_path(tilepath);
	}

	ALLEGRO_BITMAP * palettemap = 0;
	const char * palette = al_get_config_value(config, "TILESET_PROPERTIES", "palette_file");
	if(palette)
	{
		ALLEGRO_PATH * tilepath = al_create_path(palette);
		al_rebase_path(path, tilepath);
		palettemap = al_load_bitmap(al_path_cstr(tilepath, ALLEGRO_NATIVE_PATH_SEP));
		al_destroy_path(tilepath);
	}
	load_palette(palettemap);
	al_destroy_bitmap(palettemap);

	const char * dir = al_get_config_value(config, "TILESET_PROPERTIES", "tile_dir");
	if(dir)
	{
		ALLEGRO_PATH * tile_dir = al_create_path(dir);
		al_rebase_path(path, tile_dir);
		ALLEGRO_FS_ENTRY * fs_dir = al_create_fs_entry(al_path_cstr(tile_dir, ALLEGRO_NATIVE_PATH_SEP));
		if(al_open_directory(fs_dir))
		{
			ALLEGRO_FS_ENTRY * fs_tile;
			while(fs_tile = al_read_directory(fs_dir))
			{
				ALLEGRO_PATH * tile_path = al_create_path(al_get_fs_entry_name(fs_tile));
				if(strcmp(al_get_path_extension(tile_path), ".ini") == 0)
				{
					c_tile temp_tile;
					temp_tile.load_ini(tile_path);
					if(!(
						temp_tile.bottom_sprites.empty() &&
						temp_tile.surface_sprites.empty() &&
						temp_tile.top_sprites.empty() &&
						temp_tile.object_sprites.empty())
						)
						tile_set.push_back(temp_tile);
				}
				al_destroy_path(tile_path);
				al_destroy_fs_entry(fs_tile);
			}
			al_destroy_fs_entry(fs_tile);
			al_close_directory(fs_dir);
		}
		al_destroy_fs_entry(fs_dir);
		std::sort(tile_set.begin(), tile_set.end());
	}
}

c_tile * TileSet::get_tile(s_map_block block)
{
	if(tile_set.empty())
		return 0;
	for(int i = 0; i < tile_set.size(); i++)
	{
		if(
			(tile_set[i].special_terrain == TERRAIN_ANY || tile_set[i].special_terrain == block.terrain)&&
			tile_set[i].structure == STRUCTURE_NONE &&
			tile_set[i].height_min <= (block.height) &&
			tile_set[i].height_max >= (block.height)
			)
		{
			return &(tile_set.at(i));
		}
	}
	return 0;
}
c_tile * TileSet::get_structure_tile(s_map_block block)
{
	if(tile_set.empty())
		return 0;
	for(int i = 0; i < tile_set.size(); i++)
	{
		if(tile_set[i].structure != STRUCTURE_NONE && 
			(tile_set[i].structure == STRUCTURE_ANY || 
			tile_set[i].structure == block.structure))
		{
			return &(tile_set.at(i));
		}
	}
	return 0;
}

void TileSet::load_palette(ALLEGRO_BITMAP * palettemap)
{
	if(!palettemap)
	{
		palette.resize(1);
		palette[0].resize(1);
		palette[0][0] = al_map_rgb(255,0,255);
		return;
	}
	al_lock_bitmap(palettemap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
	palette.resize(al_get_bitmap_width(palettemap));
	for(int x = 0; x < palette.size(); x++)
	{
		palette[x].resize(al_get_bitmap_height(palettemap));
		for(int y = 0; y < palette[x].size(); y++)
		{
			palette[x][y] = al_get_pixel(palettemap, x, y);
		}
	}
	al_unlock_bitmap(palettemap);
};

ALLEGRO_COLOR TileSet::get_palette_color(int x, int y)
{
	if(x < 0) x = 0;
	if(y < 0) y = 0;
	if(x >= palette.size()) x = palette.size()-1;
	if(y >= palette[x].size()) y = palette[x].size()-1;
	return palette[x][y];
}