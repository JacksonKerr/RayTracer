#include "DirectionalLightSource.h"
#include "utility.h"

DirectionalLightSource::DirectionalLightSource(const Colour& colour, const Direction& direction) :
	LightSource(colour), direction_(direction) {

}

DirectionalLightSource::DirectionalLightSource(const DirectionalLightSource& lightSource) :
	LightSource(lightSource), direction_(lightSource.direction_) {

}

DirectionalLightSource::~DirectionalLightSource() {

}

DirectionalLightSource& DirectionalLightSource::operator=(const DirectionalLightSource& lightSource) {
	if (this != &lightSource) {
		LightSource::operator=(lightSource);
		direction_ = lightSource.direction_;
	}
	return *this;
}

Colour DirectionalLightSource::getIlluminationAt(const Point& point) const {
	return colour_;
}

double DirectionalLightSource::getDistanceToLight(const Point& point) const {
	return infinity;
}

Direction DirectionalLightSource::getLightDirection(const Point& point) const {
	return direction_;
}
