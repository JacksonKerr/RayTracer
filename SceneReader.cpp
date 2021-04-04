#include "SceneReader.h"

#include "utility.h"

#include "Camera.h"
#include "PinholeCamera.h"

#include "LightSource.h"
#include "AmbientLightSource.h"
#include "DirectionalLightSource.h"
#include "PointLightSource.h"

#include "Material.h"

#include "Object.h"
#include "Plane.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Tube.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <queue>
#include <sstream>


SceneReader::SceneReader(Scene* scene) :
scene_(scene), startLine_(0) {

}

SceneReader::~SceneReader() {

}

void SceneReader::parseTokenBlock(std::queue<std::string>& tokenBlock) {
	std::string blockType = toUpper(tokenBlock.front());
	tokenBlock.pop();
	if (blockType == "SCENE") {
		parseSceneBlock(tokenBlock);
	} else if (blockType == "CAMERA") {
		parseCameraBlock(tokenBlock);
	} else if (blockType == "OBJECT") {
		scene_->addObject(parseObjectBlock(tokenBlock));
	} else if (blockType == "LIGHT") {
		parseLightBlock(tokenBlock);
	} else if (blockType == "MATERIAL") {
		parseMaterialBlock(tokenBlock);
	} else {
		std::cerr << "Unexpected block type '" << blockType << "' starting on line " << startLine_ << std::endl;
		exit(-1);
	}
}

void SceneReader::read(const std::string& filename) {

	std::cout << "Reading scene from " << filename << std::endl;

	std::ifstream fin(filename);

	std::string line;
	int lineNumber = 0;
	startLine_ = 0;
	std::queue<std::string> tokenBlock;
	while (std::getline(fin, line)) {
		++lineNumber;
		std::stringstream strstream(line);
		std::string token;
		while (strstream >> token) {
			if (token[0] == '#') {
				// A comment - skip the rest of the line
				break;
			} else if (toUpper(token) == "END") {
				parseTokenBlock(tokenBlock);
			} else {
				if (tokenBlock.size() == 0) {
					startLine_ = lineNumber;
				}
				tokenBlock.push(token);
			}
		}
	}

	if (tokenBlock.size() > 0) {
		std::cerr << "Unexpected end of file in " << filename << std::endl;
		exit(-1);
	}

	fin.close();

}


void SceneReader::parseSceneBlock(std::queue<std::string>& tokenBlock)  {
	while (tokenBlock.size() > 0) {
		std::string token = toUpper(tokenBlock.front());
		tokenBlock.pop();
		if (token == "AMBIENTLIGHT") {
			scene_->ambientLight = parseColour(tokenBlock);
		} else if (token == "BACKGROUNDCOLOUR") {
			scene_->backgroundColour = parseColour(tokenBlock);
		} else if (token == "RENDERSIZE") {
			scene_->renderWidth = int(parseNumber(tokenBlock));
			scene_->renderHeight = int(parseNumber(tokenBlock));
		} else if (token == "FILENAME") {
			scene_->filename = tokenBlock.front();
			tokenBlock.pop();
			std::string& fname = scene_->filename;
			std::transform(fname.begin(), fname.end(), fname.begin(), tolower);
		} else if (token == "RAYDEPTH") {
			scene_->maxRayDepth = int(parseNumber(tokenBlock));
		} else {
			std::cerr << "Unexpected token '" << token << "' in block starting on line " << startLine_ << std::endl;
			exit(-1);
		}
	}
}

double SceneReader::parseNumber(std::queue<std::string>& tokenBlock) {
	std::string token = tokenBlock.front();
	tokenBlock.pop();
	char *endPtr;
	double result = strtod(token.c_str(), &endPtr);
	if (endPtr != token.c_str()+token.length()) {
		std::cerr << "Expected a number but found '" << token << "' in block starting on line " << startLine_ << std::endl;
		exit(-1);
	} 
	return result;
}

Colour SceneReader::parseColour(std::queue<std::string>& tokenBlock) {
	Colour result;
	result.red = parseNumber(tokenBlock);
	result.green = parseNumber(tokenBlock);
	result.blue = parseNumber(tokenBlock);
	return result;
}

void SceneReader::parseCameraBlock(std::queue<std::string>& tokenBlock){
	// Make a new camera
	std::string cameraType = toUpper(tokenBlock.front());
	tokenBlock.pop();
	std::shared_ptr<Camera> camera;
	if (cameraType == "PINHOLECAMERA") {
		double focalLength = parseNumber(tokenBlock);
		camera = std::shared_ptr<PinholeCamera>(new PinholeCamera(focalLength));
		scene_->setCamera(camera);
	} else {
		std::cerr << "Unexpected camera type '" << cameraType << "' in block starting on line " << startLine_ << std::endl;
		exit(-1);
	}

	// Parse camera details
	while (tokenBlock.size() > 0) {
		std::string token = toUpper(tokenBlock.front());
		tokenBlock.pop();
		if (token == "ROTATE") {
			std::string axis = toUpper(tokenBlock.front()); 
			tokenBlock.pop();
			double angle = parseNumber(tokenBlock);
			if (axis == "X") {
				camera->transform.rotateX(angle);
			} else if (axis == "Y") {
				camera->transform.rotateY(angle);
			} else if (axis == "Z") {
				camera->transform.rotateZ(angle);
			} else {
				std::cerr << "Unexpected axis '" << axis << "' in block starting on line " << startLine_ << std::endl;
				exit(-1);
			}
		} else if (token == "TRANSLATE") {
			double tx = parseNumber(tokenBlock);
			double ty = parseNumber(tokenBlock);
			double tz = parseNumber(tokenBlock);
			camera->transform.translate(tx, ty, tz);
		} else if (token == "SCALE") {
			double s = parseNumber(tokenBlock);
			camera->transform.scale(s);
		} else if (token == "SCALE3") {
			double sx = parseNumber(tokenBlock);
			double sy = parseNumber(tokenBlock);
			double sz = parseNumber(tokenBlock);
			camera->transform.scale(sx, sy, sz);
		} else {
			std::cerr << "Unexpected token '" << token << "' in block starting on line " << startLine_ << std::endl;
			exit(-1);
		}

	}

}

void SceneReader::parseLightBlock(std::queue<std::string>& tokenBlock) {

	std::string lightType = toUpper(tokenBlock.front());
	tokenBlock.pop();
	Point location;
	Colour colour;
	Direction direction;
	double angle = 0;

	while (tokenBlock.size() > 0) {
		std::string token = toUpper(tokenBlock.front());
		tokenBlock.pop();
		if (token == "LOCATION") {
			location(0) = parseNumber(tokenBlock);
			location(1) = parseNumber(tokenBlock);
			location(2) = parseNumber(tokenBlock);
		} else if (token == "COLOUR") {
			colour = parseColour(tokenBlock);
		} else if (token == "DIRECTION") {
			direction(0) = parseNumber(tokenBlock);
			direction(1) = parseNumber(tokenBlock);
			direction(2) = parseNumber(tokenBlock);
		} else {
			std::cerr << "Unexpected token '" << token << "' in block starting on line " << startLine_ << std::endl;
			exit(-1);
		}
	}

	std::shared_ptr<LightSource> light;
	if (lightType == "POINTLIGHT") {
		light = std::shared_ptr<PointLightSource>(new PointLightSource(colour, location));
	} else if (lightType == "AMBIENTLIGHT") {
		light = std::shared_ptr<AmbientLightSource>(new AmbientLightSource(colour));
	} else if (lightType == "DIRECTIONALLIGHT" ){
		light = std::shared_ptr<DirectionalLightSource>(new DirectionalLightSource(colour, direction));
	} else {
		std::cerr << "Unexpected light type '" << lightType << "' in block starting on line " << startLine_ << std::endl;
		exit(-1);
	}
	scene_->addLight(light);


}

std::shared_ptr<Object> SceneReader::parseObjectBlock(std::queue<std::string>& tokenBlock) {
	std::string objectType = toUpper(tokenBlock.front());
	tokenBlock.pop();
	std::shared_ptr<Object> object;
	if (objectType == "SPHERE") {
		object = std::shared_ptr<Sphere>(new Sphere());
	} else if (objectType == "CUBE") {
		object = std::shared_ptr<Cube>(new Cube());
	} else if (objectType == "PLANE") {
		object = std::shared_ptr<Plane>(new Plane());
	} else if (objectType == "CYLINDER") {
		object = std::shared_ptr<Cylinder>(new Cylinder());
	} else if (objectType == "TUBE") {
		double ratio = parseNumber(tokenBlock);
		object = std::shared_ptr<Tube>(new Tube(ratio));
	} else {
		std::cerr << "Unexpected object type '" << objectType << "' in block starting on line " << startLine_ << std::endl;
		exit(-1);
	}
	scene_->addObject(object);

	// Parse object details
	while (tokenBlock.size() > 0) {
		std::string token = toUpper(tokenBlock.front());
		tokenBlock.pop();
		if (token == "ROTATE") {
			std::string axis = toUpper(tokenBlock.front()); 
			tokenBlock.pop();
			double angle = parseNumber(tokenBlock);
			if (axis == "X") {
				object->transform.rotateX(angle);
			} else if (axis == "Y") {
				object->transform.rotateY(angle);
			} else if (axis == "Z") {
				object->transform.rotateZ(angle);
			} else {
				std::cerr << "Unexpected axis '" << axis << "' in block starting on line " << startLine_ << std::endl;
				exit(-1);
			}
		} else if (token == "TRANSLATE") {
			double tx = parseNumber(tokenBlock);
			double ty = parseNumber(tokenBlock);
			double tz = parseNumber(tokenBlock);
			object->transform.translate(tx, ty, tz);
		} else if (token == "SCALE") {
			double s = parseNumber(tokenBlock);
			object->transform.scale(s);
		} else if (token == "SCALE3") {
			double sx = parseNumber(tokenBlock);
			double sy = parseNumber(tokenBlock);
			double sz = parseNumber(tokenBlock);
			object->transform.scale(sx, sy, sz);
		} else if (token == "MATERIAL") {
			std::string materialName = tokenBlock.front();
			tokenBlock.pop();
			auto material = materials_.find(materialName);
			if (material == materials_.end()) {
				std::cerr << "Undefined material '" << materialName << "' in block starting on line " << startLine_ << std::endl;
				exit(1);
			} else {
				object->material = material->second;
			}
		} else if (token == "COLOUR") {
			Colour objColour = parseColour(tokenBlock);
			object->material.ambientColour = objColour;
			object->material.diffuseColour = objColour;
		} else if (token == "AMBIENT") {
			object->material.ambientColour = parseColour(tokenBlock);
		} else if (token == "DIFFUSE") {
			object->material.diffuseColour = parseColour(tokenBlock);
		} else if (token == "SPECULAR") {
			object->material.specularColour = parseColour(tokenBlock);
			object->material.specularExponent = parseNumber(tokenBlock);
		} else if (token == "MIRROR") {
			object->material.mirrorColour = parseColour(tokenBlock);
		} else {
			std::cerr << "Unexpected token '" << token << "' in block starting on line " << startLine_ << std::endl;
			exit(-1);
		}

	}
	return object;
}

void SceneReader::parseMaterialBlock(std::queue<std::string>& tokenBlock) {
	std::string materialName = tokenBlock.front();
	tokenBlock.pop();
	if (materials_.find(materialName) == materials_.end()) {
		materials_[materialName] = Material();
	} else {
		std::cerr << "Warning: duplicate definition of material '" << materialName << "' found in block starting on line " << startLine_ << std::endl;
	}
	Material& material = materials_.find(materialName)->second;

	while (tokenBlock.size() > 0) {
		std::string token = toUpper(tokenBlock.front());
		tokenBlock.pop();
		if (token == "COLOUR") {
			Colour objColour = parseColour(tokenBlock);
			material.ambientColour = objColour;
			material.diffuseColour = objColour;
		} else if (token == "AMBIENT") {
			material.ambientColour = parseColour(tokenBlock);
		} else if (token == "DIFFUSE") {
			material.diffuseColour = parseColour(tokenBlock);
		} else if (token == "SPECULAR") {
			material.specularColour = parseColour(tokenBlock);
			material.specularExponent = parseNumber(tokenBlock);
		} else if (token == "MIRROR") {
			material.mirrorColour = parseColour(tokenBlock);
		} else {
			std::cerr << "Unexpected token '" << token << "' in block starting on line " << startLine_ << std::endl;
			exit(-1);
		}

	}
}
