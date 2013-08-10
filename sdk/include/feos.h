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
	Welcome to the official FeOS SDK Documentation. Here you will find information
	(hopefully) on how to develop FeOS software.

	\section contents Contents
	- \ref templates
	- \ref guidelines
	- \ref buildsys
	- \ref directmode
	- \ref coopguidev
	- \ref debug
	- \ref arm7
	- \ref deploying

	\section reference Reference

	\subsection crt C/C++ Runtime Library
	- \ref crt_io "Input/Output"
	- \ref crt_mem "String/memory management"
	- \ref crt_misc "Misc utilities"
	- \ref crt_cpp "C++ facilities and runtime library"

	\subsection api FeOS API
	- \ref api_base "Core FeOS API"
	- \ref api_thr "FeOS Threading API"
	- **FeOS libnds-compatible DS hardware access API**
	  - \ref api_ds "Basic functions"
	  - \ref api_math "Math functions"
	  - \ref api_dsbg "Background functions"
	  - \ref api_dsspr "Sprite functions"

	\subsection userlibs System Libraries
	- \ref feoswifi "FeOSWifi (dswifi for FeOS)"
	- \ref feos3d.h "FeOS3D (libnds videoGL for FeOS)"
	- \ref far.h "libfar (FeOS ARchives)"
	- \ref itcm.h "libitcm (ITCM memory allocation)"
	- \ref coopgui "Cooperative GUI"
	- \ref md5.h "MD5 library"
	- \ref sndlock.h "Sound locking library"
*/

#include <feosdef.h>
#include <feosver.h>

#ifndef ARM7
#include <feoserror.h>
#include <feosuser.h>
#include <feosmath.h>
#include <feosdsapi.h>
#include <feosthr.h>
#include <feosdsspr.h>
#include <feosdsbg.h>
#else
#include <feos7.h>
#endif

#ifdef FEOS_ALLOW_OLD_NAMES
#include <feosoldnames.h>
#endif

#ifdef __cplusplus
}
#endif
