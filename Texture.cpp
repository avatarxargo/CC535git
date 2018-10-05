#include "Texture.h"

Texture::Texture(char* path) {
	loadTiff(path);
	filter = NEAREST;
}

Texture::Texture(int _w, int _h, float val) {
	w = _w;
	h = _h;
	for (int i = 0; i < w*h; ++i) {
		pix[i] = (unsigned int)(0xFFFFFFFF*val) & 0xFFFFFFFF;
	}
	filter = NEAREST;
}

float Texture::clampCoordinate(float c, float off, int max) {
	int u = ((int)(c*(max - 1) + off)) % max;
	if (u < 0) u += max;
	return u;
}

void  Texture::setFilter(TexFilter flt) {
	filter = flt;
}

unsigned int Texture::getColor(float _u, float _v) {
	switch (filter) {
	case NEAREST:
		return getColorNearest(_u, _v);
	case BILINEAR:
		return getColorBilinear(_u, _v);
	}
}

unsigned int Texture::getColorNearest(float _u, float _v) {
	//project into texture dimensions
	float u = (_u * w) - 0.0f;
	float v = ((1 - _v) * h) - 0.0f;
	u -= floor(u / w)*w;
	v -= floor(v / h)*h;
	//round down
	int iu = floor(u);
	int iv = floor(v);
	//
	return V3(pix[iv*w + iu]).getColor();
}

float decimalModulo(float a, int base) {
	a -= floor(a / base)*base;
	return a;
}

unsigned int Texture::getColorBilinear(float _u, float _v) {
	//project into texture dimensions
	float u = (_u * w)-0.5f;
	float v = ((1-_v) * h) - 0.5f;
	u -= floor(u / w)*w;
	v -= floor(v / h)*h;
	//round down
	int l = floor(u);
	int b = floor(v);
	int r = l + 1;
	int t = b + 1;
	//compute differences
	float dl = 1 - (u - l);
	float dr = 1 - (r - u);
	float db = 1 - (v - b);
	float dt = 1 - (t - v);
	//check exceeding
	l -= floor(l / w)*w;
	r -= floor(r / w)*w;
	b -= floor(b / h)*h;
	t -= floor(t / h)*h;
	//get colors
	V3 lt = V3(pix[t*w + l]);
	V3 rt = V3(pix[t*w + r]);
	V3 lb = V3(pix[b*w + l]);
	V3 rb = V3(pix[b*w + r]);
	//interpolate
	V3 top = (lt*dl) + (rt*dr);
	V3 bot = (lb*dl) + (rb*dr);
	V3 col = (top*dt) + (bot*db);
	//
	return col.getColor();
}

/*float Texture::getOpacityNearest(float _u, float _v) {
	//clamp to 0,1 preserving offset
	int u = ((int)(_u*(w)-0.5f)) % w;
	int v = ((int)((1 - _v)*(h)-0.5f)) % h;
	if (u < 0) u += w;
	if (v < 0) v += h;
	int ru = (int)u;
	int rv = (int)v;
	//if (v * w + u > w * h || v * w + u < 0)
	//	cerr << ru << " , " << rv << " : " << u << " , " << v << " from: " << _u << "  ," << _v << "\n";
	return opacity[v * w + u];
}*/

///*int left = clampCoordinate(_u, -0.5f, w);
//	int right = clampCoordinate(_u, 0.5f, w);
//	int bot = clampCoordinate(_v, -0.5f, h);
//	int top = clampCoordinate(_v, 0.5f, h);*/
//
//	/*int u = ((int)(_u*(w - 1))) % w;
//	int v = ((int)((1 - _v)*(h - 1))) % h;
//	if (u < 0) u += w;
//	if (v < 0) v += h;*/
//
//float poju = _u * (w - 1);// decimalModulo(_u*(w - 1), w);                                                                                                                                                                                                                         (_u*(w - 1), w);
//float pojv = _v * (h - 1);// decimalModulo(_v*(h - 1), h);
//
//int left = (int)floor(poju - 0.5f);
//int right = left + 1;//(int)(poju + 0.5f);
//int bot = (int)floor(pojv - 0.5f);
//int top = bot + 1;// (int)(pojv + 0.5f);
//
//if (poju >= w)
//cerr << poju << " from: " << w << "\n";
//
//float lweight = 1 - (poju - (float)left);
//float rweight = 1 - lweight;//1 - ((float)right - poju);
//float bweight = 1 - (pojv - (float)bot);
//float tweight = 1 - bweight;//1 - ((float)top - pojv);
///*float lweight = 0.4f;
//float rweight = 0.6f;
//float bweight = 0.4f;
//float tweight = 0.6f;*/
//
////cerr << (right - left) << " - " << (top - bot) << "\n";
////cerr << (lweight+ rweight) << " - " << (bweight + tweight) << "\n";
//
//if (left < 0) left = w - 1;
//if (bot < 0) bot = w - 1;
//if (right >= w) right = 0;
//if (top >= h) top = 0;
//
//
//V3 a = V3(pix[bot * w + left]);
//V3 b = V3(pix[bot * w + right]);
//V3 c = V3(pix[top * w + left]);
//V3 d = V3(pix[top * w + right]);
//
//V3 intertop = a * lweight + b * rweight;
//V3 interbot = c * lweight + d * rweight;
//
//return (intertop*tweight + interbot * bweight).getColor();

/*
void Texture::loadTiffTransparency(char* fname) {
	TIFF* in = TIFFOpen(fname, "r");
	if (in == NULL) {
		cerr << fname << " could not be opened" << endl;
		return;
	}

	int width, height;
	TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
	if (opacity == NULL) {
		delete[] opacity;
		opacity = new float[w*h];
		//clean up tmp
	}
	unsigned int* tmp = new unsigned int[w*h];

	if (TIFFReadRGBAImage(in, w, h, tmp, 0) == 0) {
		cerr << "failed to load " << fname << endl;
	}

	for (int i = 0; i < w*h; ++i) {
		V3 tmppixel(tmp[i]);
		opacity[i] = (tmppixel[0] + tmppixel[1] + tmppixel[2]) / 3;
	}

	TIFFClose(in);
	delete[] tmp;
}*/

// load a tiff image in grayscale to determine opacity.
void Texture::loadTiff(char* fname) {
	TIFF* in = TIFFOpen(fname, "r");
	if (in == NULL) {
		cerr << fname << " could not be opened" << endl;
		return;
	}

	int width, height;
	TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
	if (w != width || h != height) {
		w = width;
		h = height;
		delete[] pix;
		pix = new unsigned int[w*h];
		//clean up tmp
	}

	if (TIFFReadRGBAImage(in, w, h, pix, 0) == 0) {
		cerr << "failed to load " << fname << endl;
	}

	TIFFClose(in);
}