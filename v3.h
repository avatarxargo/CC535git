#pragma once

#include <iostream>
#include <cmath>

using namespace std;

//forward declaration
class M33;

class V3 {
public:
	float xyz[3];
	//
	V3() {};
	V3(const V3 &v0);
	V3(float x, float y, float z);
	V3(unsigned int color);
	float& operator[](int i);
	V3 operator+(V3 v1);
	V3 operator-(V3 v1);
	//dot product
	float operator*(V3 v1);
	//cross product
	V3 operator%(V3 v1);
	//per element multi
	V3 operator^(V3 v1);
	V3 operator*(float scalar);
	V3 operator/(float scalar);
	V3 norm();
	unsigned int getColor();
	unsigned int getColor(float alpha);
	float len();
	/**Rotation of a point about arbitrary axis*/
	V3 pointRotate(V3 axisOrigin, V3 axisDir, float angle);
	/**Rotation of vector about arbirary direction*/
	V3 vectorRotate(V3 axisDir, float angle);
	friend ostream& operator<<(ostream& os, V3 v0);
	friend istream& operator>>(istream& is, V3& v0);
};