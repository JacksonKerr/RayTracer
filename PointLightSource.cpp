#include "PointLightSource.h"

#include "utility.h"

PointLightSource::PointLightSource(const Colour& colour, const Point& location) :
LightSource(colour), location_(location) {

}

PointLightSource::PointLightSource(const PointLightSource& lightSource) :
LightSource(lightSource) {

}

PointLightSource::~PointLightSource() {

}

PointLightSource& PointLightSource::operator=(const PointLightSource& lightSource) {
	if (this != &lightSource) {
		LightSource::operator=(lightSource);
	}
	return *this;
}

Colour PointLightSource::getIlluminationAt(const Point& point) const {
	double distance = (location_ - point).norm();
	if (distance < epsilon) distance = epsilon;
	return (1.0 / (distance*distance)) * colour_;
}


double PointLightSource::getDistanceToLight(const Point& point) const {
	return (point - location_).norm();
}

Direction PointLightSource::getLightDirection(const Point& point) const {

	return point - location_;
}
