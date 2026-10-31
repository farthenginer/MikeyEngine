#pragma once

#include "../ECS/ECS.h"
#include "../Engine/Components.h"
#include "../Engine/Vector2D.h"
#include "../Game/Game.h"
#include "../Engine/Collision.h"
#include "../Managers/AssetsManager.h"

class BallComponent : public Component
{
public:
	BallComponent(int sp, Vector2D vel, bool isMainBall) : speed(sp), velocity(vel), isMainBall(isMainBall)
	{

	}

	~BallComponent()
	{

	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}


	void update() override
	{
		/*
		if (transform->position.x > Game::camera.x + Game::camera.w || transform->position.x < Game::camera.x
			||
			transform->position.y > Game::camera.y + Game::camera.h || transform->position.y < Game::camera.y)
		{
			entity->destroy();
		}
		*/

		if (transform->position.x < 1 && transform->velocity.x <= 0)
		{
			transform->velocity.x = 1;
			PlayHitWallSound();
		}
		if (transform->position.x > 808 && transform->velocity.x >= 0)
		{
			transform->velocity.x = -1;
			PlayHitWallSound();
		}
		if (transform->position.y < 0 && transform->velocity.y <= 0)
		{
			transform->velocity.y = 1;
			PlayHitWallSound();
		}
		if (transform->position.y > 671)
		{
			if(isMainBall) entity->getComponent<BallComponent>().CreateNewBall(true);
			entity->getComponent<BallComponent>().PlayLoseBallSound();
			entity->destroy();
		}

	}

	static void CreateNewBall(bool create);
	static void PlayHitWallSound();
	static void PlayLoseBallSound();

private:
	TransformComponent* transform;

	int speed = 0;

	bool isMainBall;

	Vector2D velocity;
	Vector2D direction;
};