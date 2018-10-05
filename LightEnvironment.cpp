#include "LightEnvironment.h"

LightEnvironment::LightEnvironment(Light* ambient) {
	ambientLight = ambient;
}

V3 LightEnvironment::getLightingAtVertex(Material* mat, V3 point, V3 uvw, V3 normal) {
	V3 color;
	float distance = (lights[0]->position - point).len();
	V3 diff = V3(mat->getDiffuse()->getColor(uvw[0], uvw[1]));
	if (distance > lights[0]->rangeMax) {
		color = V3(0, 0, 0);
	}
	else if (distance < lights[0]->rangeStart) {
		color = diff ^ lights[0]->color;
	}
	else {
		color = diff ^ lights[0]->color * (1 - ((distance - lights[0]->rangeStart) / (lights[0]->rangeMax - lights[0]->rangeStart)));
	}
	return diff ^ color;
}

void LightEnvironment::setAmbient(Light* l) {
	ambientLight = l;
}

void LightEnvironment::addLight(Light* l) {
	lights.insert(lights.end(), l);
}

void LightEnvironment::removeLight(int idx) {
	lights.erase(lights.begin() + idx);
}