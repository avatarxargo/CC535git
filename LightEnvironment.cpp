#include "LightEnvironment.h"

LightEnvironment::LightEnvironment(Light* ambient) {
	ambientLight = ambient;
}

void LightEnvironment::setCameraPos(V3 pos) {
	cameraPos = pos;
}

V3 LightEnvironment::getLightingAtVertex(Material* mat, V3 point, V3 uvw, V3 normal) {
	V3 color;
	V3 viewDirection = (cameraPos-point).norm();
	V3 lightDirection = lights[0]->position - point;
	float distance = lightDirection.len();
	//normalize more efficiently:
	lightDirection = lightDirection.norm();
	float specularPower = mat->getSpecularPower();
	V3 diff = V3(mat->getDiffuse()->getColor(uvw[0], uvw[1]));
	V3 specular = true?V3(0,0,0):V3(mat->getSpecular()->getColor(uvw[0], uvw[1]));
	//
	if (distance > lights[0]->rangeMax) {
		color =
			diff ^ ambientLight->color;
	}
	else if (distance < lights[0]->rangeStart) {
		float diffStrength = fabsf(lightDirection * normal);
		float specularStrength = fabsf(lightDirection * viewDirection);
		color =
			diff ^ ambientLight->color +
			diff ^ lights[0]->color * diffStrength +
			specular ^ lights[0]->color * specularStrength;
	}
	else {
		float diffStrength = fabsf(lightDirection * normal); 
		float specularStrength = fabsf(lightDirection * viewDirection);
		float lightStrength = (1 - ((distance - lights[0]->rangeStart) / (lights[0]->diff)));
		color =
			diff ^ ambientLight->color + 
			diff ^ lights[0]->color * lightStrength * diffStrength +
			specular ^ lights[0]->color * lightStrength * specularStrength;
	}
	return color.clamp();
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