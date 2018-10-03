#include "plane.h"
/**
 *	Title:	plane.cpp
 *	Author: David Hrusa
 *	Date:	10/2/2018
 *	Desc:	Makes rendering squares with textures more convenient
 */

Plane::Plane(V3 centre, V3 up, V3 left, Texture* _tex) {
	tex = _tex;
	a = centre + up + left;
	b = centre + up - left;
	c = centre - up + left;
	d = centre - up - left;
	uva = V3(0, 0, 0);
	uvb = V3(1, 0, 0);
	uvc = V3(0, 1, 0);
	uvd = V3(1, 1, 0);
}

void Plane::draw(PPC* ppc, FrameBuffer* fb) {
	fb->draw3DTriangleTextured(c, uvc, b, uvb, a, uva, ppc, tex);
	fb->draw3DTriangleTextured(c, uvc, d, uvd, b, uvb, ppc, tex);
}

void Plane::drawScreenspace(PPC* ppc, FrameBuffer* fb) {
	fb->draw3DTriangleTexturedScreenspace(c, uvc, b, uvb, a, uva, ppc, tex);
	fb->draw3DTriangleTexturedScreenspace(c, uvc, d, uvd, b, uvb, ppc, tex);
}

void Plane::drawuv(PPC* ppc, FrameBuffer* fb) {
	fb->draw3DTriangle(c, uvc, b, uvb, a, uva, ppc);
	fb->draw3DTriangle(c, uvc, d, uvd, b, uvb, ppc);
}