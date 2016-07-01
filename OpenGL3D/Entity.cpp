#include "Entity.h"
#include <glm/gtx/transform.hpp>
#include "Model.h"
#include "ModelCache.h"
#include "TextureManager.h"

Entity::Entity(const std::string& modelAlias, const std::string& textureAlias)
{
	model = ModelCache::getModel(modelAlias);
	texID = TextureManager::getTexture(textureAlias);
}

Entity::~Entity()
{
}

void Entity::translateLocal(float x, float y, float z)
{
	transformation = transformation * glm::translate(glm::vec3(x, y, z));
}

void Entity::translate(float x, float y, float z)
{
	transformation = glm::translate(glm::vec3(x, y, z)) * transformation;
}

void Entity::rotate(float axisx, float axisy, float axisz, float degrees)
{
	transformation = transformation * glm::rotate(glm::radians(degrees), glm::vec3(axisx, axisy, axisz));
}

void Entity::scale(float x, float y, float z)
{
	transformation = transformation * glm::scale(glm::vec3(x, y, z));
}

void Entity::setNormalMap(const std::string& normalMapAlias)
{
	normalMapID = TextureManager::getTexture(normalMapAlias);
}

void Entity::setSpecularMap(const std::string& specularMapAlias)
{
	specularMapID = TextureManager::getTexture(specularMapAlias);
}