#pragma once

#include <glm/glm.hpp>
#include <map>
#include <GL/glew.h>

class Shader
{
public:
	Shader(const char *vert, const char *frag);
	~Shader();

	void bind();
	void unbind();

	void setUniform1(const char* uniformName, float value);
	void setUniform1(const char* uniformName, int value);
	void setUniform3(const char* uniformName, const glm::vec3& values);
	void setUniformMatrix4(const char* uniformName, const glm::mat4& matrix);
private:
	char* read(const char* filename);
	void setup(const char* vs, const char* fs);

	std::map<const char*, GLint> m_UniformCache;
	GLuint m_ProgramID;
};

