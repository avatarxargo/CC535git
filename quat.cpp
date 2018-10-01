#include "quat.h"

Quaternion::Quaternion(float _w, float x, float y, float z) {
	w = _w;
	v = V3(x, y, z);
}
Quaternion::Quaternion(float _w, V3 _v) {
	w = _w;
	v = _v;
}

void Quaternion::setFromAngelAxis(float angle, V3 axis) {
	//q = [cos(Q/2), sin(Q /2)v] (where Q is an angle and v is an axis) 
	w = cosf(angle / 2);
	v = axis * sinf(angle / 2);
}

Quaternion Quaternion::operator+(Quaternion q1) {
	return Quaternion(w + q1.w, v + q1.v);
}

Quaternion Quaternion::operator-(Quaternion q1) {
	return Quaternion(w - q1.w, v - q1.v);
}

Quaternion Quaternion::operator*(Quaternion q1) {
	return Quaternion(w * q1.w - v*q1.v, q1.v*w+v*q1.w + v%q1.v);
}

float Quaternion::norm() {
	return sqrt(w*w+v*v);
}

Quaternion Quaternion::conjugate() {
	return Quaternion(w,v*(-1));
}

Quaternion Quaternion::unit() {
	if (norm() != 0) {

		float normValue = 1 / norm();
		return Quaternion(w*normValue, v*normValue);
	}
}

Quaternion Quaternion::inverse() {
	float absoluteValue = norm();
	absoluteValue *= absoluteValue;
	absoluteValue = 1 / absoluteValue;

	Quaternion conjugateValue = conjugate();

	float scalar = conjugateValue.w*absoluteValue;
	V3 imaginary = conjugateValue.v*absoluteValue;

	return Quaternion(scalar, imaginary);
}

Quaternion Quaternion::convertToUnitNormQuaternion() {

	float angle = PI*w/180.0f;
	return Quaternion(cosf(angle*0.5), v.norm() * sinf(angle*0.5));
}

//A real quaternion has a Zero vector term. 
//A pure quaternion has a Zero scalar term.
//A vector rotation occurs when you multiply a pure quaternion p by a unit-norm quaternion q and its inverse:
//p' = qpq^(-1)
V3 Quaternion::rotateVectorAboutAngleAndAxis(V3 target) {
	//convert our vector to a pure quaternion
	Quaternion p(0, target);
	//normalize the axis
	V3 uAxis = v.norm();
	float uAngle = w;
	//create the real quaternion
	Quaternion q(uAngle, uAxis);
	//convert quaternion to unit norm quaternion
	q = q.convertToUnitNormQuaternion();
	//Get the inverse of the quaternion
	Quaternion qInverse = q.inverse();
	//rotate the quaternion
	Quaternion rotatedVector = q * p * qInverse;
	//return the vector part of the quaternion
	return rotatedVector.v;
}

void Quaternion::setFromEuler(float x, float y, float z) {
	/*
	q = qyaw qpitch qroll where:
	qroll = [cos(y / 2), (sin(y / 2), 0, 0)]
	qpitch = [cos(q / 2), (0, sin(q / 2), 0)]
	qyaw = [cos(f / 2), (0, 0, sin(f / 2)]
	*/
	Quaternion qroll(cosf(y / 2), V3(sinf(y / 2), 0, 0));
}