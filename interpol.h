#pragma once
/**
 *	Title:	interpol.h
 *	Author: David Hrusa
 *	Date:	10/29/2018
 *	Desc:	Container for the current iteration over some space (model / screen). This serves as abstraction to clarify design and to pass data to shaders / textures etc.
 */
#include "v3.h"

class InterpolationCore {
public:
	//anchors
	V3 aPos, bPos, cPos;
	V3 aNor, bNor, cNor;
	V3 aUv, bUv, cUv;
	//interpolated values
	V3 iPos;
	V3 iNor;
	V3 iUv;
};