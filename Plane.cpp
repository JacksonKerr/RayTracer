#include "Plane.h"

#include "utility.h"

Plane::Plane() : Object() {

}

Plane::Plane(const Plane& plane) : Object(plane) {

}

Plane::~Plane() {

}

Plane& Plane::operator=(const Plane& plane) {
	if (this != &plane) {
		Object::operator=(plane);
	}
	return *this;
}

std::vector<RayIntersection> Plane::intersect(const Ray& ray) const {

	std::vector<RayIntersection> result;

	// Apply the inverse transform to the ray so we only have to worry 
	// about a plane at the origin with a 2x2 size, lying on the x and y
	// axis.
	Ray inverseRay = transform.applyInverse(ray);

	// Taking a 2x2 plane centered on the origin aligned with 
	// the x and y axis.
	//
	// Ray Point:  Starting Point:  Distance * Direction:
	//    [x]	        [e]                  [dˣ]
	//	  |y|     =     |f|        +        λ|dʸ|
	//	  [z]           [g]                  [dᶻ]

	const Point& rayStartPoint = inverseRay.point;
	const Direction& rayDirection = inverseRay.direction;

	// The ray is intersecting the plane when:
	// 	• -1 <= x <= 1
	//	• -1 <= y <= 1
	// 	• z = 0
	//
	// Starting with checking for z = 0
	//		We know z = g + λdᶻ from the point equation.
	//		Subbing in z = 0 and solving for λ, we get:
	//                      λ = -g/dᶻ

	const double collisionDist = (-rayStartPoint(2)) / rayDirection(2);

	// Now knowing λ, we can use the ray equation 
	// to find x, and y.
	//
	// x = e + λdˣ
	// y = f + λdʸ

	double x = rayStartPoint(0) + collisionDist * rayDirection(0);
	double y = rayStartPoint(1) + collisionDist * rayDirection(1);
	double z = 0;
	
	// Check that the point hit the plane
	if ((-1 <= x && x <= 1) && (-1 <= y && y <= 1)) {
		RayIntersection hit;
		Point hitPoint = transform.apply(Point(x, y, z));

		// Transform the hit point baack to where it belongs.
		hit.point = hitPoint;
		// Get the object's material
		hit.material = material;
		
		// Normal direction is from the intersection point towards z
		hit.normal = transform.apply(Normal(0, 0, 1)); // (Any z value > 0 would also work here)

		// Distance 
		hit.distance = collisionDist;

		// Add the hit to the result vector and return
		result.push_back(hit);	
	}
	return result;
}