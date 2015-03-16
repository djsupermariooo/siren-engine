#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define SRN_OS_WINDOWS

#include <Windows.h>
#include <string>
#include <sstream>

#endif

#ifdef __GNUCC__
#define SRN_OS_LINUX
#endif

#ifdef __APPLE__
#define SRN_OS_MAC
#endif

// OPENGL INCLUDES
#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glext.h>
#include <gl/wglext.h>


bool OutErrorMsg(const char* message);