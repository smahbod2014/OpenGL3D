#pragma once

#include <GL/glew.h>
#include <iostream>

inline void catchError(int tag)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << tag << ": " << glewGetErrorString(error) << std::endl;
	}
}