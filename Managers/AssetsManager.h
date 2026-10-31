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

	//game objects
	void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id);
	void CreateBall(Vector2D pos, Vector2D vel, int speed, std::string id, bool isMainBall);
	void CreateBrick(Vector2D pos ,const char* id, bool isBonus);
	void CreateBackground(Vector2D pos, const char* id);

	//texture management
	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

private:
	Manager* manager;

	std::map<std::string, SDL_Texture*> textures;
};