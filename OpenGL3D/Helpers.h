#pragma once

#include <GL/glew.h>
#include <iostream>
#include <Windows.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

inline float barryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

inline glm::mat4 createTransformation(float x, float y, float z, float ax, float ay, float az, float angle, float sx, float sy, float sz)
{
	glm::mat4 T;
	T = glm::translate(T, glm::vec3(x, y, z));

	glm::mat4 R;
	R = glm::rotate(R, angle, glm::vec3(ax, ay, az));

	glm::mat4 S;
	S = glm::scale(S, glm::vec3(sx, sy, sz));

	return T * R * S;
}