#pragma once

#include <GL/glew.h>

#ifdef _WIN64
	#define DEBUG_BREAK __debugbreak()
#elif __APPLE__
	#include <signal.h>
	#define DEBUG_BREAK raise(SIGTRAP)
#endif

#define ASSERT(x) \
	if (!(x))     \
		DEBUG_BREAK;

#define GLCall(x)   \
	GLClearError(); \
	x;              \
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);