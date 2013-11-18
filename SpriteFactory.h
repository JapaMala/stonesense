#pragma once

#include "common.h"

class SpriteProxy
{
private:
    ALLEGRO_BITMAP * parent;
    float scale;
public:
    void queue_sprite(int x, int y);
    void queue_sprite(int x, int y, ALLEGRO_COLOR tint);
    void queue_sprite(int x, int y, ALLEGRO_COLOR tint, float scale);
};


class SpriteFactory
{
private:
    ALLEGRO_BITMAP * atlas;
    vector<SpriteProxy> spritelist;
public:
    void initialize();
    void queue_sprite(int index, int x, int y);
    void queue_sprite(int index, int x, int y, ALLEGRO_COLOR tint);
    void queue_sprite(int index, int x, int y, ALLEGRO_COLOR tint, float scale);
};
