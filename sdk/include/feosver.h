//
// FeOS Standard Library
// feosver.h
//     FeOS version
//

#pragma once

#define FEOS_VPACK_MAKE(Maj,Min) (((int)(Maj) << 8) | (int)(Min))
#define FEOS_VERSION_PACK FEOS_VPACK_MAKE(FEOS_VERSION_MAJOR, FEOS_VERSION_MINOR)

#define FEOS_VPACK_MINORVER(x) ((x) & 0xFF)
#define FEOS_VPACK_MAJORVER(x) ((x) >> 8)

// SDK FeOS version
#define FEOS_VERSION_MAJOR 0
#define FEOS_VERSION_MINOR 0
#define FEOS_VERSION_TEXT "0.0"

#if defined(FEOS) && !defined(ARM7)

// Currently running FeOS version
int FeOS_GetVersion();

static inline int FeOS_VersionAtLeast(int minorVer, int majorVer)
{
	return FeOS_GetVersion() >= FEOS_VPACK_MAKE(minorVer, majorVer);
}

static inline int FeOS_VersionUpTo(int minorVer, int majorVer)
{
	return FeOS_GetVersion() <= FEOS_VPACK_MAKE(minorVer, majorVer);
}

static inline int FeOS_VersionLaterThan(int minorVer, int majorVer)
{
	return FeOS_GetVersion() > FEOS_VPACK_MAKE(minorVer, majorVer);
}

static inline int FeOS_VersionEarlierThan(int minorVer, int majorVer)
{
	return FeOS_GetVersion() < FEOS_VPACK_MAKE(minorVer, majorVer);
}

#endif
