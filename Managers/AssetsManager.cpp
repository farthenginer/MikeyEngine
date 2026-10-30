#include "AssetsManager.h"
#include "../Engine/Components.h"
#include "../Engine/Vector2D.h"

AssetsManager::AssetsManager(Manager* man) : manager(man)
{}

AssetsManager::~AssetsManager()
{}

void AssetsManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(id, false);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectiles);
}

void AssetsManager::CreateBall(Vector2D pos, Vector2D vel, int speed, std::string id)
{
	auto& ball(manager->addEntity());
	ball.addComponent<TransformComponent>(pos.x, pos.y, 8, 8, 3, speed);
	ball.addComponent<SpriteComponent>(id, false);
	ball.addComponent<BallComponent>(speed, vel);
	ball.addComponent<ColliderComponent>("ball");
	ball.addGroup(Game::groupBalls);
}

void AssetsManager::CreateBrick(Vector2D pos, const char* id)
{
	auto& brick(manager->addEntity());
	brick.addComponent<TransformComponent>(pos.x, pos.y, 32, 8, 2, 0);
	brick.addComponent<SpriteComponent>(id, false);
	brick.addComponent<BrickComponent>(id);
	brick.addComponent<ColliderComponent>("brick");
	brick.addGroup(Game::groupBricks);
}

void AssetsManager::CreateBackground(Vector2D pos, const char* id)
{
	auto& bg(manager->addEntity());
	bg.addComponent<TransformComponent>(pos.x, pos.y, 210, 168, 4, 0);
	bg.addComponent<SpriteComponent>(id, false);
	bg.addGroup(Game::groupBackgrounds);
}

void AssetsManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}


SDL_Texture* AssetsManager::GetTexture(std::string id)
{
	return textures[id];
}