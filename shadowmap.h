#pragma once
/**
 *	Title:	shadowmap.h
 *	Author: David Hrusa
 *	Date:	10/19/2018
 *	Desc:	Looks in 6 directions and casts shadows
 */
#include "v3.h"
#include "ppc.h"

/* top is +y
 * north is +z
 * east is +x */
class ShadowMap {
public:
	//resolution of the camera image
	const int mapResX = 100;
	const int mapResY = 100;
	V3 pos;
	//the directional cameras
	PPC *top, *bot, *north, *east, *south, *west;
	//the directional depth maps
	float *map_top, *map_bot, *map_north, *map_east, *map_south, *map_west;
	ShadowMap(V3 _pos);
	//
	//updates the shadowmap values
	void render();
	//for given point, returns how lit it is.
	float getMapValue(V3 point);
};