/**
 *	Title:	objLoader.cpp
 *	Author: David Hrusa
 *	Date:	9/10/2018
 *	Desc:	facilitates the reading of .obj files and converting them to our internal triangle mesh representation.
 */

#include "objLoader.h"

namespace objLoader {

	objl::Loader loader;

	void loadObj(const char* path, TriangleMesh* target) {
		loader.LoadFile(path);

		//TODO finish once the triangular mesh is implemented.

		//Process materials first.
		for (objl::Material mat : loader.LoadedMaterials) {

		}

		//For each mesh extract it's properties.
		for (objl::Mesh m : loader.LoadedMeshes) {
			for (objl::Vertex v : loader.LoadedVertices) {

			}

			for (unsigned int i : loader.LoadedIndices) {

			}
		}
	}
};