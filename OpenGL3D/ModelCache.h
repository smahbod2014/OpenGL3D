#pragma once

#include <string>
#include <map>

class Model;

class ModelCache
{
public:
	static void loadModel(const std::string& alias, const std::string& path);
	static void loadModel(const std::string& alias, float size);
	static Model* getModel(const std::string& alias);
private:
	static std::map<const std::string, Model*> m_Cache;
};