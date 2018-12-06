//#define GEOM_SHADER

#include "CGInterface.h"
#include "v3.h"
#include "scene.h"

#include <iostream>

using namespace std;

CGInterface::CGInterface() {};

void CGInterface::PerSessionInit() {
  glEnable(GL_DEPTH_TEST);

  CGprofile latestVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
  CGprofile latestGeometryProfile = cgGLGetLatestProfile(CG_GL_GEOMETRY);
  CGprofile latestPixelProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

#ifdef GEOM_SHADER
  if (latestGeometryProfile == CG_PROFILE_UNKNOWN) {
	  cerr << "ERROR: geometry profile is not available" << endl;
    exit(0);
  }
#endif

  cgGLSetOptimalOptions(latestGeometryProfile);
  CGerror Error = cgGetError();
  if (Error) {
	  cerr << "CG ERROR "<<Error<<": " << cgGetErrorString(Error) << endl;
  }

  cout << "Info: Latest GP Profile Supported: " << CG_PROFILE_UNKNOWN  << "," << latestGeometryProfile << cgGetProfileString(latestGeometryProfile) << endl;

  geometryCGprofile = latestGeometryProfile;

  cout << "Info: Latest VP Profile Supported: " << cgGetProfileString(latestVertexProfile) << endl;
  cout << "Info: Latest FP Profile Supported: " << cgGetProfileString(latestPixelProfile) << endl;

  vertexCGprofile = latestVertexProfile;
  pixelCGprofile = latestPixelProfile;
  cgContext = cgCreateContext();  


}

bool ShaderOneInterface::PerSessionInit(CGInterface *cgi) {

#ifdef GEOM_SHADER
  geometryProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE, 
    "shaderOne.cg", cgi->geometryCGprofile, "GeometryMain", NULL);
  if (geometryProgram == NULL)  {
    CGerror Error = cgGetError();
    cerr << "Shader One Geometry Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
    cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
    return false;
  }
#endif

  vertexProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE, 
    "shaderOne.cg", cgi->vertexCGprofile, "VertexMain", NULL);
  if (vertexProgram == NULL) {
    CGerror Error = cgGetError();
    cerr << "Shader One Vertex Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
    cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
    return false;
  }

  fragmentProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE, 
    "shaderOne.cg", cgi->pixelCGprofile, "FragmentMain", NULL);
  if (fragmentProgram == NULL)  {
    CGerror Error = cgGetError();
    cerr << "Shader One Fragment Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
    cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
    return false;
  }

	// load programs
#ifdef GEOM_SHADER
	cgGLLoadProgram(geometryProgram);
#endif
	cgGLLoadProgram(vertexProgram);
	cgGLLoadProgram(fragmentProgram);

	// build some parameters by name such that we can set them later...
  vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj" );
  geometryModelViewProj = cgGetNamedParameter(geometryProgram, "modelViewProj" );
  fragmentGPUtex = cgGetNamedParameter(fragmentProgram, "gputexture");
  fragmentC0 = cgGetNamedParameter(fragmentProgram, "C0");
  fragmentC1 = cgGetNamedParameter(fragmentProgram, "C1");
  vertexMorphRadius = cgGetNamedParameter(vertexProgram, "MR");
  vertexMorphCenter = cgGetNamedParameter(vertexProgram, "MC");
  vertexMorphFraction = cgGetNamedParameter(vertexProgram, "Mf");

  return true;

}

void ShaderOneInterface::PerFrameInit() {

	//set parameters
	cgGLSetStateMatrixParameter(vertexModelViewProj, 
		CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

  cgGLSetStateMatrixParameter(
    geometryModelViewProj, 
	  CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

  cgSetParameter1i(fragmentGPUtex, 0);
 /*cgSetParameter1f(fragmentKa, scene->ka); //just for an effect with morphing
 AABB aabb = scene->tms[0].GetAABB();
// cerr << aabb.c0 << endl << aabb.c1 << endl;
	cgSetParameter3fv(fragmentC0, (float*)&aabb.c0);
	cgSetParameter3fv(fragmentC1, (float*)&aabb.c1);

	V3 C = (aabb.c0 + aabb.c1) * 0.5f;
	cgSetParameter3fv(vertexMorphCenter, (float*)&C);
	float mr = (aabb.c1 - aabb.c0).Length() / 3.0f;
	cgSetParameter1f(vertexMorphRadius, mr);
	cgSetParameter1f(vertexMorphFraction, scene->mf);*/
	
}

void ShaderOneInterface::PerFrameDisable() {
}


void ShaderOneInterface::BindPrograms() {

#ifdef GEOM_SHADER
  cgGLBindProgram(geometryProgram);
#endif
  cgGLBindProgram(vertexProgram);
  cgGLBindProgram(fragmentProgram);

}

void CGInterface::DisableProfiles() {

  cgGLDisableProfile(vertexCGprofile);
#ifdef GEOM_SHADER
  cgGLDisableProfile(geometryCGprofile);
#endif
  cgGLDisableProfile(pixelCGprofile);

}

void CGInterface::EnableProfiles() {

  cgGLEnableProfile(vertexCGprofile);
#ifdef GEOM_SHADER
  cgGLEnableProfile(geometryCGprofile);
#endif
  cgGLEnableProfile(pixelCGprofile);

}

