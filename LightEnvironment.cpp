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
	V3 diff = V3(mat->getDiffuse()->getColor(uvw));
	V3 specularCol = V3(1, 1, 1);// true ? V3(0, 0, 0) : V3(mat->getSpecular()->getColor(uvw[0], uvw[1]));
	//
	if (distance > lights[0]->rangeMax) {
		color =
			diff ^ ambientLight->color;
		//cerr << "diff: "<<diff << " , amvient " << ambientLight->color << " color: " << color << "\n";
	}
	else if (distance < lights[0]->rangeStart) {
		float diffStrength = fmaxf(lightDirection * normal, 0);
		V3 halfway = (lightDirection + viewDirection).norm();
		float specularStrength = fmaxf(powf(halfway * normal,mat->getSpecularPower()), 0);
		V3 diffLight = ambientLight->color + lights[0]->color * diffStrength;
		color =
			diff ^ diffLight.clamp() +
			specularCol * specularStrength;
	}
	else {
		float lightStrength = (1 - ((distance - lights[0]->rangeStart) / (lights[0]->diff)));
		//
		float diffStrength = fmaxf(lightDirection * normal, 0);
		V3 halfway = (lightDirection + viewDirection).norm();
		float specularStrength = lightStrength * fmaxf(powf(halfway * normal, mat->getSpecularPower()), 0);
		V3 diffLight = ambientLight->color + lights[0]->color * lightStrength * diffStrength;
		color =
			diff ^ diffLight.clamp() +
			specularCol * specularStrength;
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