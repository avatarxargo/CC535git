#ifndef SHADOWMAP_H
#define SHADOWMAP_H
/**
 *	Title:	shadowmap.h
 *	Author: David Hrusa
 *	Date:	10/19/2018
 *	Desc:	Looks in 6 directions and casts shadows. Can also reuse the cameras ad an environment map.
 */
#include "ppc.h"
#include "renderable.h"
#include "framebuffer.h"

class Plane;
class V3;
class TriangleMesh;
class FrameBuffer;

namespace ShadowMapNS {

	enum ShadowDir { TOP = 0, BOT = 1, NORTH = 2, EAST = 3, SOUTH = 4, WEST = 5 };

	/* top is +y
	 * north is +z
	 * east is +x */
	class ShadowMap {
	public:
		//resolution of the camera image in each direction
		int mapRes;
		V3 pos;
		//the directional cameras
		PPC* dirCameras[6];// *top, *bot, *north, *east, *south, *west;
		FrameBuffer* dirBuffer[6];// *top, *bot, *north, *east, *south, *west;
		//the directional depth maps
		float * dirDepthMaps[6];// map_top, *map_bot, *map_north, *map_east, *map_south, *map_west;
		ShadowMap(V3 _pos, int resolution);
		//
		//updates the shadowmap values
		void clearDepth();
		//for given point, returns how lit it is.
		ShadowDir getMapColorCoding(V3 point);
		float getMapValue(V3 point/*, PPC* cam1*/);
		float getMapValueDir(ShadowDir dir, V3 ptr/*, PPC* cam1, V3 uvw1*/);
		V3 dirToColor(ShadowDir dir);
		//renders a triangle onto the dirDepthMap selected
		void drawPlane(Plane* plane);
		void drawTM(TriangleMesh* tm);
		void draw3DTriangleDepth(V3 point1, V3 point2, V3 point3);
		bool draw3DTriangleDepth(ShadowDir dir, V3 point1, V3 point2, V3 point3);
		//resets depth to max
		void genABC();
		void clearDepth(ShadowDir);
		void setPos(V3 pos);
		void renderBBDir(ShadowDir dir, PPC* ppc, FrameBuffer* fb);
		void renderBB(PPC* ppc, FrameBuffer* fb);
		//for env map:
		void drawEnvPlane(Plane* plane);
		void drawEnvTM(TriangleMesh* tm);
		void drawEnv3DTriangle(V3 point1, V3 point2, V3 point3);
		bool drawEnv3DTriangle(ShadowDir dir, V3 point1, V3 point2, V3 point3);
		//loads textures from files into each framebuffer
		void loadEnvMap(const char * pathN, const char* pathE, const char* pathW, const char* pathS, const char* pathT, const char* pathB);
	};
}
#endif