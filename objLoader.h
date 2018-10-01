/**
 *	Title:	objLoader.cpp
 *	Author: David Hrusa
 *	Date:	9/10/2018
 *	Desc:	facilitates the reading of .obj files and converting them to our internal triangle mesh representation.
 */

#pragma once
#include "tm.h"
#include "OBJ_Loader.h"

namespace objLoader {

	void loadObj(const char* path, TriangleMesh* target);
};