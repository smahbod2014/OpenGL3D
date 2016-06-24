#pragma once

#include <string>
#include <map>
#include <GL/glew.h>
#include <vector>

struct TextureData
{
	unsigned long width;
	unsigned long height;
	std::vector<unsigned char> imageData;

	TextureData(const std::string& path);

	int getPixel(int x, int y);
	int getRed(int x, int y);
	int getGreen(int x, int y);
	int getBlue(int x, int y);
	int getAlpha(int x, int y);
};

class TextureManager
{
public:
	static void loadTexture(const std::string& alias, const std::string& path);
	static void loadCubeMap(const std::string& alias, const std::string& path);
	static GLuint getTexture(const std::string& alias);
private:
	static std::map<const std::string, GLuint> m_Cache;
};