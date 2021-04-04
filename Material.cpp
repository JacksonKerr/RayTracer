#include "Material.h"

Material::Material() :
	ambientColour(1, 0, 1),
	diffuseColour(1, 0, 1),
	specularColour(0, 0, 0),
	specularExponent(1),
	mirrorColour(0, 0, 0) {
}
