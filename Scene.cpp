#include "Scene.h"

#include "Colour.h"
#include "ImageDisplay.h"
#include "utility.h"

#include <float.h>

// For demos

Scene::Scene() : backgroundColour(0,0,0), ambientLight(0,0,0), maxRayDepth(3), renderWidth(800), renderHeight(600), filename("render.png"), camera_(), objects_(), lights_() {

}

Scene::~Scene() {

}


bool DEBUGPIXEL = false; // DEBUG
void Scene::render() const {
	ImageDisplay display("Render", renderWidth, renderHeight);

	const double w = double(renderWidth);
	const double h = double(renderHeight);

	for (unsigned int v = 0; v < renderHeight; ++v) {
		for (unsigned int u = 0; u < renderWidth; ++u) {

			if (v == 14 && u == 9) DEBUGPIXEL = true; // DEBUG

			double cu = -1 + (u + 0.5)*(2.0 / w);
			double cv = -h/w + (v + 0.5)*(2.0 / w);
			Ray ray = camera_->castRay(cu, cv);
			display.set(u, v, computeColour(ray, maxRayDepth));

			DEBUGPIXEL = false;
		}
		display.refresh();
	}

	display.save(filename);
	display.pause(5);
}

RayIntersection Scene::intersect(const Ray& ray) const {
	RayIntersection firstHit;
	firstHit.distance = infinity;
	std::vector<RayIntersection> hits;
	
	for (const auto& obj: objects_) {
		for (const auto& hit: obj->intersect(ray)) {
			if (hit.distance > epsilon && hit.distance < firstHit.distance) {
				firstHit = hit;
			}
		}
	}
	return firstHit;
}

Colour Scene::computeColour(const Ray& ray, unsigned int rayDepth) const {
	const RayIntersection hitPoint = intersect(ray);
	if (hitPoint.distance == infinity) {
		return backgroundColour;
	}
	Colour hitColour(0, 0, 0);

	for (const auto & light : lights_) {
		// Compute the influence of this light on the appearance of the hit object.
		if (light->getDistanceToLight(hitPoint.point) < 0) {
			// === Ambient Lighting ===
			hitColour += light->getIlluminationAt(hitPoint.point) * hitPoint.material.ambientColour;
		} else {
			// === Other lighting: === 

			// Lambda for converting vectors to unit-vectors
			auto toUnitVector = [](const Vector vec) {
				Vector resultVector = Vector(vec);
				double vecLen = vec.norm();
				if (vecLen != 1) {
					resultVector(0) = vec(0)/vecLen;
					resultVector(1) = vec(1)/vecLen;
					resultVector(2) = vec(2)/vecLen;
				}
				return resultVector;
			};

			// DIFFUSE:

			const Vector hitUnitNormal = toUnitVector(hitPoint.normal);
			const Vector unitLightDir = toUnitVector(light->getLightDirection(hitPoint.point));
			const Colour luxAtHitPoint = light->getIlluminationAt(hitPoint.point);
			const Colour objectDiffuse = hitPoint.material.diffuseColour;

			// Check if point is in shadow before computing light:
			// Ray going from hitPoint towards light
			Ray shadowRay;
			shadowRay.point = hitPoint.point;
			shadowRay.direction = -light->getLightDirection(shadowRay.point);
			const double distToLight = light->getDistanceToLight(shadowRay.point);
			const RayIntersection shadowRayHit = this->intersect(shadowRay);


			auto printPoint = [](const Vector vec, std::string varName) {
				std::cout << varName << ": (" << vec(0) << ", " << vec(1) << ", " << vec(2) << ")" << std::endl;
			};

			if (DEBUGPIXEL) {
				printPoint(shadowRay.point, "shadowRay.point");
				printPoint(shadowRay.direction, "shadowRay.direction");
				std::cout << "\ndistToLight: " << distToLight << std::endl;
				std::cout << "shadowRayHit.distance: " << shadowRayHit.distance << std::endl;
				printPoint(shadowRayHit.point, "shadowRayHit.point");
				std::cout << "ShadowRayHit.material: (" << shadowRayHit.material.ambientColour.red << ", "
														<< shadowRayHit.material.ambientColour.green << ", "
														<< shadowRayHit.material.ambientColour.blue << ")" << std::endl << std::endl;
				std::cout << "distToLight < shadowRayHit.distance = ";
			}

			if (distToLight < shadowRayHit.distance) {
				double nDOTr = hitUnitNormal.dot(-unitLightDir);
				
				if (DEBUGPIXEL) {
					std::cout << "True" << std::endl;
					std::cout << "\tn . r = " << nDOTr << std::endl << std::endl;
				}
				
				if (nDOTr >= 0) {
					hitColour += luxAtHitPoint * objectDiffuse * nDOTr; 
				}
			} else if (DEBUGPIXEL)std::cout << "False" << std::endl << std::endl;
		}		
	}

	// Compute mirror reflections - only if surface hit is a mirror and we've not reached our rayDepth
	if (rayDepth > 0 && (hitPoint.material.mirrorColour.red > 0 ||
		                 hitPoint.material.mirrorColour.green > 0 ||
		                 hitPoint.material.mirrorColour.blue > 0)) {

		// Compute the reflected ray
		Ray reflectedRay;

		// Surface normal as a unit vector
		Normal n = hitPoint.normal;
		n = n / n.norm(); 

		// View direction as a unit vector
		Direction v = -ray.direction;
		v = v / v.norm(); 

		// Ray starts at the hit point
		reflectedRay.point = hitPoint.point;

		// And goes in the reflection of v about n
		reflectedRay.direction = 2 * (v.dot(n)) * n - v;

		// Hit colour is a mix of the current surface and reflected ray
		hitColour = (Colour(1, 1, 1) - hitPoint.material.mirrorColour) * hitColour 
					+ (hitPoint.material.mirrorColour * computeColour(reflectedRay, rayDepth - 1));
	}

	hitColour.clip();
	if (DEBUGPIXEL) std::cout << "Final Pixel Colour: (" << hitColour.red << ", " 
										<< hitColour.green << ", "
										<< hitColour.blue << ")" << std::endl; // DEBUG
	return hitColour;
}

bool Scene::hasCamera() const {
	return bool(camera_);
}
