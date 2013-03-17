//
// FeOS Standard Library
// feos.h
//     Main header file
//

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

/** @file feos.h
 *  \brief Main FeOS API header file. Include this file in order to access the FeOS API.
 */

/*!
	\mainpage FeOS SDK Documentation

	\section intro Introduction
	Welcome to the FeOS SDK Reference Documentation.

	\section crt C/C++ Runtime Library
	- \ref crt_io "Input/Output"
	- \ref crt_mem "String/memory management"
	- \ref crt_misc "Misc utilities"
	- \ref crt_cpp "C++ facilities and runtime library"

	\section api FeOS API
	- \ref api_base "FeOS Base Routines"
	- FeOS libnds-compatible DS hardware access API
	  - \ref api_ds "Basic functions"
	  - \ref api_dsbg "Background functions"
	  - \ref api_dsspr "Sprite functions"

	\section userlibs System Libraries
	- \ref multifeos.h "MultiFeOS (cooperative multitasking)"
	- \ref feoswifi "FeOSWiFi (dswifi for FeOS)"
	- \ref feos3d.h "FeOS3D (libnds videoGL for FeOS)"
	- \ref far.h "libfar (FeOS ARchives)"
	- \ref itcm.h "libitcm (ITCM memory allocation)"
	- \ref coopgui "Cooperative GUI"
	- \ref md5.h "MD5 library"
	- \ref sndlock.h "Sound locking library"
*/

#include <feosver.h>
#include <feosbase.h>

#ifndef ARM7
#include <feoserror.h>
#include <feosuser.h>
#include <feosmath.h>
#include <feosdsapi.h>
#include <feosdsspr.h>
#include <feosdsbg.h>
#else
#include <feos7.h>
#endif

#ifdef __cplusplus
}
#endif
