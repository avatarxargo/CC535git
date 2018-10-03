#pragma once

#include "gui.h"
#include "quat.h"
#include "tm.h"
#include "framebuffer.h"
#include "videoSaver.h"
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
	Scene();
	void DBG();
	void Save();
	void Render();
	void drawGrid();
	void cameraControl();
	void cinematicCamera(bool save);
	void animateScene();
};

extern Scene *scene;