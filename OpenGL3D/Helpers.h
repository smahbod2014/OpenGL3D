#pragma once

#include <GL/glew.h>
#include <iostream>
#include <Windows.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Window.h"
#include "Camera.h"
#include "Constants.h"
#include "AltCamera.h"

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
	return glm::perspective<float>(glm::radians<float>(FOV), (float)Window::getWidth() / (float)Window::getHeight(), NEAR_PLANE, FAR_PLANE);
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

inline glm::mat4 createGUITransformation(const glm::vec2& position, const glm::vec2& scale)
{
	return createTransformation(position.x, position.y, 0, 0, 1, 0, 0, scale.x, scale.y, 1);
}

inline float deg2rad(float degrees)
{
	return degrees * M_PI / 180.0f;
}

inline float rad2deg(float radians)
{
	return radians * 180.0f / M_PI;
}

inline void printVec(const glm::vec3& v)
{
	std::cout << "(" << v.x << ", " << v.y << ", " << v.z << ")" << std::endl;
}

inline glm::mat4 blah(const glm::vec3& axis, float degrees)
{
	glm::mat4 mat;

	float r = deg2rad(degrees);
	float c = cosf(r);
	float s = sinf(r);
	float omc = 1.0f - c;

	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	float* p = &mat[0][0];
	p[0 + 0 * 4] = x * omc + c;
	p[1 + 0 * 4] = y * x * omc + z * s;
	p[2 + 0 * 4] = x * z * omc - y * s;
	
	p[0 + 1 * 4] = x * y * omc - z * s;
	p[1 + 1 * 4] = y * omc + c;
	p[2 + 1 * 4] = y * z * omc + x * s;
	
	p[0 + 2 * 4] = x * z * omc + y * s;
	p[1 + 2 * 4] = y * z * omc - x * s;
	p[2 + 2 * 4] = z * omc + c;

	return mat;
}


inline glm::mat4 createViewMatrix(Camera* camera)
{
	glm::mat4 view;
	view = glm::rotate(view, deg2rad(camera->getPitch()), glm::vec3(1, 0, 0));
	view = glm::rotate(view, deg2rad(camera->getYaw()), glm::vec3(0, 1, 0));
	//view = view * blah(glm::vec3(1.0f, 0.0f, 0.0f), camera->getPitch());
	//view = view * blah(glm::vec3(0.0f, 1.0f, 0.0), camera->getYaw());
	view = glm::translate(view, -camera->getPosition());
	return view;
}

inline glm::mat4 createAltViewMatrix(AltCamera* camera)
{
	glm::mat4 view;
	view = glm::rotate(view, deg2rad(camera->getPitch()), glm::vec3(1, 0, 0));
	view = glm::rotate(view, deg2rad(camera->getYaw()), glm::vec3(0, 1, 0));
	//view = view * blah(glm::vec3(1.0f, 0.0f, 0.0f), camera->getPitch());
	//view = view * blah(glm::vec3(0.0f, 1.0f, 0.0), camera->getYaw());
	view = glm::translate(view, -camera->getPosition());
	return view;
}