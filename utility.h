#pragma once

#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include <cmath>
#include <limits>
#include <algorithm>
#include <string>

/** \file
 * \brief General utility functions.
 */

const double epsilon = 1e-6; //!< Small number for checking when things are almost zero

const double infinity = std::numeric_limits<double>::max(); //!< Very large number, bigger than any sensible distance

const double pi = 3.141592653589793; //!< \f$\pi\f$, obviously. Note that M_PI is NOT part of standard C/C++, so is not portable

/** 
 * \brief Convert degrees to radians.
 * 
 * C++ mathematical functions expect angles in radians, but many people think
 * in degrees. The high level Transform functions expect parameters in degrees
 * and this function, along with deg2rad(), converts between the two.
 * 
 * \param deg An angle measured in degrees.
 * \return The angle measured in radians.
 */
inline double deg2rad(double deg) {
	return deg*pi/180;
}

/** 
 * \brief Convert radians to degrees.
 * 
 * C++ mathematical functions expect angles in radians, but many people think
 * in degrees. The high level Transform functions expect parameters in degrees
 * and this function, along with rad2deg(), converts between the two.
 * 
 * \param rad An angle measured in radians.
 * \return The angle measured in degrees.
 */
inline double rad2deg(double rad) {
	return rad*180/pi;
}


/**
 * \brief Return the sign of a number.
 *
 * This function returns +1 for positive numbers, -1 for negative numbers
 * and 0 for zero values. Since rounding can cause non-zero values, very small
 * numbers (less than \c epsilon) are returned as being 0.
 *
 * \param val The value to check the sign of.
 * \return 0, +1, or -1 depending on the sign of \c val.
 */
inline int sign(double val) {
	if (std::abs(val) < epsilon) return 0;
	if (val < 0) return -1;
	return 1;
}

/**
 * \brief Convert a string to upper case.
 * 
 * This function returns a copy of the input string with all lower case
 * characters converted to upper case.
 *
 * \param str The string to convert.
 * \return The string converted to upper case.
 **/
inline std::string toUpper(const std::string& str) {
	std::string tmp = str;
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), toupper);
	return tmp;
}

#endif // UTILITY_H_INCLUDED