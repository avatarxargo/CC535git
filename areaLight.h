#pragma once

#include "v3.h"
#include "tm.h"

class AreaLight {
public:
	V3 pos;
	float size;
	TriangleMesh* tm;
	AreaLight(V3 _pos, float _size) {
		pos = _pos;
		size = _size;
		tm = new TriangleMesh((int)_size, (int)_size, -1);
		tm->translate(pos);
	}
	void update(V3 upos, float uscl) {
		tm->translate(upos);
		tm->scale(uscl);
	}
	V3 getA() {
		return pos + V3(size / 2, 0, size / 2);
	}
	V3 getB() {
		return pos + V3(size / 2, 0, -size / 2);
	}
	V3 getC() {
		return pos + V3(-size / 2, 0, -size / 2);
	}
	V3 getD() {
		return pos + V3(-size / 2, 0, size / 2);
	}
};