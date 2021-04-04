#include "LightSource.h"

LightSource::LightSource(const Colour& colour) : colour_(colour) {

}

LightSource::LightSource(const LightSource& lightSource) : colour_(lightSource.colour_) {

}

LightSource::~LightSource() {

}

LightSource& LightSource::operator=(const LightSource& lightSource) {
	if (this != &lightSource) {
		colour_ = lightSource.colour_;
	}
	return *this;

}
