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
#define FEOS_VERSION_MINOR 2 //!< SDK FeOS minor version
#define FEOS_VERSION_TEXT "0.2-alpha" //!< SDK FeOS version text

#if defined(FEOS) && !defined(ARM7)

//! \brief System flags
enum
{
	SYSTEM_FLAGS_EMULATOR = BIT(0), //!< Emulator
	SYSTEM_FLAGS_CHINESE = BIT(1), //!< iQue
	SYSTEM_FLAGS_DEBUG = BIT(2), //!< Debug kernel
};

//! \brief System type
enum
{
	SYSTEM_TYPE_DS, //!< DS Phat
	SYSTEM_TYPE_DSDebug, //!< DS Phat debug unit
	SYSTEM_TYPE_DSLite, //!< DS Lite
	SYSTEM_TYPE_DSi, //!< DSi
	SYSTEM_TYPE_DSi_XL, //!< DSi XL
	SYSTEM_TYPE_3DS, //!< 3DS
	SYSTEM_TYPE_3DS_XL, //!< 3DS XL
};

//! \brief System operation mode
enum
{
	SYSTEM_MODE_DS, //!< DS Mode
	SYSTEM_MODE_DSi, //!< DSi Mode
};

//! \brief Holds information about the system
typedef struct
{
	int osVersionPack; //!< FeOS version pack
	const char* osVersionText; //!< FeOS version pack as human-readable text

	unsigned int systemFlags; //!< System flags

	int systemType; //!< System type
	const char* systemTypeText; //!< System type, as human-readable text

	int systemMode; //!< System operation mode
	const char* systemModeText; //!< System operation mode, as human-readable text
} systeminfo_t;

//! \brief Gets the currently running FeOS version
int FeOS_GetVersion();

//! \brief Gets information about the system
const systeminfo_t* FeOS_GetSystemInfo();

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
