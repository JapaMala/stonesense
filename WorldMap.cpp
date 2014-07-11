#include "WorldMap.h"


#include "df/coord2d.h"
#include "df/world_data.h" 
#include "df/region_map_entry.h"
#include "df/world_region_details.h"
#include "df/moving_party.h"
#include "df/ui_advmode.h"

#include "GUI.h"

#include "modules\Units.h"
#include "modules\Maps.h"

void WorldMap::load_tilesets(const char * index_file)
{
    ALLEGRO_CONFIG * config = 0;

    ALLEGRO_PATH * key = al_create_path(index_file);

    config = al_load_config_file(al_path_cstr(key, ALLEGRO_NATIVE_PATH_SEP));

    int num_tilesets = get_config_int(config, "TILESETS", "num_tilesets");

    char buffer[256];
    for (size_t i = 0; i < num_tilesets; i++)
    {
        sprintf(buffer, "tileset_%d", i);
        const char * file = al_get_config_value(config, "TILESETS", buffer);
        if (file)
        {
            ALLEGRO_PATH * temp = al_create_path(file);
            al_rebase_path(key, temp);
            TileSet temp_tileset;
            temp_tileset.load_ini(temp);
            tileset_list.push_back(temp_tileset);
            al_destroy_path(temp);
        }
    }
    al_destroy_path(key);
}

terrain_type WorldMap::GetTerrainType(df::coord2d pos, df::world_data * data, int zoom)
{
    df::coord2d posRegion;
    df::coord2d posEmbark;
    df::coord2d posBlock;

    posRegion = pos / 48;
    posEmbark = pos / 3;
    posBlock = pos;

    if (posRegion.x < 0 || posRegion.y < 0 || posRegion.x >= data->world_width || posRegion.y >= data->world_height)
        return terrain_type::TERRAIN_ANY;
    else return GetTerrainType(&data->region_map[posRegion.x][posRegion.y]);
}

terrain_type WorldMap::GetTerrainType(df::region_map_entry * region)
{
    if (region->elevation <= 99)
    {
        return terrain_type::TERRAIN_OCEAN_TEMP;
    }
    else if (region->elevation <= 200)
    {
        return terrain_type::TERRAIN_GRASS_TEMP;
    }
    else
    {
        return terrain_type::TERRAIN_MOUNTAIN;
    }
    return terrain_type::TERRAIN_ANY;
}

void WorldMap::SetupMapSection()
{
    world_map_section.resize(ssState.Size.x);
    for (int i = 0; i < ssState.Size.x; i++)
    {
        world_map_section[i].resize(ssState.Size.y);
    }
}

s_map_block WorldMap::MakeBlock(df::coord2d pos, df::world_data * data, int zoom)
{
    s_map_block block;
    df::coord2d posRegion;
    df::coord2d posEmbark;
    df::coord2d posBlock;

    posRegion = pos / 48;
    posEmbark = pos / 3;
    posBlock = pos;

    df::region_map_entry * region = &data->region_map[posRegion.x][posRegion.y];

    block.terrain = GetTerrainType(pos, data, zoom);
    block.height = region->elevation;


    //*********//
    block.sprite = tileset_list[0].get_tile(block);
    return block;
}

void WorldMap::propogate_tiles(df::coord2d center, int zoom)
{
    for (int x = 0; x < world_map_section.size(); x++)
    {
        for (int y = 0; y < world_map_section[x].size(); y++)
        {

        }
    }
}

void WorldMap::Init()
{
    user_config.save_values();
    user_config.load_file();
    user_config.retrieve_values();

    load_tilesets("isoworld/tilesets.ini");
    inited = true;
}

void WorldMap::DrawRegionMap(df::coord2d center)
{
    float x = ssState.ScreenW / 2.0f;
    float y = ssState.ScreenH / 2.0f;
    df::world_data * data = df::global::world->world_data;
    int tileWidth = 16;
    x -= (center.x/48.0f)*tileWidth;
    y -= (center.y/48.0f)*tileWidth;
    float min = 9999;
    float max = -999;
    for (int xx = 0; xx < data->world_width; xx++)
    {
        for (int yy = 0; yy < data->world_height; yy++)
        {
            df::region_map_entry * map = &(data->region_map[xx][yy]);
            float elev = (float)map->elevation / 280.0f;
            if (elev > max)
                max = elev;
            if (elev < min)
                min = elev;
            ALLEGRO_COLOR map_color = al_map_rgb_f(elev, 0, 1.0 - elev);
            al_draw_filled_rectangle(
                x + xx*tileWidth, y + yy*tileWidth,
                x + (xx + 1)*tileWidth, y + (yy+1)*tileWidth, map_color);
        }
    }
    //for (int i = 0; i < data->region_details.size(); i++)
    //{
    //    df::world_region_details * detail = data->region_details[i];
    //    if (detail)
    //    {
    //        int xx = x + detail->pos.x*tileWidth;
    //        int yy = (y - data->world_height * tileWidth) + detail->pos.y*tileWidth;
    //        for (int xxx = 0; xxx < 16; xxx++)
    //        {
    //            for (int yyy = 0; yyy < 16; yyy++)
    //            {
    //                al_draw_filled_rectangle(
    //                    xx + (xxx* (float)tileWidth / 16.0f), yy + (yyy*(float)tileWidth / 16.0f),
    //                    xx + ((xxx + 1)* (float)tileWidth / 16.0f), yy + ((yyy + 1)*(float)tileWidth / 16.0f),
    //                    al_map_rgb(detail->elevation[xxx][yyy], detail->elevation[xxx][yyy], detail->elevation[xxx][yyy]));
    //            }
    //        }
    //    }
    //}
    //PrintMessage("%f - %f\n", min, max);

}
void WorldMap::DrawEmbarkTileMap(df::coord2d center)
{
    float x = ssState.ScreenW / 2.0f;
    float y = ssState.ScreenH / 2.0f;
    df::world_data * data = df::global::world->world_data;
    int tileWidth = 16*16;
    x -= (center.x / 48.0f)*tileWidth;
    y -= (center.y / 48.0f)*tileWidth;
    float min = 9999;
    float max = -999;
    for (int xx = 0; xx < data->world_width; xx++)
    {
        for (int yy = 0; yy < data->world_height; yy++)
        {
            df::region_map_entry * map = &(data->region_map[xx][yy]);
            float elev = (float)(map->elevation-100) / 180.0f;
            if (elev > max)
                max = elev;
            if (elev < min)
                min = elev;
            ALLEGRO_COLOR map_color = al_map_rgb_f(elev, elev, elev);
            al_draw_filled_rectangle(
                x + xx*tileWidth, y + yy*tileWidth,
                x + (xx + 1)*tileWidth, y + (yy + 1)*tileWidth, map_color);
        }
    }
    for (int i = 0; i < data->region_details.size(); i++)
    {
        df::world_region_details * detail = data->region_details[i];
        if (detail)
        {
            int xx = x + detail->pos.x*tileWidth;
            int yy = y + detail->pos.y*tileWidth;
            for (int xxx = 0; xxx < 16; xxx++)
            {
                for (int yyy = 0; yyy < 16; yyy++)
                {
                    float elev = (float)(detail->elevation[xxx][yyy]-100) / 180.0f;
                    al_draw_filled_rectangle(
                        xx + (xxx* (float)tileWidth / 16.0f), yy + (yyy*(float)tileWidth / 16.0f),
                        xx + ((xxx + 1)* (float)tileWidth / 16.0f), yy + ((yyy + 1)*(float)tileWidth / 16.0f),
                        al_map_rgb_f(elev, elev, elev));
                }
            }
        }
    }

}

void WorldMap::Paintboard()
{
    if (!inited)
    {
        Init();
    }
    if (ssConfig.transparentScreenshots) {
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    }
    else {
        al_clear_to_color(ssConfig.backcol);
    }
    df::coord center;
    center.x = 0;
    center.y = 0;
    df::world_data * data = df::global::world->world_data;
    center.x = data->adv_region_x;
    center.y = data->adv_region_y;

    if (DFHack::Maps::IsValid())
    {
        df::unit * adventurer = df::global::world->units.active[0];
        center.x = (df::global::world->map.region_x * 3) + adventurer->pos.x / 16;
        center.y = (df::global::world->map.region_y * 3) + adventurer->pos.y / 16;
        center.z = adventurer->pos.z;
    }
    else
    {
        //center.x = data->travel[0]->pos.x;
        //center.y = data->travel[0]->pos.y;
    }

    DrawEmbarkTileMap(center);

    draw_textf_border(font, uiColor(1), 0, 0, 0,
        "Possible Adventurer name:%s", df::global::world->units.active[0]->name.first_name.c_str());

}