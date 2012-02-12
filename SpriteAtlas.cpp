#include "SpriteAtlas.h"

c_sprite_atlas::c_sprite_atlas()
{
	sprite_atlas = 0;
	atlas_width = 4096;
	atlas_height = 4096;
	lowest_free = 0;
}

c_sprite_atlas::~c_sprite_atlas()
{
	for(int i = 0; i < usage.size(); i++)
	{
		if(usage[i])
			delete_sprite(usage[i]);
	}
	usage.clear();
	al_destroy_bitmap(sprite_atlas);
}

bool c_sprite_atlas::create_atlas()
{
	sprite_atlas = al_create_bitmap(atlas_width, atlas_height);
	if(!sprite_atlas)
	{
		DFConsole->printerr("Selected sprite atlas size too big! failed to create!");
		return 0;
	}
	sprites_x = atlas_width / SPRITEWIDTH;
	sprites_y = atlas_height / FULLSPRITEHEIGHT;
	max_sprites = sprites_x * sprites_y;
	usage.resize(max_sprites);
	usage.clear();
	return 1;
}

s_atlassed_sprite * c_sprite_atlas::add_sprite(ALLEGRO_BITMAP * sprite)
{
	if(lowest_free >= max_sprites)
	{
		DFConsole->printerr("Out of space for sprites!");
		return 0;
	}
	s_atlassed_sprite * tempsprite = new s_atlassed_sprite;
	int sx;
	int sy;
	sx = lowest_free % sprites_x;
	sy = lowest_free / sprites_x;
	tempsprite->sprite = al_create_sub_bitmap(sprite_atlas, sx*SPRITEWIDTH, sy*FULLSPRITEHEIGHT, SPRITEWIDTH, FULLSPRITEHEIGHT);
	tempsprite->index = lowest_free;

	usage[lowest_free] = tempsprite;

	//find the next empty spot
	for (; lowest_free < max_sprites; lowest_free++)
	{
		if (!usage[lowest_free]) break;
	}
	return tempsprite;
}

void c_sprite_atlas::delete_sprite(s_atlassed_sprite * sprite)
{
	int tempdex = sprite->index;
	if(tempdex < lowest_free) lowest_free = tempdex; //If this is a lower index than the previous empty spot, update the info.
	al_destroy_bitmap(sprite->sprite);
	delete usage[tempdex];
	usage[tempdex] = 0;
}