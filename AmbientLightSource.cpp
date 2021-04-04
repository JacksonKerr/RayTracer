#include "AmbientLightSource.h"

AmbientLightSource::AmbientLightSource(const Colour& colour) : LightSource(colour) {

}

AmbientLightSource::AmbientLightSource(const AmbientLightSource& lightSource) :
LightSource(lightSource) {

}

AmbientLightSource::~AmbientLightSource() {

}

AmbientLightSource& AmbientLightSource::operator=(const AmbientLightSource& lightSource) {
	if (this != &lightSource) {
		LightSource::operator=(lightSource);
	}
	return *this;
}

Colour AmbientLightSource::getIlluminationAt(const Point& point) const {
	return colour_;
}

double AmbientLightSource::getDistanceToLight(const Point& point) const {
	return -1;
}

Direction AmbientLightSource::getLightDirection(const Point& point) const {
	return Direction();
}