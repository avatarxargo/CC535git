#include "Light.h"
/**
 *	Title:	Light.h
 *	Author: David Hrusa
 *	Date:	10/2/2018
 *	Desc:	Stores information about a light source.
 */

Light::Light(V3 _color) {
	position = V3();
	color = _color;
	rangeStart = 0;
	rangeMax = 0;
}

Light::Light(V3 _position, V3 _color, float _rangeStart, float _rangeMax) {
	position = _position;
	color = _color;
	rangeStart = _rangeStart;
	rangeMax = _rangeMax;
	diff = rangeMax - rangeStart;
}