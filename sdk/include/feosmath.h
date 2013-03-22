//
// FeOS Standard Library
// feosmath.h
//     FeOS math functions
//

#pragma once

/** @file feosmath.h
 *  \brief FeOS math functions
 */

/** @addtogroup api_math FeOS math functions
 *  \brief FeOS math functions
 *  \code
 *  #include <feos.h>
 *  \endcode
 *  @{
 */

#define DEGREES_IN_CIRCLE (1 << 15) //!< Number of DS angle units in a circle

#define degreesToAngle(degrees) ((degrees) * DEGREES_IN_CIRCLE / 360) //!< Converts sexagesimal degrees to DS angle units
#define angleToDegrees(angle)   ((angle) * 360 / DEGREES_IN_CIRCLE) //!< Converts DS angle units to sexagesimal degrees

#define inttof32(n)   ((n) << 12)                     //!< Converts an integer to 12-bit fixed point
#define f32toint(n)   ((n) >> 12)                     //!< Converts a 12-bit fixed point number to a normal integer
#define floattof32(n) ((int)((n) * (1 << 12)))        //!< Converts a float to 12-bit fixed point (SHOULD ONLY BE USED FOR CONSTANTS)
#define f32tofloat(n) (((float)(n)) / (float)(1<<12)) //!< Converts a 12-bit fixed point number to float (SHOULD NOT BE USED)

short sinLerp(short); //!< Sine function. Argument is in DS angle units. \returns 12-bit fixed point result.
short cosLerp(short); //!< Cosine function. Argument is in DS angle units. \returns 12-bit fixed point result.
int tanLerp(short); //!< Tangent function. Argument is in DS angle units. \returns 12-bit fixed point result.
short asinLerp(short); //!< Arcsine function. Argument is in 12-bit fixed point. \returns Angle in DS angle units.
short acosLerp(short); //!< Arccosine function. Argument is in 12-bit fixed point. \returns Angle in DS angle units.

int FeOS_div3232(int, int); //!< Hardware-accelerated 32/32 integer division.
int FeOS_mod3232(int, int); //!< Hardware-accelerated 32/32 integer modulo.
int FeOS_div6432(long long, int); //!< Hardware-accelerated 64/32 integer division.
int FeOS_mod6432(long long, int); //!< Hardware-accelerated 64/32 integer modulo.
long long FeOS_div6464(long long, long long); //!< Hardware-accelerated 64/64 integer division.
long long FeOS_mod6464(long long, long long); //!< Hardware-accelerated 64/64 integer modulo.
int FeOS_sqrt32(int);  //!< Hardware-accelerated 32-bit square root.
long long FeOS_sqrt64(long long); //!< \brief Hardware-accelerated 64-bit square root.

// libnds compatibility functions

//! \brief 12-bit fixed point division.
static inline int divf32(int a, int b)
{
	return FeOS_div6432((long long)a << 12, b);
}

//! \brief 12-bit fixed point multiplication.
static inline int mulf32(int a, int b)
{
	long long r = (long long)a * (long long)b;
	return (int)(r >> 12);
}

//! \brief 12-bit fixed point square root.
static inline int sqrtf32(int a)
{
	return FeOS_sqrt64((long long)a << 12);
}

#define div32 FeOS_div3232 //!< libnds compatibility alias.
#define mod32 FeOS_mod3232 //!< libnds compatibility alias.
#define div64 FeOS_div6432 //!< libnds compatibility alias.
#define mod64 FeOS_mod6432 //!< libnds compatibility alias.
#define sqrt32 FeOS_sqrt32 //!< libnds compatibility alias.
#define sqrt64 FeOS_sqrt64 //!< libnds compatibility alias.

//! \brief 3D 12-bit fixed point vectorial product.
static inline void crossf32(int* a, int* b, int* result)
{
	result[0] = mulf32(a[1], b[2]) - mulf32(b[1], a[2]);
	result[1] = mulf32(a[2], b[0]) - mulf32(b[2], a[0]);
	result[2] = mulf32(a[0], b[1]) - mulf32(b[0], a[1]);
}

//! \brief 3D 12-bit fixed point scalar product.
static inline int dotf32(int* a, int* b)
{
	return mulf32(a[0], b[0]) + mulf32(a[1], b[1]) + mulf32(a[2], b[2]);
}

//! \brief 3D 12-bit fixed point vector normalization.
static inline void normalizef32(int* a)
{
	// magnitude = sqrt (Ax^2 + Ay^2 + Az^2)
	int magnitude = sqrtf32(mulf32(a[0], a[0]) + mulf32(a[1], a[1]) + mulf32(a[2], a[2]));

	a[0] = divf32(a[0], magnitude);
	a[1] = divf32(a[1], magnitude);
	a[2] = divf32(a[2], magnitude);
}

/** @} */
