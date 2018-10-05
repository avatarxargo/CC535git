#include "plane.h"
/**
 *	Title:	plane.cpp
 *	Author: David Hrusa
 *	Date:	10/2/2018
 *	Desc:	Makes rendering squares with textures more convenient
 */

Plane::Plane(V3 centre, V3 up, V3 left, Material* _mat) {
	mat = _mat;
	a = centre + up + left;
	b = centre + up - left;
	c = centre - up + left;
	d = centre - up - left;
	setUV(1,1);
	na = nb = nc = nd = (up % left).norm();
}

void Plane::drawUnlit(PPC* ppc, FrameBuffer* fb) {
	fb->draw3DTriangleTextured(c, uvc, b, uvb, a, uva, ppc, mat);
	fb->draw3DTriangleTextured(c, uvc, d, uvd, b, uvb, ppc, mat);
}

void Plane::draw(PPC* ppc, FrameBuffer* fb) {
	fb->draw3DTriangleTexturedLit(c, uvc, nc, b, uvb, nb, a, uva, na, ppc, mat);
	fb->draw3DTriangleTexturedLit(c, uvc, nc, d, uvd, nd, b, uvb, nb, ppc, mat);
}

void Plane::drawScreenspace(PPC* ppc, FrameBuffer* fb) {
	fb->draw3DTriangleTexturedScreenspace(c, uvc, b, uvb, a, uva, ppc, mat);
	fb->draw3DTriangleTexturedScreenspace(c, uvc, d, uvd, b, uvb, ppc, mat);
}

void Plane::drawuv(PPC* ppc, FrameBuffer* fb) {
	fb->draw3DTriangle(c, uvc, b, uvb, a, uva, ppc);
	fb->draw3DTriangle(c, uvc, d, uvd, b, uvb, ppc);
}

void Plane::setUV(int w, int h) {
	uvtilew = w;
	uvtileh = h;
	uva = V3(0, 0, 0);
	uvb = V3(w, 0, 0);
	uvc = V3(0, h, 0);
	uvd = V3(w, h, 0);
}