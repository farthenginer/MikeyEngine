#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "../Engine/Vector2D.h"
#include "../ECS/ECS.h"

class AssetsManager
{
public:

	AssetsManager(Manager* man);
	~AssetsManager();

	//texture management
	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

private:
	Manager* manager;

	std::map<std::string, SDL_Texture*> textures;
};