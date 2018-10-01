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
	camera->pos = camera->pos + V3(0, 0, 0);
	cam1 = new PPC(60, w, h);
	cam2 = new PPC(60, w, h);
	vizcam = new PPC(60, w, h);

	tm1 = new TriangleMesh();
	tm2 = new TriangleMesh();
	tm3 = new TriangleMesh();
	tm4 = new TriangleMesh();
	tm5 = new TriangleMesh();
	tm1->LoadBin("geometry/teapot1K.bin");
	tm1->translate(V3(0,0,-200));
	tm2->LoadBin("geometry/teapot1K.bin");
	tm1->translate(V3(-300, 0, -200));
	tm3->LoadBin("geometry/bunny.bin");
	tm3->translate(V3(0, 50, -400));
	tm3->scale(600);
	tm4->LoadBin("geometry/happy4.bin");
	tm5->LoadBin("geometry/terrain.bin");
	tm5->rotateAboutAxis(tm5->GetCenter(), V3(1, 0, 0), -90);
	//
	tm1->position(V3(0, 50, -200), 200);
	tm2->position(V3(0, 50, -350), 200);
	tm3->position(V3(-200, 50, -50), 350);
	tm4->position(V3(100, 50, -250), 250);
	tm5->position(V3(-300, -250, -0), 1500);

	fb = new FrameBuffer(u0, v0, w, h);
	fb->label("SW Framebuffer");
	fb->show();

	cerr << "To use Camera Control:\n1. hit \"Pt 1 Demo\"\n2. use AWSDQE for translation\n3. use IJKLUO to pan, tilt and roll\nuse P; to change the focal length\n";

	gui->uiw->position(u0, v0 + fb->h + 60);

	fb->refreshColor(0xFF000000);
	Render();

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
	tm1->renderFill(camera, fb);
	//tm2->renderWireframe(camera, fb);
	//tm3->renderWireframe(camera, fb);
	tm4->renderFill(camera, fb);
	tm1->getBoundingBox().render(camera, fb);
	tm4->getBoundingBox().render(camera, fb);
	//tm5->renderFill(camera, fb);

	vizcam->visualize(camera, fb);

	//grid
	drawGrid();

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
	cam1->loadFromFile("mydbg/cam2.txt");
	cam2->loadFromFile("mydbg/cam1.txt");
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
			cam1->interpolate(cam2, vizcam, (i-half) / half);
		else
			cam1->interpolate(cam2, vizcam, 0);

		animateScene();
		cameraControl();

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
	float spd = 5;
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

void Scene::DBG() {
	//Scene &me = *this;
	//std::thread updloop(updateLoop, me, fb);
	//updloop.detach();

	/*V3 vtorotate(1, 1, 0);
	Quaternion q1(0.5f, 1, 0, 0);
	cerr << vtorotate << " rotates to " << q1.rotateVectorAboutAngleAndAxis(vtorotate) << endl;*/

	cinematicCamera(false);

	while(true) {

		//move camera
		cameraControl();
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
	cerr << "inverse of m3:\n" << m3.inverse() << endl;

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