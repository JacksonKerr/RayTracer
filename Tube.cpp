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
	double r = 0.5; // Default tube radius
	double l = 0.5; // Default tube length

	// Apply the inverse transform to the ray so we only have to worry 
	// about a tube centered on the origin with a radius of r and length l
	Ray inverseRay = transform.applyInverse(ray);

	// Taking a tube of radius 2 and length 2 facing along the x axis
	//
	// Ray Point:  Starting Point:  Distance * Direction:
	//    [x]	        [e]                  [dˣ]
	//	  |y|     =     |f|        +        λ|dʸ|
	//	  [z]           [g]                  [dᶻ]

	const Point& rayStartPoint = inverseRay.point;
	const Direction& rayDirection = inverseRay.direction;

	// The ray is intersecting the tube when:
	// y² + z² = r
	// 
	// So subbing in y = f + λdʸ and z = g + λd
	// 		We get (f + λdʸ)² + (g + λdᶻ)² = r
	// 		Which we can expand to: 
	//      (dʸ² + dᶻ²)λ² + (2dʸf + 2dᶻg) + f² + g² + r = 0
	// 
	//	Using the quadratic equation, we can find λ.
	//  (There will be <=2 solutions)
	//  Our a, b and c values are:
	//		a = dʸ² + dᶻ²
	// 		b = 2dʸf + 2dᶻg
	// 		c = f² + g² + r²

	double a = (rayDirection(1)*rayDirection(1)) + (rayDirection(2)*rayDirection(2));
	double b = (2 * rayDirection(1)*rayStartPoint(1)) + (2 * rayDirection(2) * rayStartPoint(2));
	double c = (rayStartPoint(1)*rayStartPoint(1)) + (rayStartPoint(2)*rayStartPoint(2)) - r;

	double discriminant = b*b - 4*a*c;
	std::vector<double> lambdas;
	double epsilon = 0.000001;
	if (epsilon < discriminant) {
		lambdas.push_back( ( -b +sqrt(discriminant) ) / (2*a) );
		lambdas.push_back( ( -b -sqrt(discriminant) ) / (2*a) );
	}
	if (0 < discriminant && discriminant < epsilon) {
		lambdas.push_back(-b / (2*a));
	}
	// Now having some λ values, we can use them to find
	// to find co-ordinates
	//
	// x = e + λdˣ
	// y = f + λdʸ
	// z = e + λdᶻ

	for (double lambda : lambdas) {
		double x = rayStartPoint(0) + lambda * rayDirection(0);
		double y = rayStartPoint(1) + lambda * rayDirection(1);
		double z = rayStartPoint(2) + lambda * rayDirection(2);

		if (-l <= x && x <= l) {
			RayIntersection hit;
			Point hitPoint = transform.apply(Point(x, y, z));

			// Transform the hit point baack to where it belongs.
			hit.point = hitPoint;
			// Get the object's material
			hit.material = material;

			
			// Normal direction is from the intersection point towards z
			hit.normal = transform.apply(Normal(0, 0, 1)); // (Any z value > 0 would also work here)

			// Distance 
			hit.distance = lambda;

			// Add the hit to the result vector and return
			result.push_back(hit);
		}
	}
	return result;
}