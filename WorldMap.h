#pragma once

#include "common.h"
#include "c_imagelist.h"
#include "s_map_block.h"
#include "TileSet.h"
#include "UserConfig.h"

terrain_type GetTerrainType(df::region_map_entry * region);
terrain_type GetTerrainType(df::coord2d pos, df::world_data * data, int zoom = 0);



class WorldMap
{
    std::vector<std::vector<s_map_block>> world_map_section;
    bool inited;
    UserConfig user_config;

    c_imagelist imagelist;

    vector<TileSet> tileset_list;
    int current_tileset;

    void Init();
    void load_tilesets(const char * index_file);
    void SetupMapSection();
    terrain_type GetTerrainType(df::coord2d pos, df::world_data * data, int zoom);
    terrain_type GetTerrainType(df::region_map_entry * region);
    void DrawRegionMap(df::coord2d center);
    void DrawEmbarkTileMap(df::coord2d center);
    s_map_block MakeBlock(df::coord2d pos, df::world_data * data, int zoom);
    void propogate_tiles(df::coord2d center, int zoom);

public:
    void Paintboard();

};

extern WorldMap worldMap;