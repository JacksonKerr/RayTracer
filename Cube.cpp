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

std::vector<RayIntersection> Cube::intersect(const Ray& ray) const {

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

	// The ray is intersecting the cube when:
	// 	x, y, or z is 1
	//	The other two dimensions are >= -1 and <= 1
	//
	//
	// Taking x = 1
	//		We know x = g + λdˣ from the point equation.
	//		Subbing in x = 1 and solving for λ, we get:
	//                      λ = (1-g)/dˣ
	//
	// We can repeat the same thing for y = 0 and z = 0
	
	std::vector<int> axis;
	axis.push_back(0);
	axis.push_back(1);
	axis.push_back(2);

	std::rotate(axis.begin(),
            axis.end()-1, // this will be the new first element
            axis.end());


	std::vector<double> collisionDists;
	collisionDists.push_back((1-rayStartPoint(0)) / rayDirection(0));
	collisionDists.push_back((-1-rayStartPoint(0)) / rayDirection(0));

	// Now having some λ values, we can use them to find
	// to find the other co-ordinates
	//
	// x = e + λdˣ
	// y = f + λdʸ
	// z = e + λdᶻ

	// Starting with λ for x = 1
	for (double lambda : collisionDists) {
		double x = 1;
		double y = rayStartPoint(1) + lambda * rayDirection(1);
		double z = rayStartPoint(2) + lambda * rayDirection(2);
		
		// Check that the point hit the plane
		if ((-1 <= y && y <= 1) && (-1 <= z && z <= 1)) {
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
