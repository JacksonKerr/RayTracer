#include "Cylinder.h"

#include "utility.h"

Cylinder::Cylinder() : Object() {

}

Cylinder::Cylinder(const Cylinder& cylinder) : Object(cylinder) {

}

Cylinder::~Cylinder() {

}

Cylinder& Cylinder::operator=(const Cylinder& cylinder) {
	if (this != &cylinder) {
		Object::operator=(cylinder);
	}
	return *this;
}

std::vector<RayIntersection> Cylinder::intersect(const Ray& ray) const {

	std::vector<RayIntersection> result;
	double r = 1; // Tube radius
	double l = 2; // Tube length

	// Apply the inverse transform to the ray so we only have to worry 
	// about a tube centered on the origin with a radius of r and length l
	Ray inverseRay = transform.applyInverse(ray);

	const Point& rayStartPoint = inverseRay.point;
	const Direction& rayDirection = inverseRay.direction;

	// Taking a Cylinder centered on the origin and pointing
	// along the z axis.
	//
	// Ray Point:  Starting Point:  Distance * Direction:
	//    [x]	        [e]                  [dˣ]
	//	  |y|     =     |f|        +        λ|dʸ|
	//	  [z]           [g]                  [dᶻ]

	// ===================================================== //
	//			  Intersecing with Rounded Edge				 //
	// ===================================================== //
	// The ray is intersecting the tube when:
	// y² + x² = r
	// 
	// So subbing in y = f + λdʸ and x = e + λdˣ
	// 		We get (f + λdʸ)² + (e + λdˣ)² = r
	// 		Which we can expand to: 
	//      (dʸ² + dˣ²)λ² + (2dʸf + 2dˣe) + f² + e² - r = 0
	// 
	//	Using the quadratic equation, we can find λ.
	//  (There will be <=2 solutions)
	//  Our a, b and c values are:
	//		a = dʸ² + dˣ²
	// 		b = 2dʸf + 2dˣe
	// 		c = f² + e² + r
	//
	double a = (rayDirection(1)*rayDirection(1)) + (rayDirection(0)*rayDirection(0));
	double b = (2 * rayDirection(1)*rayStartPoint(1)) + (2 * rayDirection(0) * rayStartPoint(0));
	double c = (rayStartPoint(1)*rayStartPoint(1)) + (rayStartPoint(0)*rayStartPoint(0)) - r;

	double discriminant = b*b - 4*a*c;

	std::vector<double> hitDistances;

	// Discriminant > epsilon means 2 solutions to quadratic equation
	if (discriminant > epsilon) {
		double sqrtDiscriminnt = sqrt(discriminant); // So we only take one sqrt (for efficiency)
		hitDistances.push_back( (-b + sqrtDiscriminnt )  / (2*a));
		hitDistances.push_back( (-b - sqrtDiscriminnt )  / (2*a));
	}
	// Discriminant = 0 means 1 solution
	else if (0 < discriminant && discriminant < epsilon) {
		hitDistances.push_back(-b / (2*a));
	}

	// Now, using our λ value(s), we can find the hit co-ordinates.
	// x = e + λdˣ
	// y = f + λdʸ    ie.   hitPoint = startPoint + (distance * direction)
	// z = e + λdᶻ
	for (double hitDistance : hitDistances) {
		if (hitDistance > epsilon) {
			Point hitPoint = rayStartPoint + hitDistance * rayDirection;

			// Checking z is within the length of the tube
			if (-l/2 <= hitPoint(2) && hitPoint(2) <= l/2) {
				RayIntersection hit;

				hit.point = transform.apply(hitPoint); // Transform the hit point back to it's transformed co-ords
				hit.material = material;

				hit.normal = transform.apply(Normal(hitPoint(0), hitPoint(1), 0));
				if (hit.normal.dot(ray.direction) > 0) hit.normal = -hit.normal;

				hit.distance = hit.distance = (ray.point - hit.point).norm();;

				result.push_back(hit);
			}
		}
	}

	// ===================================================== //
	//			   Intersecing with End Caps				 //
	// ===================================================== //
	
	// See cube.cpp

	for (int i = -1; i <= 1; i += 2) {
		double collisionDist = ((i*(l/2))-rayStartPoint(2)) / rayDirection(2);
		if (std::abs(rayDirection(2)) > epsilon && collisionDist > 0) {
			double x = rayStartPoint(0) + collisionDist * rayDirection(0);
			double y = rayStartPoint(1) + collisionDist * rayDirection(1);
			double z = i*(l/2);

			if (pow(x, 2) + pow(y, 2) <= r) { // Equation for a circle
				RayIntersection hit;

				hit.point = transform.apply(Point(x, y, z));
				// Get the object's material
				hit.material = material;
				
				// Normal direction is from the intersection point towards z
				hit.normal = transform.apply(Normal(0, 0, 1));
				if (hit.normal.dot(ray.direction) > 0) hit.normal = -hit.normal;

				hit.distance = (ray.point - hit.point).norm();

				if (hit.distance > epsilon) result.push_back(hit);
			}
		}
	}

	return result;
}
