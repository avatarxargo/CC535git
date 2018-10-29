#include "LightEnvironment.h"

LightEnvironment::LightEnvironment(Light* ambient) {
	ambientLight = ambient;
}

void LightEnvironment::setCameraPos(V3 pos) {
	cameraPos = pos;
}

V3 color, viewDirection, lightDirection, diff, specularCol, halfway, diffLight;
float distanced, specularPower, diffStrength, specularStrength, lightStrength;

V3 LightEnvironment::getLightingAtVertex(Material* mat, V3 point, V3 uvw, V3 normal) {
	//V3 color;
	viewDirection = (cameraPos-point).norm();
	lightDirection = lights[0]->position - point;
	distanced = lightDirection.len();
	//normalize more efficiently:
	lightDirection = lightDirection.norm();
	specularPower = mat->getSpecularPower();
	diff = mat->getDiffuse()->getColorV3(uvw);
	specularCol = V3(5, 1, 1);// true ? V3(0, 0, 0) : V3(mat->getSpecular()->getColor(uvw[0], uvw[1]));
	//
	if (distanced > lights[0]->rangeMax) {
		color =
			diff ^ ambientLight->color;
		//cerr << "diff: "<<diff << " , amvient " << ambientLight->color << " color: " << color << "\n";
	}
	else if (distanced < lights[0]->rangeStart) {
		diffStrength = fmaxf(lightDirection * normal, 0);
		halfway = (lightDirection + viewDirection).norm();
		specularStrength = fmaxf(powf(halfway * normal,40/*mat->getSpecularPower()*/), 0);
		diffLight = ambientLight->color + lights[0]->color * diffStrength;
		color =
			diff ^ diffLight.clamp() +
			specularCol * specularStrength;
	}
	else {
		lightStrength = (1 - ((distanced - lights[0]->rangeStart) / (lights[0]->diff)));
		//
		diffStrength = fmaxf(lightDirection * normal, 0);
		halfway = (lightDirection + viewDirection).norm();
		specularStrength = lightStrength * fmaxf(powf(halfway * normal, 40/*mat->getSpecularPower()*/), 0);
		diffLight = ambientLight->color + lights[0]->color * lightStrength * diffStrength;
		color =
			diff ^ diffLight.clamp() +
			specularCol * specularStrength;
	}
	return color.clamp();
}


V3 LightEnvironment::getLightingAtVertexShadow(Material* mat, V3 point, V3 uvw, V3 normal) {
	//V3 color;
	viewDirection = (cameraPos - point).norm();
	lightDirection = lights[0]->position - point;
	distanced = lightDirection.len();
	//normalize more efficiently:
	lightDirection = lightDirection.norm();
	specularPower = mat->getSpecularPower();
	diff = mat->getDiffuse()->getColorV3(uvw);
	specularCol = V3(5, 1, 1);// true ? V3(0, 0, 0) : V3(mat->getSpecular()->getColor(uvw[0], uvw[1]));
	//shadow
	float shadowPower = shadowMap->getMapValue(point);
	//
	if (distanced > lights[0]->rangeMax) {
		color =
			diff ^ ambientLight->color;
		//cerr << "diff: "<<diff << " , amvient " << ambientLight->color << " color: " << color << "\n";
	}
	else if (distanced < lights[0]->rangeStart) {
		diffStrength = fmaxf(lightDirection * normal, 0) * shadowPower;
		halfway = (lightDirection + viewDirection).norm();
		specularStrength = fmaxf(powf(halfway * normal, 40/*mat->getSpecularPower()*/), 0)* shadowPower;
		diffLight = ambientLight->color + lights[0]->color * diffStrength;
		color =
			diff ^ diffLight.clamp() +
			specularCol * specularStrength;
	}
	else {
		lightStrength = (1 - ((distanced - lights[0]->rangeStart) / (lights[0]->diff)));
		//
		diffStrength = fmaxf(lightDirection * normal, 0)* shadowPower;
		halfway = (lightDirection + viewDirection).norm();
		specularStrength = lightStrength * fmaxf(powf(halfway * normal, 40/*mat->getSpecularPower()*/), 0)* shadowPower;
		diffLight = ambientLight->color + lights[0]->color * lightStrength * diffStrength;
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