#pragma once

#ifndef CYLINDER_H_INCLUDED
#define CYLINDER_H_INCLUDED

#include "Object.h"

/** 
 * \file
 * \brief Cylinder class header file.
 */


/**
 * \brief Class for Cylinder objects.
 * 
 * This class provides an Object which is a Cyclinder centred at the origin, with radius 1.
 * The clyinder is aligned with the \f$Z\f$-axis, and extends to \f$ \pm 1\f$ along that axis, as 
 * illustrated below.
 *
 * \image html cylinder.png
 * 
 * Note that the Cylinder provided in the skeleton code is not complete, and returns no 
 * RayIntersection%s from intersect(). This method needs to be implemented correctly
 * as part of the assignment.
 *
 */
class Cylinder : public Object {

public:

    /** \brief Cylinder default constructor.
	 * 
	 * A newly constructed Cylinder is centred at the origin, and has raidius 1. 
	 * Its axis is aligned with the Z-axis, and it extends out to \f$Z = \pm 1\f$.
	 * It may be moved, rotated, and scaled through its transform member.
	 */
	Cylinder();

	/** \brief Cylinder copy constructor.      
	 * \param cylinder The Cylinder to copy.
	 */
	Cylinder(const Cylinder& cylinder);

	/** \brief Cylinder destructor. */
	~Cylinder();

	/** \brief Cylinder assignment operator.
	 *
	 * \param cylinder The Cylinder to assign to \c this.
	 * \return A reference to \c this to allow for chaining of assignment.
	 */
	Cylinder& operator=(const Cylinder& cylinder);

	/** \brief Cylinder-Ray intersection computation.
	 *
	 * \todo The Cylinder-Ray intersection needs to be implemented as part of the assignment. 
	 *
	 * The intersection of a Cylinder and a Ray can be divided into three main parts:
	 * - Intersections with the curved surface of the cylinder
	 * - Intesections with the top cap (flat surface) of the cylinder
	 * - Intesections with the bottom cap of the cylinder
	 *
	 * Intersections with the curved surface are like the intersection with a Sphere, but
	 * the \f$Z\f$-axis can be ignored. You might think of this as intersecting a 2D ray in the
	 * \f$X\f$-\f$Y\f$ plane with a unit circle. This leads to a quadratic equation, like that 
	 * for the Sphere and 0, 1, or 2 hit points. These hit points would be with an 'infinite' 
	 * cylinder extending along the \f$Z\f$-axis, so you should reject those with \f$Z\f$ value
	 * greater than 1 or less than \f$-1\f$.
	 *
	 * The top cap can be seen as intersecting the Ray with the Plane \f$Z=1\f$, so finding
	 * how far you need to go along the Ray to get a \f$Z\f$ value of 1. Note that for
	 * some Rays this may never happen. Once you've found where the Ray intersects the plane
	 * \f$Z=1\f$, check that the intersection is within 1 unit of the \f$Z\f$-axis.
	 *
	 * The bottom cap is the same, except you are checking against the plane \f$Z=-1\f$.
	 *
	 * \param ray The Ray to intersect with this Cylinder.
	 * \return A list (std::vector) of intersections, which may be empty.
	 */
	std::vector<RayIntersection> intersect(const Ray& ray) const;

private:

};

#endif // CYLINDER_H_INCLUDED
