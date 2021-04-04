#pragma once

#ifndef LIGHT_SOURCE_H_INCLUDED
#define LIGTH_SOURCE_H_INCLUDED

#include "Colour.h"
#include "Ray.h"

/**
 * \file 
 * \brief LightSource class header file.
 */

/**
 * \brief Abstract base class for LightSources.
 *
 * LightSources provide illumination to the scene. All light sources have a Colour, 
 * but other properties may vary. 
 *
 * As an abstract base class, you cannot create a LightSource directly.
 * Instead one of its concrete subclasses must be created.
 */
class LightSource {

public:

	/** \brief Determine how much light reaches a Point from this LightSource.
	 *
	 * The amount of light emitted by a LightSource is determined by its colour property,
	 * but this generally varies across the Scene. This method returns a scaled value to
	 * apply to the colour of the light at a given Point in the Scene.
	 *
	 * \param point The Point at which light is measured.
	 * \return The illumination that reaches the Point.
	 */
	virtual Colour getIlluminationAt(const Point& point) const = 0;


	/** \brief Determine how far away the light source is from a given Point.
	 *
	 * This is a bit tricky, to get around the fact that there are several general types of light. 
	 * There are basically three types we are interested in:
	 * - AmbientLightSource, which does not have a location, and which does not create shadows.
	 * - PointLightSource and SpotLightSource, which have a fixed location, so cast shadows when 
	 *   an object is between the point of interest and the light source's location.
	 * - DirectionalLightSource, which does not have a specific location, and casts shadows if there's
	 *   anything blocking that direction to the point of interest.
	 *
	 * For a PointLightSource or SpotLightSource the direction is well defined.
	 * For a DirectionalLightSource we can return infinity.
	 * For an AmbientLightSource we can return -1. This is never a valid distance, 
	 * so can be used to indicate that we can skip the lighting computations for this source.
	 *
	 * \return The distance to the light source (with special values as above).
	 * \sa getDirectionToLight()
	 */
	virtual double getDistanceToLight(const Point& point) const = 0;

	/** \brief Direction from light source to a point.
	 *
	 * Return a Direction pointing from the given point towards the light source.
	 * For most light sources this is well-defined, but for AmbientLightSources it is not.
	 * This Direction is used for lighting and shadow calculations, however, so this is not a problem.
	 * We can return any value for AmbientLightSource, since it will never be used.
	 *
	 * \param point The point where the ray is being computed.
	 * \return The Direction from the point towards the light.
	 * \sa getDistanceToLight()
	 */	
	virtual Direction getLightDirection(const Point& point) const = 0;


protected:

	/** \brief LightSource constructor.
	 *
	 * \param colour The Colour of this LightSource's illumination.
	 */
	LightSource(const Colour& colour);
	
	/** \brief LightSource copy constructor. 
	 *
	 * \param lightSource The LightSource to copy.
	 */
	LightSource(const LightSource& lightSource);
	
	/** \brief LightSource destructor. */
	virtual ~LightSource();
	
	/** \brief LightSource assignment operator. 
	 *
	 * \param lightSource The LightSource to assign to \c this.
	 * \return A reference to \c this to allow for chaining of assignment.
	 */
	LightSource& operator=(const LightSource& lightSource);

	Colour colour_;  //!< The Colour of this LightSource's illumination.
};

#endif // LIGHT_SOURCE_H_INCLUDED