#include "TextureManager.h"
#include "FileReader.h"
#include "PicoPNG.h"
#include <iostream>

std::map<const std::string, GLuint> TextureManager::m_Cache;

TextureData::TextureData(const std::string& path)
{
	long fileLength;
	unsigned char* fileData = FileReader::read(path.c_str(), fileLength);
	decodePNG(imageData, width, height, fileData, fileLength);
	delete[] fileData;
}

int TextureData::getPixel(int x, int y)
{
	if (x < 0 || x >= width || y < 0 || y >= height) {
		std::cout << "x or y out of bounds" << std::endl;
	}

	int i = (y * width + x) * 4;
	return (int)imageData[i] << 24 | (int)imageData[i + 1] << 16 | (int)imageData[i + 2] << 8 | (int)imageData[i + 3];
}

int TextureData::getRed(int x, int y)
{
	return getPixel(x, y) >> 24 & 0xff;
}

int TextureData::getGreen(int x, int y)
{
	return getPixel(x, y) >> 16 & 0xff;
}

int TextureData::getBlue(int x, int y)
{
	return getPixel(x, y) >> 8 & 0xff;
}

int TextureData::getAlpha(int x, int y)
{
	return getPixel(x, y) & 0xff;
}

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_Cache[alias] = texID;
}

void TextureManager::loadCubeMap(const std::string& alias, const std::string& path)
{
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

	std::string filenames[] = { "right", "left", "top", "bottom", "back", "front" };
	for (int i = 0; i < 6; i++) {
		TextureData td(path + "/" + filenames[i] + ".png");
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, td.width, 
					 td.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, td.imageData.data());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	m_Cache[alias] = texID;
}

GLuint TextureManager::getTexture(const std::string& alias)
{
	if (m_Cache.find(alias) == m_Cache.end())
		return 0;

	return m_Cache[alias];
}