#pragma once
/**
 *	Title:	texture.h
 *	Author: David Hrusa
 *	Date:	10/2/2018
 *	Desc:	Holds texture data, constructed from a .tiff path.
 */
#include "tiffio.h"
#include "v3.h"
#include <vector>

enum TexFilter { NEAREST, BILINEAR, TRILINEAR };

class Texture {
public:
	int w, h, mipMapCount;
	TexFilter filter;
	unsigned int* pix;
	float mipMapDepth;
	vector<unsigned int*> mipMap;
	Texture(int _w, int _h, float val);
	Texture(char * path);
	void setFilter(TexFilter flt);
	void loadTiff(char* path);
	//
	float clampCoordinate(float c, float off, int max);
	//void loadTiffTransparency(char* path);
	unsigned int getColor(V3 uvw);
	unsigned int getColorNearest(float u, float v);
	unsigned int getColorBilinear(float u, float v);
	unsigned int getColorBilinearTargeted(unsigned int* tgt, int ww, int hh, float _u, float _v);
	unsigned int getColorTrilinear(float u, float v, float depth);
	void genMipMap(int tiers, float maxdepth);
};