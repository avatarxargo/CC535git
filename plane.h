#pragma once
/**
 *	Title:	plane.h
 *	Author: David Hrusa
 *	Date:	10/2/2018
 *	Desc:	Makes rendering squares with textures more convenient
 */
#include "framebuffer.h"
#include "ppc.h"
#include "material.h"
#include "v3.h"

class Plane {
public:
	V3 a, b, c, d;
	V3 uva, uvb, uvc, uvd;
	V3 na, nb, nc, nd;
	int uvtilew, uvtileh;
	Material* mat;
	Plane(V3 centre, V3 up, V3 left, Material* _mat);
	void drawUnlit(PPC* ppc, FrameBuffer* fb);
	void draw(PPC* ppc, FrameBuffer* fb);
	void drawuv(PPC* ppc, FrameBuffer* fb);
	void drawScreenspace(PPC* ppc, FrameBuffer* fb);
	void setUV(int w, int h);
};