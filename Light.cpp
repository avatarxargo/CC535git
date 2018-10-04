#include "Light.h"
/**
 *	Title:	Light.h
 *	Author: David Hrusa
 *	Date:	10/2/2018
 *	Desc:	Stores information about a light source.
 */

Light::Light(V3 _position, V3 _color, float _rangeStart, float _rangeMax) {
	position = _position;
	color = _color;
	rangeStart = _rangeStart;
	rangeMax = _rangeMax;
}

V3 Light::getIntensity(V3 pt, V3 normal) {
	float distance = (position - pt).len();
	if (distance > rangeMax) {
		return V3(0,0,0);
	}
	if (distance < rangeStart) {
		return color;
	}
	return color*(1-((distance-rangeStart)/(rangeMax-rangeStart)));
}