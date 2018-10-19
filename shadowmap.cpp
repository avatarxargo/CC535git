#include "shadowmap.h"

ShadowMap::ShadowMap(V3 _pos) {
	pos = V3(_pos);
	/* top is +y
	 * north is +z
	 * east is +x */
	north = new PPC(V3(1,0,0), V3(0, -1, 0), V3(-1,1,1), pos);
	//
	map_top = new float[mapResX * mapResY];
	map_bot = new float[mapResX * mapResY];
	map_north = new float[mapResX * mapResY];
	map_east = new float[mapResX * mapResY];
	map_south = new float[mapResX * mapResY];
	map_west = new float[mapResX * mapResY];
}