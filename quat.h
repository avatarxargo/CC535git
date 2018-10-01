/**
 *	Title:	quat.h
 *	Author: David Hrusa
 *	Date:	9/17/2018
 *	Desc:	A general Quaternion Class - mainly for rotations
 *			Inspired by: http://www.ncsa.illinois.edu/People/kindr/emtc/quaternions/quaternion.c++
 *			https://www.haroldserrano.com/blog/developing-a-math-engine-in-c-implementing-quaternions
 */

#define PI 3.1415927410125732421875

#pragma once
#include "v3.h"
#include "math.h"

class Quaternion {
public:
	//A Quaternion consists of an axis and an angle.
	float w;
	V3 v;
	Quaternion() : w(0), v() {}
	Quaternion(float w, float x, float y, float z);
	Quaternion(float w, V3 v);
	//[w, v] (where v = (x, y, z) is called a "vector" and w is called a "scalar") 
	//https://www.gamasutra.com/view/feature/131686/rotating_objects_using_quaternions.php
	Quaternion operator+(Quaternion q1);
	Quaternion operator-(Quaternion q1);
	Quaternion operator*(Quaternion q1);
	float norm();
	Quaternion conjugate();
	Quaternion inverse();
	Quaternion unit();
	Quaternion convertToUnitNormQuaternion();
	V3 rotateVectorAboutAngleAndAxis(V3 target);
	//
	void setFromEuler(float x, float y, float z);
	void setFromAngelAxis(float angle, V3 axis);
};