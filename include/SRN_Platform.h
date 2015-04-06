#pragma once

#ifndef SRN_PLATFORM_H
#define SRN_PLATFORM_H

#ifdef _WIN32
	#define SRN_OS_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#elif defined(__linux__)
	#define SRN_OS_LINUX
#elif defined(__APPLE__)
	#define SRN_OS_MAC
#endif

//#ifndef SRN_STATIC_BUILD
	#ifdef SRN_OS_WINDOWS
		#ifdef SRN_NONCLIENT_BUILD
			#define SRN_API __declspec(dllexport)
		#else
			#define SRN_API __declspec(dllimport)
		#endif
	#elif defined(SRN_OS_LINUX) || defined(SRN_OS_MAC)
		#ifdef __GNUC__ >= 4
			#define SRN_API __attribute__ ((__visibility__("default")))
		#else
			#define SRN_API SRN_API
		#endif
	#endif
#else
	#define SRN_API SRN_API
//#endif

#endif // SRN_PLATFORM_H