//
// FeOS Standard Library
// feosver.h
//     FeOS version
//

#pragma once

/** @file feosver.h
 *  \brief FeOS version utility header
 */

/** @addtogroup api_base Core FeOS API
 *  @{
 */

//! \brief Build a FeOS version pack from a major and a minor version
#define FEOS_VPACK_MAKE(Maj,Min) (((int)(Maj) << 8) | (int)(Min))
//! \brief SDK FeOS version pack
#define FEOS_VERSION_PACK FEOS_VPACK_MAKE(FEOS_VERSION_MAJOR, FEOS_VERSION_MINOR)

//! \brief Extract the minor version from a FeOS version pack
#define FEOS_VPACK_MINORVER(x) ((x) & 0xFF)
//! \brief Extract the major version from a FeOS version pack
#define FEOS_VPACK_MAJORVER(x) ((x) >> 8)

// SDK FeOS version
#define FEOS_VERSION_MAJOR 0 //!< SDK FeOS major version
#define FEOS_VERSION_MINOR 1 //!< SDK FeOS minor version
#define FEOS_VERSION_TEXT "0.1-alpha" //!< SDK FeOS version text

#if defined(FEOS) && !defined(ARM7)

//! \brief Gets the currently running FeOS version
int FeOS_GetVersion();

//! \brief Determines whether the application is running under the specified FeOS version or later
static inline int FeOS_VersionAtLeast(int minorVer, int majorVer)
{
	return FeOS_GetVersion() >= FEOS_VPACK_MAKE(minorVer, majorVer);
}

//! \brief Determines whether the application is running up to the specified FeOS version
static inline int FeOS_VersionUpTo(int minorVer, int majorVer)
{
	return FeOS_GetVersion() <= FEOS_VPACK_MAKE(minorVer, majorVer);
}

//! \brief Determines whether the application is running under a FeOS version greater than the specified one
static inline int FeOS_VersionLaterThan(int minorVer, int majorVer)
{
	return FeOS_GetVersion() > FEOS_VPACK_MAKE(minorVer, majorVer);
}

//! \brief Determines whether the application is running under a FeOS version earlier than the specified one
static inline int FeOS_VersionEarlierThan(int minorVer, int majorVer)
{
	return FeOS_GetVersion() < FEOS_VPACK_MAKE(minorVer, majorVer);
}

#endif

/** @} */
