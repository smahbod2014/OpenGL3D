#pragma once

#include <string>
#include <map>
#include <GL/glew.h>

class TextureManager
{
public:
	static void loadTexture(const std::string& alias, const std::string& path);
	static GLuint getTexture(const std::string& alias);
private:
	static std::map<const std::string, GLuint> m_Cache;
};