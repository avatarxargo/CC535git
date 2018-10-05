#pragma once

#include "v3.h"
/**
 *	Title:	Light.h
 *	Author: David Hrusa
 *	Date:	10/2/2018
 *	Desc:	Stores information about a light source.
 */
class Light {
public:
	V3 position, color;
	float rangeStart, rangeMax;
	Light(V3 color);
	Light(V3 pos, V3 color, float rangeStart, float rangeMax);
};