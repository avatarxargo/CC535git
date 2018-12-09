/**
 *	Title:	tm.h
 *	Author: David Hrusa
 *	Date:	9/14/2018
 *	Desc:	Represents a triangular mesh
 */

#pragma once
#include "v3.h"
#include "ppc.h"
#include "quat.h"
#include "aabb.h"
#include "framebuffer.h"
#include "renderable.h"

 class TriangleMesh : public Renderable
 {
 public:
	V3 *verts, *colors, *normals;
	AABB boundingBox;
	int vertsN;
	unsigned int *tris;
	int uv;
	float *tcs;
	int trisN;
	//
	float currentScale;
	Quaternion currentOrientation;
	//
	TriangleMesh() : verts(0), vertsN(0), colors(0), tris(0), trisN(0), boundingBox(), currentScale(1){
		type = REN_TM;
		currentScale = 1;
	};
	TriangleMesh(int size);
	TriangleMesh(int w, int h, int normal);
	void LoadBin(char *fname);
	AABB getBoundingBox();
	V3 GetCenter();
	//move 
	void translate(V3 translation);
	void translateRelative(V3 translation);
	//scales from the center.
	void scale(float _scale);
	void scaleRelative(float _scale);
	void scale(float scale, V3 center);
	//places the center at position and scales the dimensions to scale
	void position(V3 position, float scale);
	void renderPoints(PPC *ppc, FrameBuffer *fb);
	void renderWireframe(PPC *ppc, FrameBuffer *fb);
	void renderFill(PPC *ppc, FrameBuffer *fb);
	void renderFillTextured(PPC *ppc, FrameBuffer *fb, Texture *tex);
	void renderFillTexturedLit(PPC * ppc, FrameBuffer *fb, Material *mat);
	void renderFillTexturedLitShadow(PPC * ppc, FrameBuffer *fb, Material *mat);
	void renderFillEnvMap(PPC * ppc, FrameBuffer *fb, ShadowMapNS::ShadowMap *map);
	void renderFillClip(PPC *ppc, FrameBuffer *fb);
	//from renderable
	void drawUnlit(PPC* ppc, FrameBuffer* fb);
	void drawPerspective(PPC* ppc, FrameBuffer* fb);
	void drawuv(PPC* ppc, FrameBuffer* fb);
	void drawScreenspace(PPC* ppc, FrameBuffer* fb);
	//
	void RenderHW();
	//
	void SetRectangle(V3 O, float rw, float rh);
	V3 getCenterOfMass();
	//
	void allocate();
	void rotateAboutAxis(V3 O, V3 a, float angled);
	//
	void RayTrace(PPC *ppc, FrameBuffer* fb);
 };
