#pragma once

#include <vector>
#include "common.h"
#include <algorithm>
#include "console.h"

using namespace std;
bool compare_size( ALLEGRO_BITMAP * first, ALLEGRO_BITMAP * second );

class c_imagelist
{
	vector<ALLEGRO_BITMAP*> image_list;
	vector<string * > image_filenames;
public:
	c_imagelist(void);
	void unload_bitmaps(void);
	ALLEGRO_BITMAP * get_image(unsigned int index);
	size_t load_image(const char* filename);
};

extern c_imagelist imagelist;