#include "Object.h"

Object::Object() : transform() {

}

Object::Object(const Object& object) : transform(object.transform) {
	
}

Object::~Object() {

}

Object& Object::operator=(const Object& object) {
	if (this != &object) {
		transform = object.transform;
	}
	return *this;
}