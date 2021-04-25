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
	double r = 1; // Default tube radius
	double l = 1; // Default tube length

	// Apply the inverse transform to the ray so we only have to worry 
	// about a tube centered on the origin with a radius of r and length l
	Ray inverseRay = transform.applyInverse(ray);

	const Point& rayStartPoint = inverseRay.point;
	const Direction& rayDirection = inverseRay.direction;

	// Taking a 2x2 Cylinder centered on the origin and pointing
	// along the z axis.
	//
	// Ray Point:  Starting Point:  Distance * Direction:
	//    [x]	        [e]                  [dˣ]
	//	  |y|     =     |f|        +        λ|dʸ|
	//	  [z]           [g]                  [dᶻ]


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

	// Small number, anything between it and 0 is treated as 0
	double epsilon = std::numeric_limits<double>::epsilon();

	// > epsilon means 2 solutions
	double discriminant = b*b - 4*a*c;
	std::vector<double> lambdas;

	double distance1 = 0;
	double distance2 = 0;

	// Discriminant > epsilon means 2 solutions
	if (discriminant > epsilon) { // Treat epsilon as 0
		distance1 = (-b + sqrt(discriminant)) / (2*a);
		distance2 = (-b - sqrt(discriminant)) / (2*a);
	}
	// Discriminant = 0 means 1 solution
	else if (0 < discriminant && discriminant < epsilon) {
		distance1 = (-b / (2*a));
	}
	// Otherwise no solutions (Return early)

	// Now having a/some λ value, we can use them to find
	// to find the hit co-ordinates.
	//
	// x = e + λdˣ
	// y = f + λdʸ
	// z = e + λdᶻ
	if (distance1 > epsilon) {
		Point hitPoint = rayStartPoint + distance1 * rayDirection;

		// Checking z is within the length of the tube
		if (-l <= hitPoint(2) && hitPoint(2) <= l) {
			RayIntersection hit;

			// Transform the hit point baack to where it belongs.
			hit.point = transform.apply(hitPoint);
			// Get the object's material
			hit.material = material;

			
			hit.normal = transform.apply(Normal(hitPoint(0), hitPoint(1), 0));
			if (hit.normal.dot(ray.direction) > 0) hit.normal = -hit.normal;

			// Distance 
			hit.distance = distance1;

			// Add the hit to the result vector and return
			result.push_back(hit);
		}
	}

	if (distance2 > epsilon) {
		Point hitPoint = rayStartPoint + distance2 * rayDirection;

		// Checking z is within the length of the tube
		if (-l <= hitPoint(2) && hitPoint(2) <= l) {
			RayIntersection hit;

			// Transform the hit point baack to where it belongs.
			hit.point = transform.apply(hitPoint);
			// Get the object's material
			hit.material = material;

			
			hit.normal = transform.apply(Normal(hitPoint(0), hitPoint(1), 0));
			if (hit.normal.dot(ray.direction) > 0) hit.normal = -hit.normal;

			// Distance 
			hit.distance = distance2;

			// Add the hit to the result vector and return
			result.push_back(hit);
		}
	}


	
	
	return result;
}
