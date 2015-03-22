#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define SRN_OS_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif

#elif defined(__linux__)
	#define SRN_OS_LINUX
#elif defined(__APPLE__)
	#define SRN_OS_MAC

#endif