#pragma once
/**
 *	Title:	Material.h
 *	Author: David Hrusa
 *	Date:	10/5/2018
 *	Desc:	Holds various Textures
 */
#include "Texture.h"

class Material {
public:
	Material(Texture* _diffuse);
	Material(char* _diffuse);
	Texture* diffuse;
	Texture* opacity;
	Texture* specular;
	Texture* bump;
	Texture* occlusion;
	float specularPower;
	//
	void setDiffuse(Texture* tex);
	void setOpacity(Texture* tex);
	void setSpecular(Texture* tex);
	void setBump(Texture* tex);
	void setOcclusion(Texture* tex);
	void setSpecularPower(float val);
	//
	Texture* getDiffuse();
	Texture* getOpacity();
	Texture* getSpecular();
	Texture* getBump();
	Texture* getOcclusion();
	float getSpecularPower();
};