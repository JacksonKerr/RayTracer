#include "Tube.h"

#include "utility.h"


Tube::Tube(double ratio) : Object(), ratio_(ratio) {
}

Tube::Tube(const Tube& tube) : Object(tube), ratio_(tube.ratio_) {

}

Tube::~Tube() {

}

Tube& Tube::operator=(const Tube& tube) {
	if (this != &tube) {
		Object::operator=(tube);
		ratio_ = tube.ratio_;
	}
	return *this;
}

std::vector<RayIntersection> Tube::intersect(const Ray& ray) const {
	std::vector<RayIntersection> result;

	/*****************************************************
	* Code to implement Ray-Tube interesection goes here *
	*****************************************************/

	return result;
}
