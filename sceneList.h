#pragma once
/**
 *	Title:	sceneList.h
 *	Author: David Hrusa
 *	Date:	10/30/2018
 *	Desc:	Contains all the renderables in a scene.
 */
#include <vector>

namespace ShadowMapNS {
	enum ShadowDir;
	class ShadowMap;
}
class Renderable;
class FrameBuffer;
class PPC;

class SceneList {
public:
	//generates inverse projection matrix for all of the cameras.
	std::vector<Renderable*> renderables;

	//adds a new renderable item to the list
	void addItem(Renderable* ren);
	//renders the scene to a buffer
	void render(FrameBuffer* fb, PPC* ppc);
	//renders the scene as an environemnt map
	void renderCubeEnv(ShadowMapNS::ShadowMap* sm);
	//renders the scene as a depth shadow map
	void renderCubeDepth(ShadowMapNS::ShadowMap* sm);
};