/**
 *	Title:	ppc.h
 *	Author: David Hrusa
 *	Date:	9/14/2018
 *	Desc:	A Perspective Projection Camera
 */

#pragma once
#include "v3.h"
#include "m33.h"
#include <fstream>

using namespace std;

//forward declaration
class FrameBuffer;

class PPC
{
public:
	V3 horizontal, vertical, topleft, pos;
	int w, h;
	//
	PPC(float hfov, int _w, int _h);
	PPC(V3 _hor, V3 _ver, V3 _tl, V3 _pos);
	//X, Y, Z in the camera coordinates
	void translate(V3 translation);
	//pan, tilt, roll
	void pan(float degrees);
	void tilt(float tilt);
	void roll(float tilt);
	void changeFocalLength(float units);
	//Projects p to pp.
	int project(V3 p, V3 &pp);
	//Interpolate towards the provided camera to the rate provided 0-1.
	void interpolate(PPC* target, PPC* set, float rate);
	void loadFromFile(char* path);
	void saveToFile(char* path);
	//prints out camera info:
	friend ostream& operator<<(ostream& os, const PPC *c);
	friend istream& operator>>(istream& is, PPC* ppc);
	//bonus
	float getFocalLength();
	void setTo(PPC ppc);
	void lookAt(V3 target);
	void visualize(PPC* viewer, FrameBuffer* canvas, FrameBuffer* view);
	V3 up();
	V3 forward();
	void postitionAndOrient(V3 C1, V3 L1, V3 vpv);
};

