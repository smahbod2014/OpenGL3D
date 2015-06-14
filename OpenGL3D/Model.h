#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

struct VertexData
{
	glm::vec3 vertex;
	glm::vec2 uv;
	glm::vec3 normal;
};

class Model
{
	friend class Renderer;
public:
	Model(float* vertices, float* texCoords, float* normals, int* indices, int numVertices, int indexCount);
	Model(const std::string& filepath);
	~Model();

	void loadTexture(const char* path);

private:
	int m_NumIndices;
	GLuint m_Vao;
	GLuint m_Vbo;
	GLuint m_Ibo;
public:
	GLuint m_TexID;
};