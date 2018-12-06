#pragma once

#include "gui.h"
#include "quat.h"
#include "renderable.h"
#include "tm.h"
#include "videoSaver.h"
#include "plane.h"
#include "lightEnvironment.h"
#include "material.h"
//Still broken... no documentation for their stupid library.
//#include "VideoCapturePlus.h"
#include <fstream>
#include <thread>
#include <string>
#include <windows.h>
#include "sceneList.h"
#include "scenic.h"
#include "CGInterface.h"
#include "framebuffer.h"

class Scene: public Scenic {
public:
	bool fpsConrols = true;
	bool mouseMode = true;
	GUI *gui;
	FrameBuffer *fb, *subfb, *gpufb;
	SceneList *sceneList;
	CGInterface *cgi;
	ShaderOneInterface *soi;
	PPC *camera, *cam1, *cam2, *cam3, *vizcam;
	TriangleMesh *tm1, *tm2, *tm3, *tm4, *tm5, *gpucube1, *gpucube2, *gpucube3, *gpufloor;
	Plane *p0, *p1, *p0b, *p1b, *p2, *p2b, *floor, *groundMesh, *phamster, *phamsterb, *tstplane, *tstplanebil;
	//vector<Renderable*> objects;
	Material *tiles, *rikako, *yumemi, *mesh, *ground, *hamster, *hamsterBil, *tst, *tstbil, *wood1, *wood2, *wood1b, *wood2b;
	GLuint gputex;
	V3 gpumode;
	ShadowMapNS::ShadowMap *tstShadow, *tstEnv;
	Scene();
	bool vizcamena = false;
	void DBG();
	void Run();
	void Save();
	void Render();
	void RenderGPU();
	void drawGrid();
	void cameraControl();
	void cameraControlFPS();
	void cameraControlRevolve(V3 ctr);
	void clampMouse();
	void cinematicCamera(bool save);
	void animateScene();
	Renderable* getLastRenderable();
	void addRenderable(Renderable* renderable);
	void renderSceneObjects(PPC* ppc, FrameBuffer* fb);
	void renderSceneObjectsShadow(PPC* ppc, FrameBuffer* fb);
	void RayTrace();
};

extern Scene *scene;