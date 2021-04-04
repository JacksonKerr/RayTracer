#pragma once

#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include "Object.h"

/**
* \file
* \brief Plane class header file.
*/


/**
 * \brief Class for Plane objects.
 *
 * This class provides an Object which is a plane centred at the origin, 
 * extending \f$\pm 1\f$ units along the \f$X\f$ and \f$Y\f$ axes, as shown below:
 *
 * \image html plane.png
 *
 * Note that the Cube provided in the skeleton code is not complete and returns no
 * RayIntersection%s from intersect(). This method needs to be implemented correctly
 * as part of the labs/assignment.
 *
 */
class Plane : public Object {

public:

	/** \brief Plane default constructor.
	*
	* A newly constructed Plane is centred at the origin, and extends \f$\pm 1\f$ units along the \f$X\f$ and \f$Y\f$ axes.
	* It may be moved, rotated, and scaled through its transform member.
	*/
	Plane();

	/** \brief Plane copy constructor.
	* \param plane The Plane to copy.
	*/
	Plane(const Plane& plane);

	/** \brief Plane destructor. */
	~Plane();

	/** \brief Plane assignment operator.
	*
	* \param plane The Plane to assign to \c this.
	* \return A reference to \c this to allow for chaining of assignment.
	*/
	Plane& operator=(const Plane& plane);

	/** \brief Plane-Ray intersection computation.
	*
	* \todo The Plane-Ray intersection needs to be implemented as part of the assignment. 
	*
	* The intersection of a Ray with a Plane comes down to determining when the
	* \f$Z\f$ component of the Ray is zero. If that lies within the bounds of the 
	* plane there is an intersection.
	*
	* \param ray The Ray to intersect with this Sphere.
	* \return A list (std::vector) of intersections, which may be empty.
	*/
	std::vector<RayIntersection> intersect(const Ray& ray) const;

};

#endif // PLANE_H_INCLUDED