
#pragma once

#ifndef DIRECTIONAL_LIGHT_SOURCE_H_INCLUDED
#define DIRECTIONAL_LIGHT_SOURCE_H_INCLUDED

#include "LightSource.h"

/**
 * \file
 * \brief DirectionalLightSource class header file.
 */

/**
 * \brief Light emitted from afar in a particular Direction.
 *
 * A DirectionaLightSource represents parallel light rays from a given Direction.
 * This is often used to represent a 'Sun' light source - a strong light source
 * at a very large (practically infinite) distance.
 */
class DirectionalLightSource : public LightSource {

public:

	/** \brief DirectionalLightSource constructor.
	 *
	 * This creates a LightSource with a specified Colour coming from a particular Direction
	 *
	 * \param colour The Colour of the DirectionalLightSource
	 * \param direction The Direction of the DirectionalLightSource
	 **/
	DirectionalLightSource(const Colour& colour, const Direction& direction);

	/** \brief DirectionalLightSource copy constructor.
	*
	* \param lightSource The DirectionalLightSource to copy to \c this.
	**/
	DirectionalLightSource(const DirectionalLightSource& lightSource);

	/** \brief DirectionalLightSource destructor */
	~DirectionalLightSource();

	/** \brief DirectionalLightSource assignment operator.
	*
	* \param lightSource The DirectionalLightSource to copy to \c this.
	* \return A reference to \c this to allow for chaining of assignment.
	**/
	DirectionalLightSource& operator=(const DirectionalLightSource& lightSource);

  /** \brief Determine how much light reaches a Point from this DirectionalLightSource.
	*
	* The illumination from a DirectionalLightSource doesn't decay with distance.
	*
	* \param point The Point at which light is measured.
	* \return The illumination that reaches the Point from this DirectionalLightSource.
	*/
	Colour getIlluminationAt(const Point& point) const;

	/**
	 * \brief Determine how far away the light source is from a given Point.
	 *
	 * Since DirectionalLightSources have no real location, they are essentially at \c infinity.
	 *
	 * \return \c infinity
	 * \sa getDirectionToLight()
	 */
	double getDistanceToLight(const Point& point) const;


	/** \brief Direction from light source to a point.
	 *
	 * Return a Direction pointing from the given point towards the light source.
	 *
	 * \param point The point where the ray is being computed.
	 * \return The Direction from the point towards the light.
	 * \sa getDistanceToLight()
	 */
	Direction getLightDirection(const Point& point) const;

private:

	Direction direction_; //!< The Direction that this light source sheds light in.

};
#endif