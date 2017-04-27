#ifndef HELPERS_H
#define HELPERS_H

#include <GL/glew.h>

#if defined(_WIN32)
#pragma comment(lib, "glew32s.lib")
#ifndef _UNICODE
#define _UNICODE
#include <IL/il.h>
#undef _UNICODE
#else
#include <IL/il.h>
#endif
#pragma comment(lib, "DevIL.lib") // Link with DevIL library
typedef wchar_t maybewchar;
#define MAYBEWIDE(s) L##s
#else // On Linux, we need regular (not wide) strings
#include <IL/il.h>
typedef char maybewchar;
#define MAYBEWIDE(s) s
#endif

bool LoadAndSetTexture(const maybewchar *filename, GLenum target);
GLuint CreateAndLoadTexture(const maybewchar *filename);

#endif
