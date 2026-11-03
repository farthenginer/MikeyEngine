#include "AssetsManager.h"
#include "../Engine/Components.h"
#include "../Engine/Vector2D.h"

AssetsManager::AssetsManager(Manager* man) : manager(man)
{}

AssetsManager::~AssetsManager()
{}

void AssetsManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetsManager::GetTexture(std::string id)
{
	return textures[id];
}