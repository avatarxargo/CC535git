#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>

#include "v3.h"
#include "m33.h"
#include "ppc.h"
#include "texture.h"
#include "Light.h"
#include "LightEnvironment.h"
#include "material.h"
#include <vector>
#include "scenic.h"
#include "glext.h"

namespace ShadowMapNS {
	class ShadowMap;
	enum ShadowDir;
}

enum FrameBufferMode {
	SW, GPU
};

class FrameBuffer : public Fl_Gl_Window {
public:
	unsigned int *pix;
	Scenic * myscene;
	FrameBufferMode bufferMode = SW;
	V3 *pixv;
	float * zbuffer;
	int * stencil;
	int w, h;
	LightEnvironment* lightEnvironment;
	//vector<Light*> lights;
	volatile bool exit = false;
	volatile float inx, iny, inz, ini, inj, inu, inp;
	volatile float inputs[20];
	volatile bool printCam = false, loadCam = false;
	FrameBuffer(int u0, int v0, int _w, int _h, Scenic * scene);
	FrameBuffer(int u0, int v0, int _w, int _h);
	void draw();
	void KeyUpHandle(bool down);
	void KeyboardHandle();
	void MouseHandle();
	int handle(int guievent);
	void refreshColor(unsigned int bgr);
	void refreshDepth(float bgr);
	void refreshStencil(int bgr);
	void SetPixel(float u, float v, unsigned int bgr);
	void SetPixelGuarded(float u, float v, unsigned int bgr);
	void drawSegment(V3 v0, V3 c0, V3 v1, V3 c1);
	void Set(int u, int v, unsigned int bgr);
	void setZ(int u, int v, float z, unsigned int colour);
	void setZBlend(int u, int v, float z, unsigned int colour, float alpha);
	void drawRect(float u0, float v0, float u1, float v1, unsigned int bgr);
	void drawTriangle(float u0, float v0, float u1, float v1, float u2, float v2, unsigned int bgr);
	void drawCircle(float u, float v, float r, unsigned int bgr);
	void LoadTiff(char* fname);
	void SaveAsTiff(char* fname);
	V3 clampVector(V3 v0);
	unsigned int* FrameBuffer::getDataPtr();
	V3* FrameBuffer::getDataPtrV3();
	//
	void DrawSegment(V3 p0, V3 c0, V3 p1, V3 c1);
	int Visible(int u, int v, float currz);
	//
	void draw3DPoint(V3 point, V3 color, PPC* camera, int size);
	void draw3DSegment(V3 point1, V3 color1, V3 point2, V3 color2, PPC* camera);
	void draw3DTriangle(V3 point1, V3 color1, V3 point2, V3 color2, V3 point3, V3 color3, PPC* camera);
	void draw3DTriangleTexturedScreenspace(V3 point1, V3 uvw1, V3 point2, V3 uvw2, V3 point3, V3 uvw3, PPC* camera, Material* mat);
	void draw3DTriangleTextured(V3 point1, V3 uvw1, V3 point2, V3 uvw2, V3 point3, V3 uvw3, PPC* camera, Material* mat);
	void draw3DTriangleTexturedLit(V3 point1, V3 uvw1, V3 normal1, V3 point2, V3 uvw2, V3 normal2, V3 point3, V3 uvw3, V3 normal3, PPC* camera, Material* mat);
	void draw3DTriangleTexturedLitShadow(V3 point1, V3 uvw1, V3 normal1, V3 point2, V3 uvw2, V3 normal2, V3 point3, V3 uvw3, V3 normal3, PPC* camera, Material* mat);
	void draw3DTriangleEnvMap(V3 point1, V3 uvw1, V3 normal1, V3 point2, V3 uvw2, V3 normal2, V3 point3, V3 uvw3, V3 normal3, PPC* camera, ShadowMapNS::ShadowMap* map);
	//visualizes the contents of a shadowMap on a screen.
	void displayShadowMap(int x, int y, ShadowMapNS::ShadowMap* sm, ShadowMapNS::ShadowDir shadowDir);
	// pixels to V3;
	void pix2v3();
	//
	void addLight(Light* l);
	void removeLight(int idx);
	void fog(float start, float end, V3 color);
	void drawEnvironmentBG(float start, PPC* ppc, ShadowMapNS::ShadowMap* map);
	void input(int x, int y, int z);
	void input2(float i, float j, float u, float p);
	float getXin();
	float getYin();
	float getZin();
	float getIin();
	float getJin();
	float getUin();
	float getPin();
	void input(int idx, float vlaue);
	void inputPlus(int idx, float vlaue);
	float getInput(int idx);
	void resetInput();
	bool getPrintCam();
	bool getLoadCam();

	void pullFromGPU();
};