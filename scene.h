#pragma once

#include "gui.h"
#include "quat.h"
#include "tm.h"
#include "framebuffer.h"
#include "videoSaver.h"
#include "plane.h"
//Still broken... no documentation for their stupid library.
//#include "VideoCapturePlus.h"
#include <fstream>
#include <thread>
#include <string>

class Scene {
public:
	
	GUI *gui;
	FrameBuffer *fb, *subfb;
	PPC *camera, *cam1, *cam2, *vizcam;
	TriangleMesh *tm1, *tm2, *tm3, *tm4, *tm5;
	Plane *p0, *p1, *p0b, *p1b, *p2, *p2b, *floor;
	Texture *rikako, *yumemi, *mesh;
	Scene();
	bool vizcamena = false;
	void DBG();
	void Save();
	void Render();
	void drawGrid();
	void cameraControl(); 
	void cameraControlFPS();
	void cinematicCamera(bool save);
	void animateScene();
};

extern Scene *scene;