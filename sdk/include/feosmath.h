//
// FeOS Standard Library
// feosmath.h
//     FeOS math functions
//

#pragma once

#define DEGREES_IN_CIRCLE (1 << 15)

#define degreesToAngle(degrees) ((degrees) * DEGREES_IN_CIRCLE / 360)
#define angleToDegrees(angle)   ((angle) * 360 / DEGREES_IN_CIRCLE)

#define inttof32(n)   ((n) << 12)                     // convert int to f32
#define f32toint(n)   ((n) >> 12)                     // convert f32 to int
#define floattof32(n) ((int)((n) * (1 << 12)))        // convert float to f32
#define f32tofloat(n) (((float)(n)) / (float)(1<<12)) // convert f32 to float

short sinLerp(short);
short cosLerp(short);
int tanLerp(short);
short asinLerp(short);
short acosLerp(short);

int FeOS_div3232(int, int);
int FeOS_mod3232(int, int);
int FeOS_div6432(long long, int);
int FeOS_mod6432(long long, int);
long long FeOS_div6464(long long, long long);
long long FeOS_mod6464(long long, long long);
int FeOS_sqrt32(int);
long long FeOS_sqrt64(long long);

// libnds compatibility functions

static inline int divf32(int a, int b)
{
	return FeOS_div6432((long long)a << 12, b);
}

static inline int mulf32(int a, int b)
{
	long long r = (long long)a * (long long)b;
	return (int)(r >> 12);
}

static inline int sqrtf32(int a)
{
	return FeOS_sqrt64((long long)a << 12);
}

#define div32 FeOS_div3232
#define mod32 FeOS_mod3232
#define div64 FeOS_div6432
#define mod64 FeOS_mod6432
#define sqrt32 FeOS_sqrt32
#define sqrt64 FeOS_sqrt64

static inline void crossf32(int* a, int* b, int* result)
{
	result[0] = mulf32(a[1], b[2]) - mulf32(b[1], a[2]);
	result[1] = mulf32(a[2], b[0]) - mulf32(b[2], a[0]);
	result[2] = mulf32(a[0], b[1]) - mulf32(b[0], a[1]);
}

static inline int dotf32(int* a, int* b)
{
	return mulf32(a[0], b[0]) + mulf32(a[1], b[1]) + mulf32(a[2], b[2]);
}

static inline void normalizef32(int* a)
{
	// magnitude = sqrt (Ax^2 + Ay^2 + Az^2)
	int magnitude = sqrtf32(mulf32(a[0], a[0]) + mulf32(a[1], a[1]) + mulf32(a[2], a[2]));

	a[0] = divf32(a[0], magnitude);
	a[1] = divf32(a[1], magnitude);
	a[2] = divf32(a[2], magnitude);
}
