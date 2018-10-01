/**
 *	Title:	tm.cpp
 *	Author: David Hrusa
 *	Date:	9/14/2018
 *	Desc:	Represents a triangular mesh
 */

#include "tm.h"
#include <iostream>
#include <fstream>

using namespace std;

void TriangleMesh::allocate() {
	verts = new V3[vertsN];
	colors = new V3[vertsN];
	normals = new V3[vertsN];
	tris = new unsigned int[3 * trisN];
}

void TriangleMesh::rotateAboutAxis(V3 origin, V3 angle, float angled) {
	for (int vi = 0; vi < vertsN; vi++) {
		verts[vi] = verts[vi].pointRotate(origin, angle, angled);
	}
}

void TriangleMesh::translate(V3 translation) {
	for (int vi = 0; vi < vertsN; vi++) {
		verts[vi] = verts[vi] + translation;
	}
}


void TriangleMesh::scale(float _scale) {
	currentScale *= _scale;
	V3 center = GetCenter();
	for (int vi = 0; vi < vertsN; vi++) {
		verts[vi] = (verts[vi]-center) * _scale + center;
	}
}

void TriangleMesh::scale(float _scale, V3 center) {
	currentScale *= _scale;
	for (int vi = 0; vi < vertsN; vi++) {
		verts[vi] = verts[vi] + (verts[vi]-center).norm() * _scale;
	}
}


void TriangleMesh::renderPoints(PPC *ppc, FrameBuffer *fb) {

	for (int vi = 0; vi < vertsN; vi++) {
		fb->draw3DPoint(verts[vi], colors[vi], ppc, 7);
	}

}

void TriangleMesh::renderWireframe(PPC *ppc, FrameBuffer *fb) {

	for (int tri = 0; tri < trisN; tri++) {
		for (int ei = 0; ei < 3; ei++) {
			int vi0 = tris[3 * tri + ei];
			int vi1 = tris[3 * tri + ((ei + 1) % 3)];
			fb->draw3DSegment(verts[vi0], colors[vi0],
				verts[vi1], colors[vi1], ppc);
		}
	}

}

void TriangleMesh::renderFill(PPC *ppc, FrameBuffer *fb) {
	for (int tri = 0; tri < trisN; tri++) {
		int vi0 = tris[3 * tri];
		int vi1 = tris[3 * tri + 1];
		int vi2 = tris[3 * tri + 2];
		fb->draw3DTriangle(verts[vi0], colors[vi0], verts[vi1], colors[vi1], verts[vi2], colors[vi2], ppc);
	}

}

/*void TriangleMesh::renderFillClip(PPC *ppc, FrameBuffer *fb) {
	V3 pverts[];
	for (int tri = 0; tri < trisN; tri++) {
	unsigned int viz[3] = { tris[3 * tri], tris[3 * tri + 1] , tris[3 * tri + 2] };
	ppc->project(verts[tri], pverts);
	AABB aabb(pverts[viz[0]]);
	aabb.AddPoint(pverts[viz[1]]);
	aabb.AddPoint(pverts[viz[2]]);
	if (!aabb.clip2D(0, (float)fb->w, 0, (float)fb->h))
		continue;
	int lefti = (int)(aabb.c0[0] + 0.5f);
	int right = (int)(aabb.c1[0] + 0.5f);
	int topi = (int)(aabb.c0[1] + 0.5f);
	int boti = (int)(aabb.c1[1] + 0.5f);

	//set up edge equations
	M33 eeqs;
	for (int ei = 0; ei < 3; ei++) {
		V3 v0 = pverts[viz[ei]];
		V3 v1 = pverts[viz[(ei + 1) % 3]];
		V3 v2 = pverts[viz[(ei + 2) % 3]]; v2[2] = 1.0f;
		eeqs[ei][0] = v0[1] - v1[1];
		eeqs[ei][1] = v1[0] - v0[0];
		eeqs[ei][2] = -v0[0] * eeqs[ei][0] - v0[1] * eeqs[ei][1];
		if (eeqs[ei] * v2 < 0.0f) {
			eeqs[ei] = eeqs[ei] * -1.0f;
		}
	}

	unsigned int tc = colors[viz[0]].getColor();
	float tz = pverts[viz[0]][2];
}*/

AABB TriangleMesh::getBoundingBox() {
	if (!verts)
		return AABB(V3(0.0f, 0.0f, 0.0f));

	AABB aabb(verts[0]);
	for (int i = 1; i < vertsN; i++)
		aabb.AddPoint(verts[i]);

	return aabb;

}

V3 TriangleMesh::GetCenter() {

	V3 tmc(0.0f, 0.0f, 0.0f);
	for (int vi = 0; vi < vertsN; vi++) {

		tmc = tmc + verts[vi];

	}

	tmc = tmc * (1.0f / (float)vertsN);

	return tmc;

}

void TriangleMesh::position(V3 position, float _scale) {
	float ratio = _scale/getBoundingBox().GetDiagonal();
	V3 c = GetCenter();
	translate(position - c);
	scale(ratio);
}

void TriangleMesh::LoadBin(char *fname) {

	ifstream ifs(fname, ios::binary);
	if (ifs.fail()) {
		cerr << "INFO: cannot open file: " << fname << endl;
		return;
	}

	ifs.read((char*)&vertsN, sizeof(int));
	char yn;
	ifs.read(&yn, 1); // always xyz
	if (yn != 'y') {
		cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
		return;
	}
	if (verts)
		delete verts;
	verts = new V3[vertsN];

	ifs.read(&yn, 1); // cols 3 floats
	if (colors)
		delete colors;
	colors = 0;
	if (yn == 'y') {
		colors = new V3[vertsN];
	}

	ifs.read(&yn, 1); // normals 3 floats
	if (normals)
		delete normals;
	normals = 0;
	if (yn == 'y') {
		normals = new V3[vertsN];
	}

	ifs.read(&yn, 1); // texture coordinates 2 floats
	float *tcs = 0; // don't have texture coordinates for now
	if (tcs)
		delete tcs;
	tcs = 0;
	if (yn == 'y') {
		tcs = new float[vertsN * 2];
	}

	ifs.read((char*)verts, vertsN * 3 * sizeof(float)); // load verts

	if (colors) {
		ifs.read((char*)colors, vertsN * 3 * sizeof(float)); // load cols
	}

	if (normals)
		ifs.read((char*)normals, vertsN * 3 * sizeof(float)); // load normals

	if (tcs)
		ifs.read((char*)tcs, vertsN * 2 * sizeof(float)); // load texture coordinates

	ifs.read((char*)&trisN, sizeof(int));
	if (tris)
		delete tris;
	tris = new unsigned int[trisN * 3];
	ifs.read((char*)tris, trisN * 3 * sizeof(unsigned int)); // read tiangles

	ifs.close();

	cerr << "INFO: loaded " << vertsN << " verts, " << trisN << " tris from " << endl << "      " << fname << endl;
	cerr << "      xyz " << ((colors) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;

	delete[]tcs;

}