#pragma once
/**
 *	Title:	LightEnvironment.h
 *	Author: David Hrusa
 *	Date:	10/5/2018
 *	Desc:	Consolidates all lights in the scene + ambient lights and does computation of lighting
 */
#include "v3.h"
#include "light.h"
#include "material.h"
#include <vector>

class LightEnvironment {
public:
	V3 cameraPos;
	Light* ambientLight;
	vector<Light*> lights;
	//
	LightEnvironment(Light* ambient);
	void setAmbient(Light* l);
	void addLight(Light* l);
	void removeLight(int idx);
	void setCameraPos(V3 pos);
	//main function for rendering 
	V3 getLightingAtVertex(Material* mat, V3 point, V3 uvw, V3 normal);
};
