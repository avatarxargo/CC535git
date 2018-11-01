#pragma once
/**
 *	Title:	renderable.h
 *	Author: David Hrusa
 *	Date:	10/18/2018
 *	Desc:	Allows all scene objects to be displayed to be stored within a single array.
 */
#include "ppc.h"
#include "material.h"

enum RenderMode { UNLIT, UV, SCREENSPACE, PERSPECTIVE };

enum RenderableType { REN_PLANE, REN_TM };

class Renderable {
public:
	Material* material;
	RenderMode renderMode = PERSPECTIVE;
	RenderableType type = REN_PLANE;
	//We let each object implement its own thing
	virtual void drawUnlit(PPC* ppc, FrameBuffer* fb) = 0;
	virtual void drawPerspective(PPC* ppc, FrameBuffer* fb) = 0;
	virtual void drawuv(PPC* ppc, FrameBuffer* fb) = 0;
	virtual void drawScreenspace(PPC* ppc, FrameBuffer* fb) = 0;
	void draw(PPC* ppc, FrameBuffer* fb) {
		switch(renderMode) {
		case PERSPECTIVE:
			drawPerspective(ppc, fb);
			break;
		case UNLIT:
			drawUnlit(ppc, fb);
			break;
		case UV:
			drawuv(ppc, fb);
			break;
		case SCREENSPACE:
			drawScreenspace(ppc, fb);
			break;
		}
	}
	//
	void setMaterial(Material* mat) {
		material = mat;
	}
	Material* getMaterial() {
		return material;
	}
	//
	void setMode(RenderMode mode) {
		renderMode = mode;
	}
	RenderableType getType() {
		return type;
	}
};