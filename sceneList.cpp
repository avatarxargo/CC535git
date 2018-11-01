#include "sceneList.h"
/**
*	Title:	sceneList.cpp
*	Author: David Hrusa
*	Date:	10/30/2018
*	Desc:	Contains all the renderables in a scene.
*/
#include "renderable.h"
#include "framebuffer.h"
#include "shadowmap.h"

void SceneList::addItem(Renderable* ren) {
	renderables.insert(renderables.end(), ren);
}

void SceneList::render(FrameBuffer* fb, PPC* ppc) {
	for (int i = 0; i < renderables.size() - 1; ++i) {
		renderables[i]->draw(ppc, fb);
	}
}


void SceneList::renderCubeEnv(ShadowMapNS::ShadowMap* sm) {
	for (int i = 0; i < renderables.size() - 1; ++i) {
		switch (renderables[i]->getType()) {
		case REN_PLANE:
			sm->drawEnvPlane((Plane*)renderables[i]);
			continue;
		case REN_TM:
			sm->drawEnvTM((TriangleMesh*)renderables[i]);
			continue;
		}
	}
}


void SceneList::renderCubeDepth(ShadowMapNS::ShadowMap* sm) {
	sm->genABC();
	for (int i = 0; i < renderables.size() - 1; ++i) {
		RenderableType type = renderables[i]->getType();
		switch (type) {
		case REN_PLANE:
			sm->drawPlane((Plane*)renderables[i]);
			continue;
		case REN_TM:
			sm->drawTM((TriangleMesh*)renderables[i]);
			continue;
		}
	}
}