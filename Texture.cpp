#include "Texture.h"

Texture::Texture(char* path) {
	loadTiff(path);
	filter = NEAREST;
}

Texture::Texture(int _w, int _h) {
	w = _w;
	h = _h;
	for (int i = 0; i < w*h; ++i) {
		pix[i] = 0x0;
	}
	filter = NEAREST;
}

bool Texture::isOpaque() {
	return (opacity == NULL);
}

float  Texture::clampCoordinate(float c) {
	c = c - (int)c;
	return c < 0 ? 1+c : c;
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
	//clamp to 0,1 preserving offset
	int u = ((int)(_u*(w-1)))%w;
	int v = ((int)((1-_v)*(h-1)))%h;
	if (u < 0) u += w;
	if (v < 0) v += h;
	int ru = (int)u;
	int rv = (int)v;
	if (v * w + u > w * h || v * w + u < 0)
		cerr << ru << " , " << rv << " : " << u << " , " << v << " from: " << _u << "  ," << _v << "\n";

	return pix[v * w + u];
}

unsigned int Texture::getColorBilinear(float _u, float _v) {
	//clamp to 0,1 preserving offset
	float u = _u - (int)_u;
	float v = _v - (int)_v;
	if (u < 0) u += 1;
	if (v < 0) v += 1;
	//if (u < 0 || u>1 || v < 0 || v>1) {
	//	cerr << u << " , " << v << "from: " << _u << "  ," << _v <<"\n";
	//}
	u *= (w - 1);
	v = (1 - v)*(h - 1);
	//if (u >= w - 1) u = w - 1;
	//if (v < h - 1) v = h - 1;
	int ru = (int)u;
	int rv = (int)v;
	//if (rv * w + ru > w * h || rv * w + ru < 0)
	//	cerr << ru << " , " << rv << " : " << u << " , " << v << " from: " << _u << "  ," << _v << "\n";

	return pix[rv * w + ru];
}

float Texture::getOpacityNearest(float _u, float _v) {
	//clamp to 0,1 preserving offset
	int u = ((int)(_u*(w - 1))) % w;
	int v = ((int)((1 - _v)*(h - 1))) % h;
	if (u < 0) u += w;
	if (v < 0) v += h;
	int ru = (int)u;
	int rv = (int)v;
	//if (v * w + u > w * h || v * w + u < 0)
	//	cerr << ru << " , " << rv << " : " << u << " , " << v << " from: " << _u << "  ," << _v << "\n";
	return opacity[v * w + u];
}

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
}

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