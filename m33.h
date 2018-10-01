#pragma once
#include "v3.h"
#include <math.h>

class M33 {
public:
	V3 rows[3];
	//
	M33();
	M33(V3& v0, V3& v1, V3& v2);
	M33(M33& mat);
	V3& operator[](int i);
	V3 getColumn(int i);
	M33 setColumn(int i, V3 value);
	//M * v multiplied by vector from the right
	V3 operator*(V3 v0);
	M33 operator*(M33 m1);
	M33 inverse();
	M33 transpose();
	void setAsRotationX(float degrees);
	void setAsRotationY(float degrees);
	void setAsRotationZ(float degrees);
	friend ostream& operator<<(ostream& os, const M33 dt);
	friend istream& operator>>(istream& is, M33& dt);
	//sets this matrix to be an arbitrary cooridnate system with major axis through dir
	void setAsCoordinateSystem(V3 dir);
};