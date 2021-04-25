/* $Rev: 250 $ */
#include "Cube.h"

#include "utility.h"

Cube::Cube() : Object() {

}

Cube::Cube(const Cube& cube) : Object(cube) {

}

Cube::~Cube() {

}

Cube& Cube::operator=(const Cube& cube) {
	if (this != &cube) {
		Object::operator=(cube);
	}
	return *this;
}

extern bool DEBUGPIXEL;

std::vector<RayIntersection> Cube::intersect(const Ray& ray) const {
	std::vector<RayIntersection> result;

	Ray inverseRay = transform.applyInverse(ray);

	const Point& rayStartPoint = inverseRay.point;
	const Direction& rayDirection = inverseRay.direction;


	int xAxis = 0;
	int yAxis = 1;
	int zAxis = 2;

	for (int i = -1; i <= 1; i+=2) {	
		if (true) {
			double collisionDist = ((i*1) - rayStartPoint(zAxis) ) / rayDirection(zAxis);
			if (std::abs(rayDirection(zAxis)) > epsilon && collisionDist > epsilon) {
				double x = rayStartPoint(xAxis) + collisionDist * rayDirection(xAxis);
				double y = rayStartPoint(yAxis) + collisionDist * rayDirection(yAxis);
				double z = i*1;
				
				// Check that the point hit the plane
				if ((-1 <= x && x <= 1) && (-1 <= y && y <= 1)) {
					RayIntersection hit;
					Point hitPoint = transform.apply(Point(x, y, z));

					// Transform the hit point baack to where it belongs.
					hit.point = hitPoint;
					// Get the object's material
					hit.material = material;
					
					// Normal direction is from the intersection point towards z
					Normal norm = Normal(0, 0, 0);
					norm(zAxis) = -1;
					hit.normal = transform.apply(norm); // (Any z value < 0 would also work here)
					if (hit.normal.dot(ray.direction) > 0) {
							hit.normal = -hit.normal;
					}

					// Distance 
					hit.distance = (ray.point - hit.point).norm();
					if (hit.distance >= 0) {

					// Add the hit to the result vector and return
					result.push_back(hit);	
					}
				}
			}
		}
	}


	return result;
}
