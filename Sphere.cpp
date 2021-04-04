#include "Sphere.h"

#include "utility.h"

Sphere::Sphere() : Object() {

}

Sphere::Sphere(const Sphere& sphere) : Object(sphere) {

}

Sphere::~Sphere() {

}

Sphere& Sphere::operator=(const Sphere& sphere) {
	if (this != &sphere) {
		Object::operator=(sphere);
	}
	return *this;
}

std::vector<RayIntersection> Sphere::intersect(const Ray& ray) const {

	std::vector<RayIntersection> result;

	Ray inverseRay = transform.applyInverse(ray);

	// Intersection is of the form ad^2 + bd + c, where d = distance along the ray

	const Point& p = inverseRay.point;
	const Direction& d = inverseRay.direction;
	double a = d.dot(d);
	double b = 2 * d.dot(p);
	double c = p.dot(p) - 1;

	RayIntersection hit;
	hit.material = material;

	double b2_4ac = b*b - 4*a*c;
	double t;
	switch (sign(b2_4ac)) {
	case -1:
		// No intersections
		break;
	case 0:
		// One intersection
		t = -b/(2*a);
		if (t > 0) {
			// Intersection is in front of the ray's start point
			hit.point = transform.apply(Point(p + t*d));
			hit.normal = transform.apply(Normal(p + t*d));
			if (hit.normal.dot(ray.direction) > 0) {
				hit.normal = -hit.normal;
			}
			hit.distance = (hit.point - ray.point).norm();
			result.push_back(hit);
		}
		break;
	case 1:
		// Two intersections
		t = (-b + sqrt(b*b - 4*a*c))/(2*a);
		if (t > 0) {
			// Intersection is in front of the ray's start point
			hit.point = transform.apply(Point(p + t * d));
			hit.normal = transform.apply(Normal(p + t*d));
			if (hit.normal.dot(ray.direction) > 0) {
				hit.normal = -hit.normal;
			}
			hit.distance = (hit.point - ray.point).norm();
			result.push_back(hit);
		}

		t = (-b - sqrt(b*b - 4*a*c))/(2*a);
		if (t > 0) {
			// Intersection is in front of the ray's start point
			hit.point = transform.apply(Point(p + t*d));
			hit.normal = transform.apply(Normal(p + t*d));
			if (hit.normal.dot(ray.direction) > 0) {
				hit.normal = -hit.normal;
			}
			hit.distance = (hit.point - ray.point).norm();
			result.push_back(hit);
		}
		break;
	default:
		// Shouldn't be possible, but just in case
		std::cerr << "Something's wrong - sign(x) should be -1, +1 or 0" << std::endl;
		exit(-1);
		break;
	}

	return result;
}