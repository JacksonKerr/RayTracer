#pragma once

#ifndef TUBE_H_INCLUDED
#define TUBE_H_INCLUDED

#include "Object.h"

/**
 * \file
 * \brief Tunbe class header file.
 */


 /**
  * \brief Class for Tube objects.
  *
  * This class provides an Object which is a Tune centred at the origin with outer radius 1
  * and a user-specified inner radius in the range \f$(0,1)\f$, as shown below.
  *
  * \image html tube.png
  * 
  * Note that the Tube provided in the skeleton code is not complete and returns no
  * RayIntersection%s from intersect(). This method needs to be implemented correctly
  * as part of the assignment.
  *
  */
class Tube : public Object {

public:

	/** \brief Tube default constructor.
	 *
	 * A newly constructed Octahedron is centred at the origin, and is aligned with the Z-axis
	 * It has outer radius 1, and an inner radius specified by the ratio parameter.
	 * It extends out to \f$Z= \pm 1\f$
	 * It may be moved, rotated, and scaled through its transform member.
	 * 
	 * \param ratio The ratio of the inner and outer radii of the Tube.
	 */
	Tube(double ratio = 0.5);

	/** \brief Tube copy constructor.
	 * \param tube The Tube to copy.
	 */
	Tube(const Tube& tube);

	/** \brief Tube destructor. */
	~Tube();

	/** \brief Tube assignment operator.
	 *
	 * \param tube The Tube to assign to \c this.
	 * \return A reference to \c this to allow for chaining of assignment.
	 */
	Tube& operator=(const Tube& tube);

	/** \brief Tube-Ray intersection computation.
	 *
	 * \todo The Tube-Ray intersection needs to be implemented as part of the assignment. 
	 *
	 * The intersection of a Ray with an Tube is similar to that of a Cylinder, except:
	 * - There are two curved surfaces.
	 * - The top and bottom caps are rings rather than circles.
	 *
	 * \param ray The Ray to intersect with this Tube.
	 * \return A list (std::vector) of intersections, which may be empty.
	 */
	std::vector<RayIntersection> intersect(const Ray& ray) const;

private:

	double ratio_;

};

#endif // TUBE_H_INCLUDED