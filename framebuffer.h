#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>

#include "v3.h"
#include "ppc.h"

class FrameBuffer : public Fl_Gl_Window {
public:
	unsigned int *pix;
	float * zbuffer;
	int * stencil;
	int w, h;
	volatile int inx, iny, inz, ini, inj, inu, inp;
	volatile bool printCam = false, loadCam = false;
	FrameBuffer(int u0, int v0, int _w, int _h);
	void draw();
	void KeyboardHandle();
	int handle(int guievent);
	void refreshColor(unsigned int bgr);
	void refreshDepth(float bgr);
	void refreshStencil(int bgr);
	void SetPixel(float u, float v, unsigned int bgr);
	void SetPixelGuarded(float u, float v, unsigned int bgr);
	void drawSegment(V3 v0, V3 c0, V3 v1, V3 c1);
	void Set(int u, int v, unsigned int bgr);
	void setZ(int u, int v, float z, unsigned int colour);
	void drawRect(float u0, float v0, float u1, float v1, unsigned int bgr);
	void drawTriangle(float u0, float v0, float u1, float v1, float u2, float v2, unsigned int bgr);
	void drawCircle(float u, float v, float r, unsigned int bgr);
	void LoadTiff(char* fname);
	void SaveAsTiff(char* fname);
	V3 clampVector(V3 v0);
	unsigned int* FrameBuffer::getDataPtr();
	//
	void DrawSegment(V3 p0, V3 c0, V3 p1, V3 c1);
	int Visible(int u, int v, float currz);
	//
	void draw3DPoint(V3 point, V3 color, PPC* camera, int size);
	void draw3DSegment(V3 point1, V3 color1, V3 point2, V3 color2, PPC* camera);
	void draw3DTriangle(V3 point1, V3 color1, V3 point2, V3 color2, V3 point3, V3 color3, PPC* camera);
	//
	void input(int x, int y, int z);
	void input2(int i, int j, int u, int p);
	int getXin();
	int getYin();
	int getZin();
	int getIin();
	int getJin();
	int getUin();
	int getPin();
	bool getPrintCam();
	bool getLoadCam();
};