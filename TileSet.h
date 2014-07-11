#pragma once

#include "common.h"
#include <vector>
#include "c_tile.h"
#include "s_map_block.h"

class DetailedTile;

enum DetailDrawMode {
    NORMAL,
    MAX_ELEVATION,
    CURRENT_ELEVATION,
};


class TileSet
{
public:
	TileSet(void);

	void load_ini(ALLEGRO_PATH * _path);

	void load_palette(ALLEGRO_BITMAP *);

	c_tile * get_tile(s_map_block block);
	c_tile * get_structure_tile(s_map_block block);

	vector<vector<ALLEGRO_COLOR>> palette;
	ALLEGRO_COLOR get_palette_color(int x, int y);
	vector<c_tile> tile_set;
	c_tile grid_tile;
	int tile_width;
	int tile_height;
	int snap_height;

    //variables controlling how the detailed tiles are drawn.
    std::string tileset_folder;
    DetailDrawMode draw_mode;
    int elevation_palette;
    int water_depth_palette;
    bool draw_water;
};
