#pragma once
/**
 *	Title:	texture.h
 *	Author: David Hrusa
 *	Date:	10/2/2018
 *	Desc:	Holds texture data, constructed from a .tiff path.
 */
#include "tiffio.h"
#include "v3.h"

enum TexFilter { NEAREST, BILINEAR, TRILINEAR };

class Texture {
public:
	int w, h;
	TexFilter filter;
	unsigned int* pix;
	Texture(int _w, int _h, float val);
	Texture(char * path);
	void setFilter(TexFilter flt);
	void loadTiff(char* path);
	//
	float clampCoordinate(float c, float off, int max);
	//void loadTiffTransparency(char* path);
	unsigned int getColor(float u, float v);
	unsigned int getColorNearest(float u, float v);
	unsigned int getColorBilinear(float u, float v);
	//float getOpacityNearest(float _u, float _v);
	//using a mipmap, generate first
	unsigned int getColorTrilinear(float u, float v, float depth);
	void genMipMap(int tiers);
};