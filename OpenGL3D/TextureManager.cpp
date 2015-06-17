#include "TextureManager.h"
#include "FileReader.h"
#include "PicoPNG.h"
#include <iostream>

std::map<const std::string, GLuint> TextureManager::m_Cache;

void TextureManager::loadTexture(const std::string& alias, const std::string& path)
{
	if (m_Cache.find(alias) != m_Cache.end())
	{
		std::cout << "Texture already in cache: " << path << std::endl;
		return;
	}

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	long fileLength;
	unsigned long width, height;
	std::vector<unsigned char> imageData;
	unsigned char* fileData = FileReader::read(path.c_str(), fileLength);
	decodePNG(imageData, width, height, fileData, fileLength);
	delete[] fileData;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	m_Cache[alias] = texID;
}

GLuint TextureManager::getTexture(const std::string& alias)
{
	if (m_Cache.find(alias) == m_Cache.end())
		return 0;

	return m_Cache[alias];
}