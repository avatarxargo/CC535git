#pragma once

#include "gui.h"
#include "quat.h"
#include "renderable.h"
#include "tm.h"
#include "framebuffer.h"
#include "videoSaver.h"
#include "plane.h"
#include "lightEnvironment.h"
#include "material.h"
#include "shadowmap.h"
//Still broken... no documentation for their stupid library.
//#include "VideoCapturePlus.h"
#include <fstream>
#include <thread>
#include <string>

class Scene {
public:
	bool fpsConrols = true;
	GUI *gui;
	FrameBuffer *fb, *subfb;
	PPC *camera, *cam1, *cam2, *cam3, *vizcam;
	TriangleMesh *tm1, *tm2, *tm3, *tm4, *tm5;
	Plane *p0, *p1, *p0b, *p1b, *p2, *p2b, *floor, *groundMesh, *phamster, *phamsterb, *tstplane, *tstplanebil;
	vector<Renderable*> objects;
	Material *tiles, *rikako, *yumemi, *mesh, *ground, *hamster, *hamsterBil, *tst, *tstbil, *wood1, *wood2, *wood1b, *wood2b;
	ShadowMapNS::ShadowMap *tstShadow;
	Scene();
	bool vizcamena = false;
	void DBG();
	void Run();
	void Save();
	void Render();
	void drawGrid();
	void cameraControl(); 
	void cameraControlFPS();
	void cinematicCamera(bool save);
	void animateScene();
	Renderable* getLastRenderable();
	void addRenderable(Renderable* renderable);
	void renderSceneObjects(PPC* ppc, FrameBuffer* fb);
};

extern Scene *scene;