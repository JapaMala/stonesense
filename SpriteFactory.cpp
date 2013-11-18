#include "SpriteFactory.h"
#include "WorldSegment.h"

void SpriteProxy::queue_sprite(int x, int y, ALLEGRO_COLOR tint, float scale)
{

}

void SpriteFactory::queue_sprite(int index, int x, int y)
{
    spritelist[index].queue_sprite(x, y);
}
void SpriteFactory::queue_sprite(int index, int x, int y, ALLEGRO_COLOR tint)
{
    spritelist[index].queue_sprite(x, y, tint);
}
void SpriteFactory::queue_sprite(int index, int x, int y, ALLEGRO_COLOR tint, float scale)
{
    spritelist[index].queue_sprite(x, y, tint, scale);
}
