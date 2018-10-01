#include "m33.h"

#define PI 3.141592654
#define TO_RAD PI/180

M33::M33() {
	rows[0] = V3(0, 0, 0);
	rows[1] = V3(0, 0, 0);
	rows[2] = V3(0, 0, 0);
}


M33::M33(V3& v0, V3& v1, V3& v2) {
	rows[0] = V3(v0);
	rows[1] = V3(v1);
	rows[2] = V3(v2);
}

M33::M33(M33& m0) {
	rows[0] = V3(m0[0]);
	rows[1] = V3(m0[1]);
	rows[2] = V3(m0[2]);
}

//M * v
V3 M33::operator*(V3 v0) {
	M33 &m0 = *this;
	V3 v1 = V3(m0[0] * v0, m0[1] * v0, m0[2] * v0);
	return v1;
}

M33 M33::operator*(M33 m1) {
	M33 &m0 = *this;
	M33 m2 = M33(m0*m1.getColumn(0), m0*m1.getColumn(1), m0*m1.getColumn(2));
	return m2.transpose();
}

V3 M33::getColumn(int i) {
	return V3(rows[0][i], rows[1][i], rows[2][i]);
}

M33 M33::setColumn(int index, V3 value) {
	M33 m1;
	for (int i = 0; i < 3; i++) {
		if (i == index) {
			m1[i] = value;
		}
		else {
			m1[i] = getColumn(i);
		}
	}
	return m1.transpose();
}

M33 M33::transpose() {
	return M33(getColumn(0), getColumn(1), getColumn(2));
}

M33 M33::inverse() {
	M33 &m0 = *this;
	M33 m1 = M33();
	float determinant = 0;
	for (int i = 0; i < 3; i++) {
		determinant += (m0[0][i] * (m0[1][(i + 1) % 3] * m0[2][(i + 2) % 3] - m0[1][(i + 2) % 3] * m0[2][(i + 1) % 3]));
	}
	if (determinant == 0) {
		cerr << "Matrix: \n" << m0 << "\nis singular! No inverse possible." << endl;
		return m1;
	}
	//inverse
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m1[i][j] = ((m0[(j + 1) % 3][(i + 1) % 3] * m0[(j + 2) % 3][(i + 2) % 3]) - (m0[(j + 1) % 3][(i + 2) % 3] * m0[(j + 2) % 3][(i + 1) % 3])) / determinant;
		}
	}
	return m1;
}

ostream & operator<<(ostream & os, M33 m) {
	return os << m[0] << endl << m[1] << endl << m[2];
	return os;
}

istream& operator>>(istream& is, M33& dt) {
	is >> dt[0] >> dt[1] >> dt[2];
	return is;
}

V3& M33::operator[](int i) {
	return rows[i];
}

void M33::setAsRotationX(float degrees) {
	M33 &m0 = *this;
	float rads = degrees * TO_RAD;
	m0[0] = V3(1, 0, 0);
	m0[1] = V3(0, cosf(rads), -sinf(rads));
	m0[2] = V3(0, sinf(rads), cosf(rads));
}

void M33::setAsRotationY(float degrees) {
	M33 &m0 = *this;
	float rads = degrees * TO_RAD;
	m0[0] = V3(cosf(rads), 0, sinf(rads));
	m0[1] = V3(0, 1, 0);
	m0[2] = V3(-sinf(rads), 0, cosf(rads));
}

void M33::setAsRotationZ(float degrees) {
	M33 &m0 = *this;
	float rads = degrees * TO_RAD;
	m0[0] = V3(cosf(rads), -sinf(rads), 0);
	m0[1] = V3(sinf(rads), cosf(rads), 0);
	m0[2] = V3(0, 0, 1);
}

void M33::setAsCoordinateSystem(V3 dir) {
	M33 &m0 = *this;
	//set the dir as principal axis.
	m0[0] = dir.norm();
	//find the most different second base axis.
	V3 type = V3(1, 0, 0);
	if ((dir - V3(1, 0, 0)).len() < (dir - V3(0, 1, 0)).len()) {
		if ((dir - V3(0, 1, 0)).len() < (dir - V3(0, 0, 1)).len()) {
			type = V3(0, 0, 1);
		}
		else {
			type = V3(0, 1, 0);
		}
	}
	else {
		if ((dir - V3(1, 0, 0)).len() < (dir - V3(0, 0, 1)).len()) {
			type = V3(0, 0, 1);
		}
	}
	m0[1] = (type % dir).norm();
	//3rd is the cross product of the two.
	m0[2] = (m0[0] % m0[1]).norm();
}