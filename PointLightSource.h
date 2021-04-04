#pragma once

#ifndef POINT_LIGHT_SOURCE_H_INCLUDED
#define POINT_LIGHT_SOURCE_H_INCLUDED

#include "LightSource.h"

/**
 * \file 
 * \brief PointLightSource class header file.
 */

/**
 * \brief Light emitted from a Point.
 *
 * A PointLightSource represents light emitted from a point in the Scene.
 * The amount of light that reaches any given part of the scene follows
 * a \f$1/r^2\f$ law, so more distant Objects receive less illumination.
 */
class PointLightSource : public LightSource {

public:

	/** \brief PointLightSource constructor. 
	 *
	 * This creates a LightSource with a specified Colour at a given Location.
	 *
	 * \param colour The Colour of the PointLightSource
	 * \param location The location of the PointLightSource
	 **/
	PointLightSource(const Colour& colour, const Point& location);

	/** \brief PointLightSource copy constructor. 
	 *
	 * \param lightSource The PointLightSource to copy to \c this.
	 **/
	PointLightSource(const PointLightSource& lightSource);
	
	/** \brief PointLightSource destructor */
	~PointLightSource();
	
	/** \brief PointLightSource assignment operator.
	 *
	 * \param lightSource The PointLightSource to copy to \c this.
	 * \return A reference to \c this to allow for chaining of assignment.
	 **/
	PointLightSource& operator=(const PointLightSource& lightSource);

	/** \brief Determine how much light reaches a Point from this PointLightSource.
	 *
	 * The colour property determines the basic amount of light emitted by a 
	 * PointLightSource. This is the illumination received at distance 1 from 
	 * the PointLightSource, but it will appear brighter or darker if the illuminated
	 * Point is closer or further away. The amount of light received by a Point
	 * is scaled by \f$1/d^2\f$, where \f$d\f$ is the distance between the Point
	 * and the PointLightSource.
	 *
	 * \param point The Point at which light is measured.
	 * \return The illumination that reaches the Point.
	 */
	Colour getIlluminationAt(const Point& point) const;

	/**
	 * \brief Determine how far away the light source is from a given Point.
	 *
	 * \return The distance between this light source and the point.
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

	Point location_; //!< Location of this PointLightSource
};

#endif