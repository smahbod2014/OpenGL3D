#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct VertexData
{
	glm::vec3 vertex;
	glm::vec2 uv;
	glm::vec3 normal;
};

class Model
{
	friend class Renderer;
	friend class SkyboxRenderer;
	friend class TerrainRenderer;
	friend class WaterRenderer;
	friend class GUIRenderer;
	friend class ShadowMapEntityRenderer;
public:
	Model();
	~Model();

	void loadTexture(const char* path);
	void loadPlane(float size);
	void loadArbitrary(float* verts, float* texCoords, float* normals, unsigned int* ind, int numVertices, int indexCount);
	void generate(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices);
	void generateVerticesOnly(float* vertices, int numVertices);
	void generate2dVerticesOnly(float* vertices, int numVertices);
	void loadFromFile(const std::string& filepath);
	void bindTexture(int i);

	void setTextureID(GLuint texID) { m_TexID = texID; }
private:
	int m_NumIndices;
	GLuint m_Vao;
	GLuint m_Vbo;
	GLuint m_Ibo;
	GLuint m_TexID;
};