#pragma once

#include <glm/glm.hpp>
#include <map>
#include <GL/glew.h>
#include <string>

class Shader
{
public:
	Shader();
	~Shader();

	void bind();
	void unbind();

	void load(const char* vert, const char* frag);
	void setUniform1(const std::string& uniformName, float value);
	void setUniform1(const std::string& uniformName, int value);
	void setUniform1(const std::string& uniformName, unsigned int value);
	void setUniform2(const std::string& uniformName, const glm::vec2& values);
	void setUniform3(const std::string& uniformName, const glm::vec3& values);
	void setUniform4(const std::string& uniformName, const glm::vec4& values);
	void setUniformMatrix4(const std::string& uniformName, const glm::mat4& matrix);
	static Shader* createRegularDefault();
	static Shader* createSkyboxDefault();
	static Shader* createTerrainDefault();
	static Shader* createWaterDefault();
private:
	static GLuint currentlyBoundID;

	char* read(const char* filename);
	void setup(const char* vs, const char* fs);
	GLint lookup(const std::string& uniformName);

	std::map<const std::string, GLint> m_UniformCache;
	GLuint m_ProgramID;
};

