#include "WorldMap.h"

#include "UserConfig.h"
#include "c_imagelist.h"
#include "s_map_block.h"

#include "df/coord2d.h"
#include "df/world_data.h" 
#include "df/region_map_entry.h"
#include "df/world_region_details.h"
#include "df/moving_party.h"
#include "df/ui_advmode.h"

#include "GUI.h"

UserConfig user_config;

c_imagelist imagelist;

bool inited = false;

terrain_type GetTerrainType(int elevation, int rain, int drain, int temp)
{
    return terrain_type::TERRAIN_ANY;
}

void WorldMapInit()
{
    user_config.save_values();
    user_config.load_file();
    user_config.retrieve_values();
}

void DrawRegionMap(df::coord2d center)
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
void DrawEmbarkTileMap(df::coord2d center)
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

void WorldMapPaintboard()
{
    if (!inited)
    {
        WorldMapInit();
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

    if (df::global::ui_advmode && df::global::ui_advmode->travel_not_moved)
    {
        df::unit * adventurer = df::global::world->units.active[0];
        center.x = (df::global::world->map.region_x * 3) + adventurer->pos.x / 16;
        center.y = (df::global::world->map.region_y * 3) + adventurer->pos.y / 16;
        center.z = adventurer->pos.z;
    }
    else
    {
        center.x = data->travel[0]->pos.x;
        center.y = data->travel[0]->pos.y;
    }

    DrawEmbarkTileMap(center);
    draw_textf_border(font, uiColor(1), 0, 0, 0,
        "Adventurer Coords: %d, %d, %d", center.x, center.y, center.z);

}