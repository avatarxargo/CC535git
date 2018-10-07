#include "scene.h"
#include "v3.h"
#include "m33.h"
#include <stdlib.h>

Scene *scene;

using namespace std;

#include <fstream>

#include <iostream>

Scene::Scene() {

	gui = new GUI();
	gui->show();

	int u0 = 20;
	int v0 = 20;
	int w = 1280;
	int h = 720;

	camera = new PPC(60, w, h);
	camera->pos = camera->pos + V3(0, 50, 0);
	camera->loadFromFile("mydbg/cameraInfo.txt");
	cam1 = new PPC(60, w, h);
	cam2 = new PPC(60, w, h);
	cam3 = new PPC(60, w, h);
	vizcam = new PPC(60, w, h);

	tm1 = new TriangleMesh();
	tm2 = new TriangleMesh();
	tm3 = new TriangleMesh();
	tm4 = new TriangleMesh();
	tm5 = new TriangleMesh();
	tm1->LoadBin("geometry/teapot1K.bin");
	tm1->scale(50);
	tm1->translate(V3(0,0,-200));
	tm2->LoadBin("geometry/teapot1K.bin");
	tm1->translate(V3(-300, 0, -200));
	tm3->LoadBin("geometry/happy4.bin");
	tm3->translate(V3(0, 50, -400));
	tm3->scale(600);
	tm4->LoadBin("geometry/happy4.bin");
	tm5->LoadBin("geometry/terrain.bin");
	tm5->rotateAboutAxis(tm5->GetCenter(), V3(1, 0, 0), -90);
	//
	tm1->position(V3(0, 50, -200), 200);
	tm2->position(V3(0, 50, -350), 200);
	tm3->position(V3(-200, 50, -50), 350);
	tm4->position(V3(100, 50, -550), 250);
	tm5->position(V3(-300, -250, -0), 1500);

	rikako = new Material("tex/rikako.tif");
	rikako->setOpacity(new Texture("tex/rikako_opacity.tif"));
	yumemi = new Material("tex/yumemi.tif");
	mesh = new Material("tex/mesh.tif");
	ground = new Material("tex/ground.tif");
	hamster = new Material("tex/hamster.tif");
	hamsterBil = new Material("tex/hamster.tif");
	hamsterBil->setOpacity(new Texture("tex/hamstera.tif"));
	wood1 = new Material("tex/wood1.tif");
	wood2 = new Material("tex/wood2.tif");
	wood1b = new Material("tex/wood1.tif");
	wood2b = new Material("tex/wood2.tif");
	tst = new Material("tex/tst.tif");
	tstbil = new Material("tex/tst.tif");
	tstbil->getDiffuse()->setFilter(BILINEAR);
	mesh->getDiffuse()->setFilter(BILINEAR);
	hamsterBil->getDiffuse()->setFilter(BILINEAR);
	rikako->getDiffuse()->setFilter(BILINEAR);
	wood1b->getDiffuse()->setFilter(BILINEAR);
	wood2b->getDiffuse()->setFilter(BILINEAR);

	tiles = new Material("tex/tiles.tif");
	tiles->getDiffuse()->genMipMap(5,2000);
	p0 = new Plane(V3(0, 50, -400), V3(0, 100, 0), V3(-100, 0, 0), wood1);
	p1 = new Plane(V3(250, 50, -400), V3(0, 100, 0), V3(-100, 0, 0), mesh);
	p0b = new Plane(V3(0, 250, -400), V3(0, 100, 0), V3(-100, 0, 0), wood1b);
	p1b = new Plane(V3(250, 250, -400), V3(0, 100, 0), V3(-100, 0, 0), mesh);
	p2 = new Plane(V3(-250, 50, -400), V3(0, 100, 0), V3(-100, 0, 0), wood2);
	p2b = new Plane(V3(-250, 250, -400), V3(0, 100, 0), V3(-100, 0, 0), wood2b);
	floor = new Plane(V3(0, -50, 0), V3(150, 0, 0), V3(0, 0, -150), tstbil);
	phamster = new Plane(V3(-500, 250, -400), V3(0, 100, 0), V3(-100, 0, 0), hamster);
	phamsterb = new Plane(V3(-500, 50, -400), V3(0, 100, 0), V3(-100, 0, 0), hamsterBil);
	tstplane = new Plane(V3(-750, 250, -400), V3(0, 100, 0), V3(-100, 0, 0), tst);
	tstplanebil = new Plane(V3(-750, 50, -400), V3(0, 100, 0), V3(-100, 0, 0), tstbil);
	phamsterb->setUV(3, 3);
	tstplane->setUV(2, 2);
	tstplanebil->setUV(2, 2);
	groundMesh = new Plane(V3(0, -55, -1000), V3(500, 0, 0), V3(0, 0, -1000), tiles);
	groundMesh->setUV(20, 10);
	/*for (float f = -2.6f; f < 2.6f; f += 0.01f) {
		if(mesh->clampCoordinate(f)<0 || mesh->clampCoordinate(f)>1)
		cerr << mesh->clampCoordinate(f) << endl;
	}*/

	fb = new FrameBuffer(u0, v0, w, h);
	fb->label("SW Framebuffer");
	Light* ambientl = new Light(V3(0.1, 0.1, 0.2));
	Light* l1 = new Light(V3(0, 100, 0), V3(1, 1, 1), 400, 600);
	fb->addLight(l1);
	fb->lightEnvironment->setAmbient(ambientl);
	fb->show();

	if (vizcamena) {
		subfb = new FrameBuffer(u0, v0, w, h);
		subfb->label("SW Cam 0");
		subfb->show();
		subfb->refreshColor(0xFF000000);
	}
	cerr << "To use Camera Control:\n1. hit \"Pt 1 Demo\"\n2. use AWSDQE for translation\n3. use IJKLUO to pan, tilt and roll\nuse P; to change the focal length\n";

	gui->uiw->position(u0, v0 + fb->h + 60);

	fb->refreshColor(0xFF000000);
	Render();

	DBG();
}

V3 pos = V3(1, -1, -10);
float life = -1;

void updateLoop(Scene& scn, FrameBuffer* fb)
{
	while (true) {
		Fl::lock();
		{
			PPC* camera = scn.camera;
			//update camera
			camera->pos[0] = camera->pos[0] + fb->getYin();
			camera->pos[1] = camera->pos[1] + fb->getXin();
			scn.Render();
			cerr << "tick" << endl;
		}
		Fl::unlock();
		std::this_thread::sleep_for(200ms);
	}
}

void Scene::Render() {
	fb->refreshColor(0xFF000000);
	fb->refreshDepth(5000);
	fb->fog(0.5f, 1.5f, V3(0.8, 0.8, 1));
	//fb->lightEnvironment->lights[0]->position = camera->pos;

	//fb->drawRect(50, 100, 300, 150, 0xFF550066);
	//fb->drawCircle(950, 600, 150, 0xFF55FF88);
	//fb->drawTriangle(350, 400, 150, 350, 200, 450, 0xFFAA4488);
	//fb->drawTriangle(350, 400, 200, 450, 550, 550, 0xFF55BB88);
	//int interval = 3;
	//for (int a = 0; a < 10; ++a) {
	//	for (int d = 0; d < 10; ++d) {
	//		pos = V3(-15 + a * 3, -1, -1 - d);
	//		for (unsigned int t = 0; t < 360; ++t) {
	//			pos = pos.pointRotate(V3(-15 + a * 3, 0, -d), V3(0, 1, 0), 1); //  - d * interval
	//			//fb->Set(pos[0], pos[1], 0xFFFFFFFF);
	//			fb->draw3DPoint(pos, V3(1, 0, 0), camera, 3);
	//		}
	//	}
	//}
	//fb->drawCircle(pos[0], pos[1], 5, 0xFF9999AA);
	//fb->DrawSegment(V3(-200, 200, 0), V3(1, 0, 0), V3(300, 300, 0), V3(0, 1, 0));
	//tm1->renderWireframe(camera, fb);
	//tm2->renderFillTextured(camera, fb, rikako);
	//tm3->renderFillTextured(camera, fb, rikako);
	/*tm1->renderFillTextured(camera, fb, rikako->getDiffuse());
	tm4->renderFillTextured(camera, fb, rikako->getDiffuse());
*/
	tm1->renderFillTexturedLit(camera, fb, wood2);
	//tm4->renderFillTexturedLit(camera, fb, wood2);

	tm1->getBoundingBox().render(camera, fb);
	//tm4->getBoundingBox().render(camera, fb);
	//tm5->renderFillTextured(camera, fb, rikako);
	//p0b->drawScreenspace(camera, fb);
	//p1b->drawScreenspace(camera, fb);
	//cerr << "here\n";
	groundMesh->draw(camera, fb);
	floor->draw(camera, fb);
	p0->draw(camera, fb);
	p1->draw(camera, fb);
	p2->draw(camera, fb);
	p2b->draw(camera, fb);
	phamster->draw(camera, fb);
	phamsterb->draw(camera, fb);
	tstplane->draw(camera, fb);
	tstplanebil->draw(camera, fb);

	//grid
	drawGrid();

	if (vizcamena) {
		subfb->refreshColor(0xFF000000);
		subfb->refreshDepth(5000);
		tm1->renderFill(vizcam, subfb);
		tm4->renderFill(vizcam, subfb);
		tm1->getBoundingBox().render(vizcam, subfb);
		tm4->getBoundingBox().render(vizcam, subfb);
		p0->draw(vizcam, subfb);
		p1->drawuv(vizcam, subfb);
		vizcam->visualize(camera, fb, subfb);
		subfb->redraw();
	}
	   	 	
	fb->fog(0.5f, 1.5f, V3(0.8, 0.8, 1));
	fb->redraw();
	Fl::check();
}

void Scene::Save() {
	cinematicCamera(true);
	return;
	cerr << "Encoding video:" << endl;
	encodeFile(); //libx264rgb
	//videoCapture = Init(1280, 720, 30, 400000);
	for (int i = 0; i < 360; ++i) {
		Render();
		//do the image
		string filename = "mydbg/img" + std::to_string(i) + ".tif";
		fb->SaveAsTiff((char*)filename.c_str());
		unsigned int* data = fb->getDataPtr();
		//do video frame
		drawFrame(data, i);
		uint8_t* datauint = (uint8_t*)data;
		//videoCapture->xAddFrame(datauint);
		Fl::check();
	}
	//videoCapture->xFinish();
	finishEncodingFile();
}

void Scene::drawGrid() {
	float step = 50;
	int count = 20;
	float start = step * count / 2;
	V3 col = V3(0.5f, 0.5f, 0.5f);
	for (int i = 0; i < count; ++i) {
		for (int j = 0; j < count; ++j) {
			//fb
			fb->draw3DSegment(V3(start - j * step, 0, start - i * step), col, V3(start - (j + 1) * step, 0, start - i * step), col, camera);
			fb->draw3DSegment(V3(start - j * step, 0, start - i * step), col, V3(start - j * step, 0, start - (i + 1) * step), col, camera);
			if (vizcamena) {
				subfb->draw3DSegment(V3(start - j * step, 0, start - i * step), col, V3(start - (j + 1) * step, 0, start - i * step), col, vizcam);
				subfb->draw3DSegment(V3(start - j * step, 0, start - i * step), col, V3(start - j * step, 0, start - (i + 1) * step), col, vizcam);
			}
		}
	}
}

bool dir = true;
void Scene::animateScene() {
	tm1->rotateAboutAxis(tm1->GetCenter(), V3(0, 1, 0), 5);
	if (dir) {
		tm1->scale(1.01f);
		if (tm1->currentScale > 1.2)
			dir = false;
	}
	else {
		tm1->scale(0.99f);
		if (tm1->currentScale < 0.8)
			dir = true;
	}
	//
	tm2->rotateAboutAxis(tm2->GetCenter(), V3(1, 0, 0), 7);
	tm3->rotateAboutAxis(tm3->GetCenter(), V3(1, 1, 0), 11);
	tm4->rotateAboutAxis(tm4->GetCenter(), V3(0, 1, 0), 9);
	tm5->rotateAboutAxis(V3(0,0,0), V3(0, 1, 0), 9);
}

void Scene::cinematicCamera(bool save) {
	cam1->loadFromFile("mydbg/cam1.txt");
	cam2->loadFromFile("mydbg/cam2.txt");
	cam3->loadFromFile("mydbg/cam3.txt");
	if (save) {
		cerr << "Encoding video:" << endl;
		encodeFile();
	}
	float seconds = 10;
	float fps = 30;
	int iterations = seconds * fps;
	float half = iterations / 2.0f;
	for (int i = 0; i < iterations; ++i) {

		if(i >= half)
			cam2->interpolate(cam3, camera, (i-half) / half);
		else
			cam1->interpolate(cam2, camera, (i / half));

		animateScene();
		//cameraControlFPS();

		Render();
		Fl::check();
		if (save) {
			unsigned int* data = fb->getDataPtr();
			//do video frame
			drawFrame(data, i);
			uint8_t* datauint = (uint8_t*)data;
		}
	}
	if (save)
	finishEncodingFile();
}

void Scene::cameraControl() {
	float spd = 25;
	float tspd = 10;
	float zoomspd = 50;
	camera->translate(V3(fb->getXin()*spd, fb->getYin()*spd, fb->getZin()*spd));
	int tmp = fb->getJin()*tspd;
	if (tmp != 0) {
		camera->pan(-tmp);
	}
	tmp = fb->getIin()*tspd;
	if (tmp != 0) {
		camera->tilt(tmp);
	}
	tmp = fb->getUin()*tspd;
	if (tmp != 0) {
		camera->roll(-tmp);
	}
	tmp = fb->getPin()*zoomspd;
	if (tmp != 0) {
		camera->changeFocalLength(tmp);
	}
	//
	if (fb->getPrintCam()) {
		camera->saveToFile("mydbg/cameraInfo.txt");
	}
	if (fb->getLoadCam()) {
		camera->loadFromFile("mydbg/cameraInfo.txt");
	}
}

float ypos = 0;
float yspd = 0;
float jumpspd = 80;
float gravity = -10;
bool grounded = true;
void Scene::cameraControlFPS() {
	float spd = 25;
	float tspd = 1;
	float zoomspd = 50;
	camera->translateFlat(V3(fb->getInput(1)*spd, fb->getInput(0)*spd, 0));
	if (grounded && fb->getZin()*spd > 0) {
		grounded = false;
		yspd = jumpspd;
		ypos = 0;
	}
	yspd += gravity;
	ypos += yspd;
	if (ypos < 0) {
		grounded = true;
		ypos = 0;
		yspd = 0;
	}
	camera->pos[1] = ypos+50;

	float tmp = fb->getInput(11)*tspd;
	if (tmp != 0) {
		camera->panFlat(-tmp,V3(0,1,0));
	}
	tmp = fb->getInput(10)*tspd;
	if (tmp != 0) {
		camera->tilt(tmp);
	}
	tmp = fb->getUin()*tspd;
	if (tmp != 0) {
		camera->roll(-tmp);
	}
	tmp = fb->getPin()*zoomspd;
	if (tmp != 0) {
		camera->changeFocalLength(tmp);
	}
	//
	if (fb->getPrintCam()) {
		camera->saveToFile("mydbg/cameraInfo.txt");
	}
	if (fb->getLoadCam()) {
		camera->loadFromFile("mydbg/cameraInfo.txt");
	}
	fb->input(10, 0);
	fb->input(11, 0);
}

void Scene::DBG() {
	//Scene &me = *this;
	//std::thread updloop(updateLoop, me, fb);
	//updloop.detach();

	/*V3 vtorotate(1, 1, 0);
	Quaternion q1(0.5f, 1, 0, 0);
	cerr << vtorotate << " rotates to " << q1.rotateVectorAboutAngleAndAxis(vtorotate) << endl;*/

	//cinematicCamera(true);

	while(true) {

		//move camera
		cameraControlFPS();
		//animate scene
		animateScene();

		Render();
		Fl::check();
	}

	return;

	//Vector showcase:
	V3 v0 = V3(1, 2, 3);
	V3 v1 = V3(1, 1, 1);
	V3 v2 = V3(3, 5, 2);
	V3 v3 = V3(5, 7, 2);
	V3 r = V3(1, 0, 0);
	V3 g = V3(0, 1, 0);
	V3 b = V3(0, 0, 1);
	cerr << "Showcase of Vector properties:\n";
	cerr << "v0 = (" << v0 << ")\n";
	cerr << "v1 = (" << v1 << ")\n";
	cerr << "v2 = (" << v2 << ")\n";
	cerr << "v3 = (" << v3 << ")\n";
	cerr << "v0-v3 = (" << v0 - v3 << ")\n";
	cerr << "v2 * v3 = (" << v0 * v3 << ")\n";
	cerr << "v2 x v3 = (" << v0 % v3 << ")\n";
	cerr << "norm(v3) = " << v3.norm() << ")\n";
	cerr << "len(v3) = " << v3.len() << ")\n";

	//Matrix showcase:
	M33 m1 = M33(V3(1, 2, 3),
		V3(4, 5, 6),
		V3(7, 8, 9));
	M33 m0 = M33(V3(1, 0, 0),
		V3(0, 1, 0),
		V3(0, 0, 1));
	M33 m2 = M33(V3(3, 4, 5),
		V3(6, 8, 1),
		V3(9, 2, 3));
	M33 m3 = M33(V3(75, 25, 0),
		V3(0, 50, 50),
		V3(60, 40, 0));
	cerr << "Showcase of Matrix properties:\n";
	cerr << "m0\n" << m0 << endl;
	cerr << "m1\n" << m1 << endl;
	cerr << "m2\n" << m2 << endl;
	cerr << "m3\n" << m3 << endl;
	cerr << "m2*v2\n" << m2 * v2 << endl;
	cerr << "m2*m0\n" << m2 * m1 << endl;
	cerr << "m1 getColumn(2)\n" << m1.getColumn(2) << endl;
	cerr << "m1 getColumn(2)\n" << m1.getColumn(2) << endl;
	cerr << "m1 transpose\n" << m1.transpose() << endl;
	cerr << "m1 set column 1 to (2,2,2)\n" << m1.setColumn(1, V3(2, 2, 2)) << endl;
	M33* m3inv = new M33();
	m3.inverse(m3inv);
	cerr << "inverse of m3:\n" << m3inv << endl;

	V3 origin = V3(100, 50, 0);
	V3 dir = V3(0, 0.5f, 1);
	M33 sys;
	sys.setAsCoordinateSystem(dir);
	cerr << "Generating coord system:\nFrom origin:" << origin << "\ndirection: " << dir << "\nresult:\n" << sys << endl;

	//testing saving to and reading from file for << >> overloads.
	/*ofstream myfile;
	myfile.open("mat.txt");
	myfile << m0 << endl;
	myfile.close();

	ifstream myfile1;
	myfile1.open("mat.txt");
	myfile1 >> m0;
	myfile1.close();
	cerr << "mat from file:\n" << m0 << endl;*/

	cerr << "red: " << r.getColor() << " , " << r[0] << "," << r[1] << "," << r[2] << endl;
	cerr << "green: " << g.getColor() << " , " << g[0] << "," << g[1] << "," << g[2] << endl;
	cerr << "blue: " << b.getColor() << " , " << b[0] << "," << b[1] << "," << b[2] << endl;

	Render();
	fb->redraw();
}