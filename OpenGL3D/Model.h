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
	Model();
	~Model();

	void loadTexture(const char* path);
	void generate(const std::string& filepath);
	void generate(float size);
	void generate(float* vertices, float* texCoords, float* normals, unsigned int* indices, int numVertices, int indexCount);
	void bindTexture(int i);

	void setTextureID(GLuint texID) { m_TexID = texID; }
private:
	int m_NumIndices;
	GLuint m_Vao;
	GLuint m_Vbo;
	GLuint m_Ibo;
	GLuint m_TexID;
};