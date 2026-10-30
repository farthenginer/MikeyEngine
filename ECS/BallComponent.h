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
	BallComponent(int sp, Vector2D vel) : speed(sp), velocity(vel)
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

		if (transform->position.x < 1)
		{
			transform->velocity.x = 1;
		}
		if (transform->position.x > 808)
		{
			transform->velocity.x = -1;
		}
		if (transform->position.y > 671)
		{
			entity->getComponent<BallComponent>().CreateNewBall();
			entity->destroy();
		}
		if (transform->position.y < 0)
		{
			transform->velocity.y = 1;
		}
	}

	static void CreateNewBall();

private:
	TransformComponent* transform;

	int speed = 0;

	Vector2D velocity;
	Vector2D direction;
};