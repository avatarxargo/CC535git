#include "shadowmap.h"
#include "plane.h"
#include "framebuffer.h"
#include "tm.h"
#include "v3.h"
/**
 *	Title:	shadowmap.h
 *	Author: David Hrusa
 *	Date:	10/19/2018
 *	Desc:	Looks in 6 directions and casts shadows
 */

#define MAXDEPTH 10000;

namespace ShadowMapNS {
	ShadowMap::ShadowMap(V3 _pos, int resolution) {
		mapRes = resolution;
		pos = V3(_pos);
		/* top is +y
		 * north is +z
		 * east is +x */
		 //create the directional buffers:
		dirBuffer[TOP] = new FrameBuffer(0, 0, mapRes, mapRes);
		dirBuffer[BOT] = new FrameBuffer(0, 0, mapRes, mapRes);
		dirBuffer[NORTH] = new FrameBuffer(0, 0, mapRes, mapRes);
		dirBuffer[SOUTH] = new FrameBuffer(0, 0, mapRes, mapRes);
		dirBuffer[WEST] = new FrameBuffer(0, 0, mapRes, mapRes);
		dirBuffer[EAST] = new FrameBuffer(0, 0, mapRes, mapRes);
		//create the directional cameras:
		dirCameras[TOP] = new PPC(V3(0, 0, 1), V3(1, 0, 0), V3(-1, 1, -1), pos);
		dirCameras[BOT] = new PPC(V3(0, 0, 1), V3(-1, 0, 0), V3(1, -1, -1), pos);
		dirCameras[NORTH] = new PPC(V3(0, 0, 1), V3(0, -1, 0), V3(1, 1, -1), pos);
		dirCameras[EAST] = new PPC(V3(-1, 0, 0), V3(0, -1, 0), V3(1, 1, 1), pos);
		dirCameras[SOUTH] = new PPC(V3(0, 0, -1), V3(0, -1, 0), V3(-1, 1, 1), pos);
		dirCameras[WEST] = new PPC(V3(1, 0, 0), V3(0, -1, 0), V3(-1, 1, -1), pos);
		float factor = resolution / 2;
		dirCameras[TOP]->topleft = dirCameras[TOP]->topleft * factor;
		dirCameras[BOT]->topleft = dirCameras[BOT]->topleft * factor;
		dirCameras[NORTH]->topleft = dirCameras[NORTH]->topleft * factor;
		dirCameras[EAST]->topleft = dirCameras[EAST]->topleft * factor;
		dirCameras[SOUTH]->topleft = dirCameras[SOUTH]->topleft * factor;
		dirCameras[WEST]->topleft = dirCameras[WEST]->topleft * factor;
		dirCameras[TOP]->w = mapRes;
		dirCameras[TOP]->h = mapRes;
		dirCameras[BOT]->w = mapRes;
		dirCameras[BOT]->h = mapRes;
		dirCameras[NORTH]->w = mapRes;
		dirCameras[NORTH]->h = mapRes;
		dirCameras[EAST]->w = mapRes;
		dirCameras[EAST]->h = mapRes;
		dirCameras[SOUTH]->w = mapRes;
		dirCameras[SOUTH]->h = mapRes;
		dirCameras[WEST]->w = mapRes;
		dirCameras[WEST]->h = mapRes;
		//
		dirDepthMaps[TOP] = new float[mapRes * mapRes];
		dirDepthMaps[BOT] = new float[mapRes * mapRes];
		dirDepthMaps[NORTH] = new float[mapRes * mapRes];
		dirDepthMaps[EAST] = new float[mapRes * mapRes];
		dirDepthMaps[SOUTH] = new float[mapRes * mapRes];
		dirDepthMaps[WEST] = new float[mapRes * mapRes];
	}

	ShadowMap::ShadowMap(int resolution, const char * pathN, const char* pathE, const char* pathW, const char* pathS, const char* pathT, const char* pathB) {
		mapRes = resolution;
		pos = V3(0,0,0);
		/* top is +y
		 * north is +z
		 * east is +x */
		 //create the directional buffers:
		dirBuffer[TOP] = new FrameBuffer(0, 0, mapRes, mapRes);
		dirBuffer[BOT] = new FrameBuffer(0, 0, mapRes, mapRes);
		dirBuffer[NORTH] = new FrameBuffer(0, 0, mapRes, mapRes);
		dirBuffer[SOUTH] = new FrameBuffer(0, 0, mapRes, mapRes);
		dirBuffer[WEST] = new FrameBuffer(0, 0, mapRes, mapRes);
		dirBuffer[EAST] = new FrameBuffer(0, 0, mapRes, mapRes);
		//create the directional cameras:
		dirCameras[TOP] = new PPC(V3(0, 0, 1), V3(1, 0, 0), V3(-1, 1, -1), pos);
		dirCameras[BOT] = new PPC(V3(0, 0, 1), V3(-1, 0, 0), V3(1, -1, -1), pos);
		dirCameras[NORTH] = new PPC(V3(0, 0, 1), V3(0, -1, 0), V3(1, 1, -1), pos);
		dirCameras[EAST] = new PPC(V3(-1, 0, 0), V3(0, -1, 0), V3(1, 1, 1), pos);
		dirCameras[SOUTH] = new PPC(V3(0, 0, -1), V3(0, -1, 0), V3(-1, 1, 1), pos);
		dirCameras[WEST] = new PPC(V3(1, 0, 0), V3(0, -1, 0), V3(-1, 1, -1), pos);
		float factor = resolution / 2;
		dirCameras[TOP]->topleft = dirCameras[TOP]->topleft * factor;
		dirCameras[BOT]->topleft = dirCameras[BOT]->topleft * factor;
		dirCameras[NORTH]->topleft = dirCameras[NORTH]->topleft * factor;
		dirCameras[EAST]->topleft = dirCameras[EAST]->topleft * factor;
		dirCameras[SOUTH]->topleft = dirCameras[SOUTH]->topleft * factor;
		dirCameras[WEST]->topleft = dirCameras[WEST]->topleft * factor;
		dirCameras[TOP]->w = mapRes;
		dirCameras[TOP]->h = mapRes;
		dirCameras[BOT]->w = mapRes;
		dirCameras[BOT]->h = mapRes;
		dirCameras[NORTH]->w = mapRes;
		dirCameras[NORTH]->h = mapRes;
		dirCameras[EAST]->w = mapRes;
		dirCameras[EAST]->h = mapRes;
		dirCameras[SOUTH]->w = mapRes;
		dirCameras[SOUTH]->h = mapRes;
		dirCameras[WEST]->w = mapRes;
		dirCameras[WEST]->h = mapRes;
		//
		dirDepthMaps[TOP] = new float[mapRes * mapRes];
		dirDepthMaps[BOT] = new float[mapRes * mapRes];
		dirDepthMaps[NORTH] = new float[mapRes * mapRes];
		dirDepthMaps[EAST] = new float[mapRes * mapRes];
		dirDepthMaps[SOUTH] = new float[mapRes * mapRes];
		dirDepthMaps[WEST] = new float[mapRes * mapRes];
		//gen buffers
		this->loadEnvMap(pathN,pathE,pathW,pathS,pathT,pathB);
	}

	void ShadowMap::clearDepth(ShadowDir dir) {
		for (int i = 0; i < mapRes * mapRes; ++i) {
			dirDepthMaps[dir][i] = MAXDEPTH;
		}
	}

	void ShadowMap::clearDepth() {
		clearDepth(TOP);
		clearDepth(BOT);
		clearDepth(NORTH);
		clearDepth(EAST);
		clearDepth(SOUTH);
		clearDepth(WEST);
	}

	void ShadowMap::genABC() {
		dirCameras[TOP]->genABC();
		dirCameras[BOT]->genABC();
		dirCameras[NORTH]->genABC();
		dirCameras[WEST]->genABC();
		dirCameras[SOUTH]->genABC();
		dirCameras[EAST]->genABC();
	}

	void ShadowMap::drawPlane(Plane* plane) {
		draw3DTriangleDepth(plane->c, plane->b, plane->a);
		draw3DTriangleDepth(plane->c, plane->d, plane->b);
	}

	void ShadowMap::drawTM(TriangleMesh* tm) {
		;
		for (int tri = 0; tri < tm->trisN; tri++) {
			int vi0 = tm->tris[3 * tri];
			int vi1 = tm->tris[3 * tri + 1];
			int vi2 = tm->tris[3 * tri + 2];
			draw3DTriangleDepth(tm->verts[vi0], tm->verts[vi1], tm->verts[vi2]);
		}
	}

	void ShadowMap::draw3DTriangleDepth(V3 point1, V3 point2, V3 point3) {
		draw3DTriangleDepth(TOP, point1, point2, point3);
		draw3DTriangleDepth(BOT, point1, point2, point3);
		draw3DTriangleDepth(NORTH, point1, point2, point3);
		draw3DTriangleDepth(EAST, point1, point2, point3);
		draw3DTriangleDepth(SOUTH, point1, point2, point3);
		draw3DTriangleDepth(WEST, point1, point2, point3);
	}

	V3 pp1, pp2, pp3, v1, v2, v3, C;
	int ui0, vi0, ui1, vi1, ui2, vi2, umin, vmin, umax, vmax;
	bool ShadowMap::draw3DTriangleDepth(ShadowDir dir, V3 point1, V3 point2, V3 point3) {
		//check whether all vertexes are within projection plane
		if (!dirCameras[dir]->project(point1, pp1))
			return false;
		if (!dirCameras[dir]->project(point2, pp2))
			return false;
		if (!dirCameras[dir]->project(point3, pp3))
			return false;

		//cerr << " [" << dirCameras[dir]->topleft << "]" << endl;

		//round to int
		ui0 = floorf(pp1[0] + 0.5);
		vi0 = floorf(pp1[1] + 0.5);
		ui1 = floorf(pp2[0] + 0.5);
		vi1 = floorf(pp2[1] + 0.5);
		ui2 = floorf(pp3[0] + 0.5);
		vi2 = floorf(pp3[1] + 0.5);

		//clamping
		umin = fminf(ui2, fminf(ui0, ui1)) - 1;
		vmin = fminf(vi2, fminf(vi0, vi1)) - 1;
		umax = fmaxf(ui2, fmaxf(ui0, ui1)) + 1;
		vmax = fmaxf(vi2, fmaxf(vi0, vi1)) + 1;
		if (umin < 0) { umin = 0; }
		if (vmin < 0) { vmin = 0; }
		if (umax > mapRes - 1) { umax = mapRes - 1; }
		if (vmax > mapRes - 1) { vmax = mapRes - 1; }

		//initialize projection matrix
		/*V3 a = camera->horizontal;
		V3 b = camera->vertical;
		V3 c = camera->topleft;*/
		v1 = point1;
		v2 = point2;
		v3 = point3;
		C = dirCameras[dir]->pos;
		//M33 abc(a, b, c);
		//M33 abc(camera->horizontal, camera->vertical, camera->topleft);
		//abc = abc.transpose();
		M33 q = M33(v1 - C, v2 - C, v3 - C).transpose();
		M33 qinv;
		if (!q.inverse(&qinv))
			return false;
		q = qinv * dirCameras[dir]->getABC();//abc


		//render using Model Space Coordinates
		for (int u = umin; u < umax; ++u) {
			for (int v = vmin; v < vmax; ++v) {
				//check sidedness
				int side1 = u * (vi1 - vi0) - v * (ui1 - ui0) - ui0 * vi1 + vi0 * ui1; //0 1
				int side2 = u * (vi2 - vi1) - v * (ui2 - ui1) - ui1 * vi2 + vi1 * ui2; //1 2
				int side3 = u * (vi0 - vi2) - v * (ui0 - ui2) - ui2 * vi0 + vi2 * ui0; //2 0 
				if (side1 < 0 || side2 < 0 || side3 < 0)
					if (side1 > 0 || side2 > 0 || side3 > 0) {
						continue;
					}
				V3 uv1 = V3(u, v, 1);
				V3 quv1 = (q * uv1);
				float w = (quv1[0] + quv1[1] + quv1[2]);
				V3 kl = quv1 / w;
				//V3 localPos = point1 * kl[0] + point2 * kl[1] + point3 * kl[2];
				V3 projPos = pp1 * kl[0] + pp2 * kl[1] + pp3 * kl[2];

				//if (projPos[2] > 0)
				//	cerr << dir << " [" << u << "," << v << "]" << endl;
				if (dirDepthMaps[dir][u*mapRes + v] > projPos[2]) {
					dirDepthMaps[dir][u*mapRes + v] = projPos[2];
					//cerr << 1 / w << endl;
				}
			}
		}
		return true;
	}

	V3 proj;
	float ShadowMap::getMapValueDir(ShadowDir dir, V3 ptr/*, PPC* cam1, V3 uvw1*/) {
		//V3 uvw2 = dirCameras[dir]->getABCinv()*(dirCameras[dir]->pos - cam1->pos)+ dirCameras[dir]->getABCinv()*cam1->getABC()*uvw1;
		dirCameras[dir]->project(ptr, proj);
		int coord = (int)(proj[1] + 0.5f) + (int)(proj[0]) * mapRes;
		float dist = proj[2];
		float storedist = dirDepthMaps[dir][coord];
		return dist - 0.01f < storedist;
	}

	V3 dir;
	float ShadowMap::getMapValue(V3 point) {
		dir = point - pos;
		ShadowDir sdir = WEST;
		if (dirCameras[NORTH]->contained(point)) {
			sdir = NORTH;
		}
		else if (dirCameras[SOUTH]->contained(point)) {
			sdir = SOUTH;
		}
		else if (dirCameras[TOP]->contained(point)) {
			sdir = TOP;
		}
		else if (dirCameras[BOT]->contained(point)) {
			sdir = BOT;
		}
		else if (dirCameras[EAST]->contained(point)) {
			sdir = EAST;
		}
		float depthdist = getMapValueDir(sdir, point);
		return depthdist;
	}

	ShadowDir ShadowMap::getMapColorCoding(V3 point) {
		dir = point - pos;
		if (dirCameras[NORTH]->contained(point)) {
			return NORTH;
		}
		else if (dirCameras[SOUTH]->contained(point)) {
			return SOUTH;
		}
		else if (dirCameras[TOP]->contained(point)) {
			return TOP;
		}
		else if (dirCameras[BOT]->contained(point)) {
			return BOT;
		}
		else if (dirCameras[EAST]->contained(point)) {
			return EAST;
		}
		return WEST;
	}

	V3 ShadowMap::getEnvValueDir(ShadowDir dir, V3 normal) {
		//return (normal - pos)*0.5f+V3(0.5f,0.5f,0.5f);
		/*dirCameras[dir]->project(normal, proj);
		float x = proj[0];
		float y = proj[1];
		int x1 = (int)floorf(x + 0.5f) % mapRes;
		int x2 = ((int)floorf(x + 0.5f)+1) % mapRes;
		int y1 = (int)floorf(y + 0.5f) % mapRes;
		int y2 = ((int)floorf(y + 0.5f)+1) % mapRes;
		float xfactor = x2 - x;
		float yfactor = y2 - y;
		if (xfactor < 0) xfactor *= -1;
		if (yfactor < 0) yfactor *= -1;
		if (xfactor > 1) xfactor = 1;
		if (yfactor > 1) yfactor = 1;
		int coord1 = (int)(x2) + (int)(mapRes - 1 - y2) * mapRes;
		int coord2 = (int)(x1) + (int)(mapRes - 1 - y2) * mapRes;
		int coord3 = (int)(x2) + (int)(mapRes - 1 - y1) * mapRes;
		int coord4 = (int)(x1) + (int)(mapRes - 1 - y1) * mapRes;
		V3 col1 = dirBuffer[dir]->getDataPtrV3()[coord1] * xfactor + dirBuffer[dir]->getDataPtrV3()[coord2] * (1 - xfactor);
		V3 col2 = dirBuffer[dir]->getDataPtrV3()[coord3] * xfactor + dirBuffer[dir]->getDataPtrV3()[coord4] * (1 - xfactor);
		return (col1*yfactor+col2*(1-yfactor)).clamp();*/

		//
		dirCameras[dir]->project(normal, proj);
		float _u = proj[0];
		float _v = proj[1];
		//project into texture dimensions
		float u = (_u) - 0.5f;
		float v = ((1 - _v)) - 0.5f;
		u -= floor(u / mapRes)*mapRes;
		v -= floor(v / mapRes)*mapRes;
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
		l -= floor(l / mapRes)*mapRes;
		r -= floor(r / mapRes)*mapRes;
		b -= floor(b / mapRes)*mapRes;
		t -= floor(t / mapRes)*mapRes;
		//get colors
		V3 lt = dirBuffer[dir]->getDataPtrV3()[t*mapRes + l];
		V3 rt = dirBuffer[dir]->getDataPtrV3()[t*mapRes + r];
		V3 lb = dirBuffer[dir]->getDataPtrV3()[b*mapRes + l];
		V3 rb = dirBuffer[dir]->getDataPtrV3()[b*mapRes + r];
		//interpolate
		V3 top = (lt*dl) + (rt*dr);
		V3 bot = (lb*dl) + (rb*dr);
		V3 col = (top*dt) + (bot*db);
		return col;
	}

	V3 ShadowMap::getEnvValue(V3 normal) {
		V3 posi = pos + normal;
		ShadowDir sdir = WEST;
		if (dirCameras[NORTH]->contained(posi)) {
			sdir = NORTH;
		}
		else if (dirCameras[SOUTH]->contained(posi)) {
			sdir = SOUTH;
		}
		else if (dirCameras[TOP]->contained(posi)) {
			sdir = TOP;
		}
		else if (dirCameras[BOT]->contained(posi)) {
			sdir = BOT;
		}
		else if (dirCameras[EAST]->contained(posi)) {
			sdir = EAST;
		}
		return getEnvValueDir(sdir, posi);
	}

	V3 ShadowMap::dirToColor(ShadowDir dir) {
		switch (dir) {
		case NORTH:
			return V3(1, 0, 0);
		case EAST:
			return V3(1, 1, 0);
		case WEST:
			return V3(0, 1, 1);
		case SOUTH:
			return V3(1, 0, 1);
		case TOP:
			return V3(0, 1, 0);
		case BOT:
			return V3(0, 0, 1);
		}
	}

	void ShadowMap::setPos(V3 _pos) {
		pos = _pos;
		dirCameras[TOP]->pos = pos;
		dirCameras[BOT]->pos = pos;
		dirCameras[NORTH]->pos = pos;
		dirCameras[EAST]->pos = pos;
		dirCameras[SOUTH]->pos = pos;
		dirCameras[WEST]->pos = pos;
	}

	void ShadowMap::renderBBDir(ShadowDir dir, PPC* ppc, FrameBuffer* fb) {
		dirCameras[dir]->visualize(ppc, fb, 5);
	}

	void ShadowMap::renderBB(PPC* ppc, FrameBuffer* fb) {
		renderBBDir(TOP, ppc, fb);
		renderBBDir(BOT, ppc, fb);
		renderBBDir(NORTH, ppc, fb);
		renderBBDir(WEST, ppc, fb);
		renderBBDir(SOUTH, ppc, fb);
		renderBBDir(EAST, ppc, fb);
	}

	void ShadowMap::loadEnvMapDir(ShadowDir dir, const char* path) {
		dirBuffer[dir]->LoadTiff((char*)path);
		//convert the loaded data into quickly readable V3 form
		dirBuffer[dir]->pix2v3();
	}

	void ShadowMap::loadEnvMap(const char * pathN, const char* pathE, const char* pathW, const char* pathS, const char* pathT, const char* pathB) {
		loadEnvMapDir(TOP, pathT);
		loadEnvMapDir(BOT, pathB);
		loadEnvMapDir(NORTH, pathN);
		loadEnvMapDir(WEST, pathW);
		loadEnvMapDir(SOUTH, pathS);
		loadEnvMapDir(EAST, pathE);
	}

	void ShadowMap::drawEnvPlane(Plane* plane) {
		plane->draw(dirCameras[TOP], dirBuffer[TOP]);
		plane->draw(dirCameras[BOT], dirBuffer[BOT]);
		plane->draw(dirCameras[NORTH], dirBuffer[NORTH]);
		plane->draw(dirCameras[EAST], dirBuffer[EAST]);
		plane->draw(dirCameras[SOUTH], dirBuffer[SOUTH]);
		plane->draw(dirCameras[WEST], dirBuffer[WEST]);
		/*for (int dir = 0; dir < 6; dir++) {
			plane->draw(dirCameras[dir], dirBuffer[dir]);
		}*/
	}

	void ShadowMap::drawEnvTM(TriangleMesh* tm) {
		tm->draw(dirCameras[TOP], dirBuffer[TOP]);
		tm->draw(dirCameras[BOT], dirBuffer[BOT]);
		tm->draw(dirCameras[NORTH], dirBuffer[NORTH]);
		tm->draw(dirCameras[EAST], dirBuffer[EAST]);
		tm->draw(dirCameras[SOUTH], dirBuffer[SOUTH]);
		tm->draw(dirCameras[WEST], dirBuffer[WEST]);
		/*for (int dir = 0; dir < 6; dir++) {
			tm->draw(dirCameras[dir], dirBuffer[dir]);
		}*/
	}

	void ShadowMap::clearEnv() {
		dirBuffer[TOP]->clear();
		dirBuffer[BOT]->clear();
		dirBuffer[NORTH]->clear();
		dirBuffer[EAST]->clear();
		dirBuffer[WEST]->clear();
		dirBuffer[SOUTH]->clear();
		//
		dirBuffer[TOP]->refreshDepth(5000);
		dirBuffer[BOT]->refreshDepth(5000);
		dirBuffer[NORTH]->refreshDepth(5000);
		dirBuffer[EAST]->refreshDepth(5000); 
		dirBuffer[WEST]->refreshDepth(5000);
		dirBuffer[SOUTH]->refreshDepth(5000);
	}
}