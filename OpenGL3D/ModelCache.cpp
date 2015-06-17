#include "ModelCache.h"
#include "OBJLoader.h"
#include "Model.h"
#include <iostream>

std::map<const std::string, Model*> ModelCache::m_Cache;

void ModelCache::loadModel(const std::string& alias, const std::string& path)
{
	if (m_Cache.find(alias) != m_Cache.end())
	{
		std::cout << "Cache already contains " << path << std::endl;
		return;
	}

	//delete this at some point
	Model* model = new Model();
	model->generate(path);
	m_Cache[alias] = model;
}

Model* ModelCache::getModel(const std::string& alias)
{
	if (m_Cache.find(alias) == m_Cache.end())
		return nullptr;
	
	return m_Cache[alias];
}