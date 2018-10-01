/**
 *	Title:	aabb.h
 *	Author: David Hrusa
 *	Date:	9/14/2018
 *	Desc:	An Axis Aligned Bounding Box
 */

#pragma once

#include "v3.h"

 //forward declaration
class PPC;
class FrameBuffer;

class AABB
{
public:
	V3 c0, c1;
	bool initialized;
	//
	AABB();
	AABB(V3 firstPoint);
	void render(PPC* ppc, FrameBuffer* fb);
	void AddPoint(V3 p);
	float GetDiagonal();
	int clip2D(float l, float r, float t, float b);
};

