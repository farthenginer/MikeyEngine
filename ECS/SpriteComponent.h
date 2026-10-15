#pragma once
#include "../Engine/Components.h"
#include "SDL.h"
#include "../Managers/TextureManager.h"
#include "../Engine/Animation.h"
#include <map>

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;

public:

	int animIndex = 0;

	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;

	SpriteComponent(const char* path)
	{
		SetTex(path);
	}
	SpriteComponent(const char* path, bool isAnimated)
	{
		animated = isAnimated;

		Animation bottomIdle = Animation(0, 4, 155);
		Animation sideIdle = Animation(1, 4, 155);
		Animation topIdle = Animation(2, 4, 155);

		Animation bottomRun = Animation(3, 4, 155);
		Animation sideRun = Animation(4, 4, 155);
		Animation topRun = Animation(5, 4, 155);

	    animations.emplace("bottomIdle", bottomIdle);
	    animations.emplace("sideIdle", sideIdle);
	    animations.emplace("topIdle", topIdle);
	    animations.emplace("bottomRun", bottomRun);
	    animations.emplace("sideRun", sideRun);
	    animations.emplace("topRun", topRun);

		Play("bottomIdle");
		SetTex(path);
	}
	~SpriteComponent()
	{
		SDL_DestroyTexture(texture);
	}
	void SetTex(const char* path)
	{
		texture = TextureManager::LoadTexture(path);
	}

	void init() override
	{

		transform = &entity->getComponent<TransformComponent>();


		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update()override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x);
		destRect.y = static_cast<int>(transform->position.y);

		destRect.h = transform->height * transform->scale;
		destRect.w = transform->width * transform->scale;
	}
	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void Play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};