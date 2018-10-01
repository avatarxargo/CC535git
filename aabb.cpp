/**
 *	Title:	aabb.cpp
 *	Author: David Hrusa
 *	Date:	9/14/2018
 *	Desc:	An Axis Aligned Bounding Box
 */

#include "aabb.h"
#include "framebuffer.h"
#include "ppc.h"

AABB::AABB() {
	initialized = false;
}

AABB::AABB(V3 firstPoint) : c0(firstPoint), c1(firstPoint), initialized(true) {};

void AABB::render(PPC* ppc, FrameBuffer* fb) {
	V3 diff(c1 - c0);
	fb->draw3DSegment(c0 + V3(0, 0, 0), V3(1, 1, 0), c0 + V3(diff[0], 0, 0), V3(1, 1, 0), ppc);
	fb->draw3DSegment(c0 + V3(0, 0, 0), V3(1, 1, 0), c0 + V3(0, diff[1], 0), V3(1, 1, 0), ppc);
	fb->draw3DSegment(c0 + V3(0, 0, 0), V3(1, 1, 0), c0 + V3(0, 0, diff[2]), V3(1, 1, 0), ppc);
	//
	fb->draw3DSegment(c1 + V3(0, 0, 0), V3(1, 1, 0), c1 - V3(diff[0], 0, 0), V3(1, 1, 0), ppc);
	fb->draw3DSegment(c1 + V3(0, 0, 0), V3(1, 1, 0), c1 - V3(0, diff[1], 0), V3(1, 1, 0), ppc);
	fb->draw3DSegment(c1 + V3(0, 0, 0), V3(1, 1, 0), c1 - V3(0, 0, diff[2]), V3(1, 1, 0), ppc);
	//
	fb->draw3DSegment(c0 + V3(0, diff[1], 0), V3(1, 1, 0), c0 + V3(diff[0], diff[1], 0), V3(1, 1, 0), ppc);
	fb->draw3DSegment(c0 + V3(0, diff[1], 0), V3(1, 1, 0), c0 + V3(0, diff[1], diff[2]), V3(1, 1, 0), ppc);
	//
	fb->draw3DSegment(c1 - V3(0, diff[1], 0), V3(1, 1, 0), c1 - V3(diff[0], diff[1], 0), V3(1, 1, 0), ppc);
	fb->draw3DSegment(c1 - V3(0, diff[1], 0), V3(1, 1, 0), c1 - V3(0, diff[1], diff[2]), V3(1, 1, 0), ppc);
	//
	fb->draw3DSegment(c1 - V3(0, diff[1], diff[2]), V3(1, 1, 0), c1 - V3(0, 0, diff[2]), V3(1, 1, 0), ppc);
	fb->draw3DSegment(c1 - V3(diff[0], diff[1], 0), V3(1, 1, 0), c1 - V3(diff[0], 0, 0), V3(1, 1, 0), ppc);
	//fb->draw3DSegment(c0 + V3(0, 0, diff[2]), V3(1, 0, 0), c0 + V3(diff[0], 0, diff[2]), V3(1, 0, 0), ppc);
	//fb->draw3DSegment(c0 + V3(0, diff[1], 0), V3(1, 1, 0), c0 + V3(0, diff[1], diff[2]), V3(1, 1, 0), ppc);
}

void AABB::AddPoint(V3 p) {
	if (initialized) {
		for (int i = 0; i < 3; i++) {
			if (p[i] < c0[i])
				c0[i] = p[i];
			if (p[i] > c1[i])
				c1[i] = p[i];
		}
	}
	else {
		initialized = true;
		c0 = p;
		c1 = p;
	}

}


int AABB::clip2D(float l, float r, float t, float b) {
	if (c0[0] > r || c1[0] < l || c0[1] > b || c1[1] < t)
		return 0;
	if (c0[0] < l)
		c0[0] = l;
	if (c1[0] > r)
		c1[0] = r;
	if (c0[1] < b)
		c0[1] = b;
	if (c1[1] > t)
		c1[1] = t;
}

float AABB::GetDiagonal() {

	return (c1 - c0).len();

}
