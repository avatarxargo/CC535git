#include "v3.h"

#include "m33.h"

V3::V3(float x, float y, float z) {

	xyz[0] = x;
	xyz[1] = y;
	xyz[2] = z;

}

V3::V3(const V3 &v0) {
	xyz[0] = v0.xyz[0];
	xyz[1] = v0.xyz[1];
	xyz[2] = v0.xyz[2];

}

V3::V3(unsigned int color) {
	xyz[0] = (float)(color & 0xFF) / (float)0xFF;
	xyz[1] = (float)((color >> 8) & 0xFF) / (float)0xFF;
	xyz[2] = (float)((color >> 16) & 0xFF) / (float)0xFF;
}


float& V3::operator[](int i) {
	return xyz[i];
}

V3 V3::operator+(V3 v1) {
	V3 &v0 = *this;
	return V3(v0[0] + v1[0], v0[1] + v1[1], v0[2] + v1[2]);
}

V3 V3::operator-(V3 v1) {
	V3 &v0 = *this;
	return V3(v0[0] - v1[0], v0[1] - v1[1], v0[2] - v1[2]);
}

float V3::operator*(V3 v1) {
	V3 &v0 = *this;
	return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

V3 V3::operator*(float scalar) {
	V3 &v0 = *this;
	return V3(v0[0] * scalar, v0[1] * scalar, v0[2] * scalar);
}

V3 V3::operator/(float scalar) {
	V3 &v0 = *this;
	return V3(v0[0] / scalar, v0[1] / scalar, v0[2] / scalar);
}

V3 V3::operator%(V3 v1) {
	V3 &v0 = *this;
	return V3(
		v0[1] * v1[2] - v0[2] * v1[1],
		v0[2] * v1[0] - v0[0] * v1[2],
		v0[0] * v1[1] - v0[1] * v1[0]
	);
}

V3 V3::operator^(V3 v1) {
	V3 &v0 = *this;
	return V3(
		v0[0] * v1[0],
		v0[1] * v1[1],
		v0[2] * v1[2]
	);
}

/*ostream & operator<<(ostream & os, V3& v0) {
	os << '[' << v0[0] << ',' << v0[1] << ',' << v0[2] << ']';
	return os;
}*/

ostream& operator<<(ostream& ostr, V3 v) {

	return ostr << v[0] << " " << v[1] << " " << v[2];

}

istream& operator>>(istream& is, V3& v0) {
	is >> v0[0] >> v0[1] >> v0[2];
	return is;
}

float V3::len() {
	V3 &v0 = *this;
	return sqrt(v0[0] * v0[0] + v0[1] * v0[1] + v0[2] * v0[2]);
}

V3 V3::norm() {
	V3 &v0 = *this;
	return V3(v0) / this->len();
}

V3 V3::clamp() {
	V3 &v0 = *this;
	return V3(fminf(v0[0],1), fminf(v0[1], 1), fminf(v0[2], 1));
}


unsigned int V3::getColor() {
	V3 &v0 = *this;
	return v0.getColor(1);
}

unsigned int V3::getColor(float alpha) {
	V3 &v0 = *this;
	unsigned int valr = (unsigned int)(v0[0] * 0xFF) & 0xFF;
	unsigned int valg = ((unsigned int)(v0[1] * 0xFF) & 0xFF) << 8;
	unsigned int valb = ((unsigned int)(v0[2] * 0xFF) & 0xFF) << 16;
	unsigned int vala = (((unsigned int)(alpha * 0xFF) & 0xFF) << 24);
	return valr + valg + valb + vala;
}


V3 V3::pointRotate(V3 axisOrigin, V3 axisDir, float angle) {
	//shift
	V3 v1 = *this - axisOrigin;
	//project to new origin
	M33 projectionMatrix, rotationMatrix;
	projectionMatrix.setAsCoordinateSystem(axisDir);
	v1 = projectionMatrix * v1;
	//rotate
	rotationMatrix.setAsRotationX(angle);
	v1 = rotationMatrix * v1;
	//project back to initial coordinates
	M33 projectionMatrixInv;
	projectionMatrix.inverse(&projectionMatrixInv);
	v1 = projectionMatrixInv * v1;
	//shift back
	v1 = v1 + axisOrigin;
	return v1;
}

V3 V3::vectorRotate(V3 axisDir, float angle) {
	return pointRotate(V3(0, 0, 0), axisDir, angle);
}

V3 V3::reflect(V3 v0) {
	V3 &me = *this;
	V3 along = me * (me * v0);
	V3 delta = v0 - along;
	return along - delta;
}
