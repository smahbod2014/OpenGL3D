#include "Model.h"
#include "PicoPNG.h"
#include "FileReader.h"
#include "OBJLoader.h"
#include "VBOIndexer.h"
#include <iostream>

Model::Model(float* vertices, float* texCoords, float* normals, int* indices, int numVertices, int indexCount)
{
	m_NumIndices = indexCount;
	
	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	glGenBuffers(1, &m_Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);

	VertexData* vertexData = new VertexData[numVertices];
	for (int i = 0; i < numVertices; i++)
	{
		memcpy(&vertexData[i].vertex, &vertices[i * 3], 12);
		memcpy(&vertexData[i].uv, &texCoords[i * 2], 8);
		memcpy(&vertexData[i].normal, &normals[i * 3], 12);
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * numVertices, vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)offsetof(VertexData, vertex));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)offsetof(VertexData, uv));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(VertexData), (const GLvoid*)offsetof(VertexData, normal));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glGenBuffers(1, &m_Ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * m_NumIndices, indices, GL_STATIC_DRAW);

	delete[] vertexData;
}

Model::Model(const std::string& filepath)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool success = loadOBJ(filepath.c_str(), vertices, uvs, normals);
	assert(success);

	std::vector<unsigned int> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);


#if 0
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_GenNormals);
	assert(scene);

	const aiMesh* mesh = scene->mMeshes[0];

	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		VertexData vd;
		vd.vertex = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vd.uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		vd.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertices.push_back(vd);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}
#endif
	m_NumIndices = indices.size();
	
	/*std::cout << indexed_vertices.size() << std::endl;
	std::cout << indexed_uvs.size() << std::endl;
	std::cout << indexed_normals.size() << std::endl;
	std::cout << indices.size() << std::endl;
	std::cout << sizeof(glm::vec3) << " " << sizeof(glm::vec2) << std::endl;*/

	VertexData* vd = new VertexData[indexed_vertices.size()];
	for (size_t i = 0; i < indexed_vertices.size(); i++)
	{
		vd[i].vertex = indexed_vertices[i];
		vd[i].uv = indexed_uvs[i];
		vd[i].normal = indexed_normals[i];
	}

	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	glGenBuffers(1, &m_Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * indexed_vertices.size(), vd, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)offsetof(VertexData, vertex));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)offsetof(VertexData, uv));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const GLvoid*)offsetof(VertexData, normal));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glGenBuffers(1, &m_Ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_NumIndices, indices.data(), GL_STATIC_DRAW);

	delete[] vd;
}

Model::~Model()
{
	if (m_Vao)
		glDeleteVertexArrays(1, &m_Vao);
	if (m_Vbo)
		glDeleteBuffers(1, &m_Vbo);
	if (m_Ibo)
		glDeleteBuffers(1, &m_Ibo);
	if (m_TexID)
		glDeleteTextures(1, &m_TexID);
}

void Model::loadTexture(const char* path)
{
	glGenTextures(1, &m_TexID);
	glBindTexture(GL_TEXTURE_2D, m_TexID);

	long fileLength;
	unsigned long width, height;
	std::vector<unsigned char> imageData;
	unsigned char* fileData = FileReader::read(path, fileLength);
	decodePNG(imageData, width, height, fileData, fileLength);
	delete fileData;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}