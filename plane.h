#pragma once
/**
 *	Title:	plane.h
 *	Author: David Hrusa
 *	Date:	10/2/2018
 *	Desc:	Makes rendering squares with textures more convenient
 */
#include "framebuffer.h"
#include "ppc.h"
#include "texture.h"
#include "v3.h"

class Plane {
public:
	V3 a, b, c, d;
	V3 uva, uvb, uvc, uvd;
	Texture* tex;
	Plane(V3 centre, V3 up, V3 left, Texture* _tex);
	void draw(PPC* ppc, FrameBuffer* fb);
	void drawuv(PPC* ppc, FrameBuffer* fb);
	void drawScreenspace(PPC* ppc, FrameBuffer* fb);
};