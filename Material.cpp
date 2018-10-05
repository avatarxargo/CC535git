#include "Material.h"
/**
 *	Title:	Material.cpp
 *	Author: David Hrusa
 *	Date:	10/5/2018
 *	Desc:	Holds various Textures
 */

Material::Material(Texture* tex) {
	diffuse = tex;
}

Material::Material(char* tex) {
	diffuse = new Texture(tex);
}

void Material::setDiffuse(Texture* tex) {
	diffuse = tex;
}

void Material::setOpacity(Texture* tex) {
	opacity = tex;
}

void Material::setSpecular(Texture* tex) {
	specular = tex;
}

void Material::setBump(Texture* tex) {
	bump = tex;
}

void Material::setOcclusion(Texture* tex) {
	occlusion = tex;
}

Texture* Material::getDiffuse() {
	return diffuse;
}

Texture* Material::getOpacity() {
	return opacity;
}

Texture* Material::getSpecular() {
	return specular;
}

Texture* Material::getBump() {
	return bump;
}

Texture* Material::getOcclusion() {
	return occlusion;
}

void Material::setSpecularPower(float pow) {
	specularPower = pow;
}

float Material::getSpecularPower() {
	return specularPower;
}