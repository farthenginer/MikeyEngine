#pragma once
#include "../Engine/Components.h"
#include "SDL.h"
#include "../Managers/TextureManager.h"
#include "../Engine/Animation.h"
#include <map>
#include "../Managers/AssetsManager.h"

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;

	bool fadingOut = false;
	bool isBrick = false;

	float fadeDuration = 1.0f;
	float fadeTimer = 0.0f;
	float flashAmount = 0.0f;

	Uint8 alpha = 255;


public:

	int animIndex = 0;

	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;

	SpriteComponent(std::string id)
	{
		SetTex(id);
	}
	SpriteComponent(std::string id, bool isAnimated)
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
		SetTex(id);
	}
	~SpriteComponent()
	{
	}
	void SetTex(std::string id)
	{
		texture = Game::assets->GetTexture(id);
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
		if (fadingOut)
		{
			fadeTimer += Game::deltaTime;

			float progress = fadeTimer / fadeDuration;

			if (progress >= 1.0f)
			{
				progress = 1.0f;
				std::cout << "fade out has finished" << std::endl;
				entity->destroy();
				fadingOut = false;
			}

			alpha = static_cast<Uint8>(255.0f * (1.0f - progress));

			flashAmount = -progress;
		}

		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;

		destRect.h = transform->height * transform->scale;
		destRect.w = transform->width * transform->scale;
	}
	void draw() override
	{
		if (isBrick)
		{
			Uint8 color = static_cast<Uint8>(255.0f * flashAmount);

			SDL_SetTextureColorMod(texture, color, color, color);
			SDL_SetTextureAlphaMod(texture, alpha);
		}

		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);

		if (isBrick)
		{
			SDL_SetTextureColorMod(texture, 255, 255, 255);
			SDL_SetTextureAlphaMod(texture, 255);
		}

	}

	void Play(const char* animName)
	{
		if (Game::isPlayerHasAnimation == false)
		{
			frames = 0;
			animIndex = 0;
			speed = 1;
			return;
		}

		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}

	void FadeOut(float duration)
	{
		fadeDuration = duration;
		fadeTimer = 0.0f;
		alpha = 255;
		flashAmount = 0.0f;
		fadingOut = true;
		isBrick = true;
		std::cout << "fade out has started" << std::endl;
		entity->delGroup(Game::groupBricks);
		entity->addGroup(Game::groupOldBricks);
	}
};