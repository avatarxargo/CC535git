#pragma once
/**
 *	Title:	shadowmap.h
 *	Author: David Hrusa
 *	Date:	10/19/2018
 *	Desc:	Looks in 6 directions and casts shadows
 */
#include "v3.h"
#include "ppc.h"
#include "renderable.h"
#include "plane.h"
#include "tm.h"

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
		//the directional depth maps
		float * dirDepthMaps[6];// map_top, *map_bot, *map_north, *map_east, *map_south, *map_west;
		ShadowMap(V3 _pos, int resolution);
		//
		//updates the shadowmap values
		void clearDepth();
		//updates the shadowmap values
		void renderDir(ShadowDir dir, Renderable* scene);
		//for given point, returns how lit it is.
		float getMapValue(V3 point);
		//renders a triangle onto the dirDepthMap selected
		void drawPlane(Plane* plane);
		void drawPlane(TriangleMesh* tm);
		void draw3DTriangleDepth(V3 point1, V3 point2, V3 point3);
		void draw3DTriangleDepth(ShadowDir dir, V3 point1, V3 point2, V3 point3);
		//resets depth to max
		void clearDepth(ShadowDir);
	};
}