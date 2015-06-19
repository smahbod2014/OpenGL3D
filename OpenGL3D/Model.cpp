#include "Model.h"
#include "PicoPNG.h"
#include "FileReader.h"
#include "OBJLoader.h"
#include "VBOIndexer.h"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <ctime>

Model::Model()
{

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
	delete[] fileData;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Model::loadArbitrary(float* verts, float* texCoords, float* normals, unsigned int* ind, int numVertices, int indexCount)
{
	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i < numVertices; i++)
	{
		VertexData vd;
		vd.vertex = glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]);
		vd.uv = glm::vec2(texCoords[i * 2], texCoords[i * 2 + 1]);
		vd.normal = glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
		vertices.push_back(vd);
	}

	for (int i = 0; i < indexCount; i++)
		indices.push_back(ind[i]);

	generate(vertices, indices);
}

void Model::loadFromFile(const std::string& filepath)
{
	Assimp::Importer importer;

	clock_t begin = clock();
	
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate
											 | aiProcess_GenSmoothNormals
											 | aiProcess_FlipUVs);

	if (!scene)
	{
		std::cout << "ERROR: Couldn't load model: " << filepath << std::endl;
		assert(0);
	}

	const aiMesh* model = scene->mMeshes[0];

	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;

	const aiVector3D aiZeroVector(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < model->mNumVertices; i++)
	{
		const aiVector3D& pos = model->mVertices[i];
		const aiVector3D& normal = model->mNormals[i];
		const aiVector3D& texCoord = model->HasTextureCoords(0) ? model->mTextureCoords[0][i] : aiZeroVector;

		vertices.push_back(VertexData());

		VertexData& vdRef = vertices.back();

		memcpy(&vdRef.vertex, &pos, 12);
		memcpy(&vdRef.uv, &texCoord, 8);
		memcpy(&vdRef.normal, &normal, 12);
	}
	
	for (unsigned int i = 0; i < model->mNumFaces; i++)
	{
		const aiFace& face = model->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	double elapsed = double(clock() - begin) / CLOCKS_PER_SEC;
	std::cout << "Done loading " << filepath << ". Took " << elapsed << " seconds." << std::endl;

	generate(vertices, indices);
}

void Model::loadPlane(float size)
{
	float half = size / 2.0f;
	float verts[] =
	{
		-half, 0, -half,
		-half, 0, half,
		half, 0, half,
		half, 0, -half
	};

	float texCoords[] =
	{
		0, 0, 0, 1, 1, 1, 1, 0
	};

	float normals[] =
	{
		0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0
	};

	unsigned int ind[] =
	{
		0, 1, 2, 2, 3, 0
	};

	std::vector<VertexData> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i < 4; i++)
	{
		VertexData vd;
		vd.vertex = glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]);
		vd.uv = glm::vec2(texCoords[i * 2], texCoords[i * 2 + 1]);
		vd.normal = glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
		vertices.push_back(vd);
	}

	for (int i = 0; i < 6; i++)
		indices.push_back(ind[i]);

	generate(vertices, indices);
}

void Model::generate(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices)
{
	m_NumIndices = indices.size();

	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	glGenBuffers(1, &m_Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

void Model::bindTexture(int i)
{
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, m_TexID);
}