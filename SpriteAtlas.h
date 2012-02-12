#ifndef SPRITE_ATLAS_H
#define SPRITE_ATLAS_H

#include "common.h"
#include <vector>


struct s_atlassed_sprite
{
	ALLEGRO_BITMAP * sprite; //should be a sub-bitmap of the atlas, but does not need to be.
	int index;
};
class c_sprite_atlas
{
	int sprites_x; //number of sprites being stored on the atlas, in each direction
	int sprites_y;
	int atlas_width; // width and height of the actual image being used.
	int atlas_height;

	int lowest_free;
	vector<s_atlassed_sprite *> usage;
public:
	ALLEGRO_BITMAP * sprite_atlas;
	int max_sprites;

	c_sprite_atlas();
	~c_sprite_atlas();

	bool create_atlas();

	s_atlassed_sprite * add_sprite(ALLEGRO_BITMAP * sprite); //returns an index to the added sprite
	void delete_sprite(s_atlassed_sprite * sprite); //frees up an area on the atlas for later use
};


extern c_sprite_atlas * Sprite_Atlas;
//14464

#endif