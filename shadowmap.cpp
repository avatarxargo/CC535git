#include "shadowmap.h"

namespace ShadowMapNS {
	ShadowMap::ShadowMap(V3 _pos, int resolution) {
		mapRes = resolution;
		pos = V3(_pos);
		/* top is +y
		 * north is +z
		 * east is +x */
		dirCameras[TOP] = new PPC(V3(0, 0, 1), V3(1, 0, 0), V3(-1, 1, -1), pos);
		dirCameras[BOT] = new PPC(V3(0, 0, 1), V3(-1, 0, 0), V3(1, -1, -1), pos);
		dirCameras[NORTH] = new PPC(V3(0, 0, 1), V3(0, -1, 0), V3(1, 1, -1), pos);
		dirCameras[EAST] = new PPC(V3(1, 0, 0), V3(0, -1, 0), V3(1, 1, 1), pos);
		dirCameras[SOUTH] = new PPC(V3(0, 0, -1), V3(0, -1, 0), V3(-1, 1, 1), pos);
		dirCameras[WEST] = new PPC(V3(-1, 0, 0), V3(0, -1, 0), V3(-1, 1, -1), pos);
		//
		dirDepthMaps[TOP] = new float[mapRes * mapRes];
		dirDepthMaps[BOT] = new float[mapRes * mapRes];
		dirDepthMaps[NORTH] = new float[mapRes * mapRes];
		dirDepthMaps[EAST] = new float[mapRes * mapRes];
		dirDepthMaps[SOUTH] = new float[mapRes * mapRes];
		dirDepthMaps[WEST] = new float[mapRes * mapRes];
	}

	void ShadowMap::clearDepth(ShadowDir dir) {
		for (int i = 0; i < mapRes * mapRes; ++i) {
			dirDepthMaps[dir][i] = 0;
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

	void ShadowMap::drawPlane(Plane* plane) {

	}

	void ShadowMap::drawPlane(TriangleMesh* tm) {

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
					continue;
				V3 uv1 = V3(u, v, 1);
				V3 quv1 = (q * uv1);
				float w = (quv1[0] + quv1[1] + quv1[2]);
				if (w*w < 0.00001) { continue; }
				V3 kl = quv1 / w;
				V3 localPos = point1 * kl[0] + point2 * kl[1] + point3 * kl[2];
				V3 projPos = pp1 * kl[0] + pp2 * kl[1] + pp3 * kl[2];

				dirDepthMaps[dir][u*mapRes + v] = projPos[2];
				//V3 col = lightEnvironment->getLightingAtVertex(mat, localPos, localCoord, localNormal);

				/*if (opaque) {
					setZ(u, v, 1 / w, col.getColor());
				}
				else {
					float alpha = mat->getOpacity()->getColorNearestV3(localCoord[0], localCoord[1])[0];
					setZBlend(u, v, 1 / w, col.getColor(), alpha);
				}*/
			}
		}
		return true;
	}
}