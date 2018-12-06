#include "Texture.h"

Texture::Texture(char* path) {
	loadTiff(path);
	genV3();
	filter = NEAREST;
}

Texture::Texture(int _w, int _h, float val) {
	w = _w;
	h = _h;
	for (int i = 0; i < w*h; ++i) {
		pix[i] = (unsigned int)(0xFFFFFFFF*val) & 0xFFFFFFFF;
		pixv[i] = V3(pix[i]);
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

unsigned int Texture::getColor(V3 uvw) {
	switch (filter) {
	case NEAREST:
		return getColorNearest(uvw[0], uvw[1]);
	case BILINEAR:
		return getColorBilinear(uvw[0], uvw[1]);
	case TRILINEAR:
		return getColorTrilinear(uvw[0], uvw[1], uvw[2]);
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

unsigned int Texture::getColorBilinearTargeted(unsigned int* tgt, int ww, int hh, float _u, float _v) {
	//project into texture dimensions
	float u = (_u * ww) - 0.5f;
	float v = ((1 - _v) * hh) - 0.5f;
	u -= floor(u / ww)*ww;
	v -= floor(v / hh)*hh;
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
	l -= floor(l / ww)*ww;
	r -= floor(r / ww)*ww;
	b -= floor(b / hh)*hh;
	t -= floor(t / hh)*hh;
	//get colors
	V3 lt = V3(tgt[t*ww + l]);
	V3 rt = V3(tgt[t*ww + r]);
	V3 lb = V3(tgt[b*ww + l]);
	V3 rb = V3(tgt[b*ww + r]);
	//interpolate
	V3 top = (lt*dl) + (rt*dr);
	V3 bot = (lb*dl) + (rb*dr);
	V3 col = (top*dt) + (bot*db);
	//
	return col.getColor();
}

unsigned int Texture::getColorTrilinear(float _u, float _v, float _depth) {
	float pick = _depth / mipMapDepth;
	if (pick < 0) pick = 0;
	if (pick > 1) pick = 1;
	pick *= (mipMapDepth-1);
	int bot = floor(pick);
	int top = bot + 1;
	if (top > (mipMapDepth - 1)) top = (mipMapDepth - 1);
	float diffb = pick - bot;
	float difft = top - pick;
	V3 bcol = V3(getColorBilinearTargeted(mipMap[bot], mipMapSides[bot], mipMapSides[bot], _u, _v));
	V3 tcol = V3(getColorBilinearTargeted(mipMap[top], mipMapSides[top], mipMapSides[top], _u, _v));
	return (bcol * difft + tcol * diffb).getColor();
}

//V3


V3 Texture::getColorV3(V3 uvw) {
	switch (filter) {
	case NEAREST:
		return getColorNearestV3(uvw[0], uvw[1]);
	case BILINEAR:
		return getColorBilinearV3(uvw[0], uvw[1]);
	case TRILINEAR:
		return getColorTrilinearV3(uvw[0], uvw[1], uvw[2]);
	}
}

V3 Texture::getColorNearestV3(float _u, float _v) {
	//project into texture dimensions
	float u = (_u * w) - 0.0f;
	float v = ((1 - _v) * h) - 0.0f;
	u -= floor(u / w)*w;
	v -= floor(v / h)*h;
	//round down
	int iu = floor(u);
	int iv = floor(v);
	//
	return pixv[iv*w + iu];
}

V3 Texture::getColorBilinearV3(float _u, float _v) {
	//project into texture dimensions
	float u = (_u * w) - 0.5f;
	float v = ((1 - _v) * h) - 0.5f;
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
	V3 lt = pixv[t*w + l];
	V3 rt = pixv[t*w + r];
	V3 lb = pixv[b*w + l];
	V3 rb = pixv[b*w + r];
	//interpolate
	V3 top = (lt*dl) + (rt*dr);
	V3 bot = (lb*dl) + (rb*dr);
	V3 col = (top*dt) + (bot*db);
	//
	return col;
}

V3 Texture::getColorBilinearTargetedV3(V3* tgt, int ww, int hh, float _u, float _v) {
	//project into texture dimensions
	float u = (_u * ww) - 0.5f;
	float v = ((1 - _v) * hh) - 0.5f;
	u -= floor(u / ww)*ww;
	v -= floor(v / hh)*hh;
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
	l -= floor(l / ww)*ww;
	r -= floor(r / ww)*ww;
	b -= floor(b / hh)*hh;
	t -= floor(t / hh)*hh;
	//get colors
	V3 lt = tgt[t*ww + l];
	V3 rt = tgt[t*ww + r];
	V3 lb = tgt[b*ww + l];
	V3 rb = tgt[b*ww + r];
	//interpolate
	V3 top = (lt*dl) + (rt*dr);
	V3 bot = (lb*dl) + (rb*dr);
	V3 col = (top*dt) + (bot*db);
	//
	return col;
}

V3 Texture::getColorTrilinearV3(float _u, float _v, float _depth) {
	float pick = _depth / 1;// mipMapDepth;
	if(_depth>0)
	cout << _depth << endl;
	if (pick < 0) pick = 0;
	if (pick > 1) pick = 1;
	pick *= (mipMapDepth - 1);
	int bot = floor(pick);
	int top = bot + 1;
	if (top > (mipMapDepth - 1)) top = (mipMapDepth - 1);
	float diffb = pick - bot;
	float difft = top - pick;
	top = 1;
	bot = 1;
	V3 bcol = getColorBilinearTargetedV3(mipMapV3[bot], mipMapSides[bot], mipMapSides[bot], _u, _v);
	V3 tcol = getColorBilinearTargetedV3(mipMapV3[top], mipMapSides[top], mipMapSides[top], _u, _v);
	return (bcol * difft + tcol * diffb);
}

//currently only generates power of two square textures
void Texture::genMipMap(int tiers, float maxdepth) {
	mipMapDepth = maxdepth;
	filter = TRILINEAR;
	mipMapCount = tiers;
	mipMap.insert(mipMap.end(), pix);
	int lastside = w;
	int side = w;
	mipMapSides.insert(mipMapSides.end(), side);
	for (int i = 1; i < mipMapCount; ++i) {
		side = sqrt(side);
		mipMapSides.insert(mipMapSides.end(), side);
		unsigned int* tmppix = new unsigned int[side*side];
		//
		for (int y = 0; y < side; ++y) {
			for (int x = 0; x < side; ++x) {
				tmppix[y*side+x] = getColorBilinearTargeted(mipMap[i - 1], lastside, lastside, (1 + x * 2) / 8.0f, (1 + y * 2) / 8.0f);
			}
		}
		//
		mipMap.insert(mipMap.end(), tmppix);
		lastside = side;
	}
}

void Texture::genMipMapV3(int tiers, float maxdepth) {
	mipMapDepth = maxdepth;
	filter = TRILINEAR;
	mipMapCount = tiers;
	mipMapV3.insert(mipMapV3.end(), pixv);
	int lastside = w;
	int side = w;
	mipMapSides.insert(mipMapSides.end(), side);
	for (int i = 1; i < mipMapCount; ++i) {
		side = sqrt(side);
		mipMapSides.insert(mipMapSides.end(), side);
		V3* tmppix = new V3[side*side];
		//
		for (int y = 0; y < side; ++y) {
			for (int x = 0; x < side; ++x) {
				tmppix[y*side + x] = V3(i % 3 == 0, i % 3 == 1, i % 3 == 2);//getColorBilinearTargetedV3(mipMapV3[i - 1], lastside, lastside, (1 + x * 2) / 8.0f, (1 + y * 2) / 8.0f);
			}
		}
		//
		mipMapV3.insert(mipMapV3.end(), tmppix);
		lastside = side;
	}
}

void Texture::genV3() {
	for (int i = 0; i < w * h; ++i) {
		pixv[i] = V3(pix[i]);
	}
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
		pixv = new V3[w*h];
		//clean up tmp
	}

	if (TIFFReadRGBAImage(in, w, h, pix, 0) == 0) {
		cerr << "failed to load " << fname << endl;
	}

	TIFFClose(in);
}

GLuint Texture::load2GPU() {
	GLuint texid;
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pix);
	return texid;
}