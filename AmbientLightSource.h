#pragma once

#ifndef AMBIENT_LIGHT_SOURCE_H_INCLUDED
#define AMBIENT_LIGHT_SOURCE_H_INCLUDED

#include "LightSource.h"

/**
 * \file
 * \brief AmbientLightSource class header file.
 */

/**
 * \brief Ambient light in a scene.
 *
 * An AmbientLightSource represents background illumination in a scene.
 * This is essentially a hack to avoid purely black shadows without having to 
 * model indirect lighting, which can be expensive.
 */
class AmbientLightSource : public LightSource {

public:

	/** \brief AmbientLightSource constructor.
 	 *
	 * \param colour The Colour of light emitted by the AmbientLightSource.
	 **/
	AmbientLightSource(const Colour& colour);

	/** \brief AmbientLightSource copy constructor.
	*
	* \param lightSource The AmbientLightSource to copy to \c this.
	**/
	AmbientLightSource(const AmbientLightSource& lightSource);

	/** \brief AmbientLightSource destructor */
	~AmbientLightSource();

	/** \brief AmbientLightSource assignment operator.
	*
	* \param lightSource The AmbientLightSource to copy to \c this.
	* \return A reference to \c this to allow for chaining of assignment.
	**/
	AmbientLightSource& operator=(const AmbientLightSource& lightSource);

	/** \brief Determine how much light reaches a Point from this AmbientLightSource.
	*
	* For an AmbientLightSource this is just the Colour of the light.
	*
	* \param point The Point at which light is measured.
	* \return The illumination that reaches the Point.
	*/
	Colour getIlluminationAt(const Point& point) const;

	/**
	 * \brief Distance factor for shadows from this light source.
	 *
	 * Ambient light sources cast no shadows, so this returns -1 as a flag.
	 * 
	 * \return -1
	 * \sa getDirectionToLight()
	 */
	double getDistanceToLight(const Point& point) const;


	/** \brief Direction from light source to a point.
	 *
	 * Return a Direction pointing from the given point towards the light source.
	 * We can return any value for AmbientLightSource, since it will never be used.
	 *
	 * \param point The point where the ray is being computed.
	 * \return The Direction from the point towards the light.
	 * \sa getDistanceToLight()
	 */
	Direction getLightDirection(const Point& point) const;

};

#endif // AMBIENT_LIGHT_SOURCE_H_INCLUDED