#pragma once

#include <GL/glew.h>
#include <iostream>
#include <Windows.h>
#include <glm/gtx/transform.hpp>
#include <vector>
#include "Window.h"

inline void catchError(int tag)
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << tag << ": " << glewGetErrorString(error) << std::endl;
	}
}

#if 1
inline std::vector<std::string> getAllFilesInFolder(std::string folder)
{
	std::vector<std::string> names;
	folder += "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(folder.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}
#endif

inline glm::mat4 getDefaultProjectionMatrix()
{
	return glm::perspective<float>(glm::radians<float>(60.0f), (float)Window::getWidth() / (float)Window::getHeight(), 0.1f, 1000.0f);
}