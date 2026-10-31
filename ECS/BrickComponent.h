#pragma once

#include "../ECS/ECS.h"
#include "../Engine/Components.h"
#include "../Engine/Vector2D.h"
#include "../Game/Game.h"
#include "../Engine/Collision.h"

class BrickComponent : public Component
{

public:
	BrickComponent(const char* color)
	{

	}

	~BrickComponent()
	{

	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{

	}
	bool isBonus;

private:
	TransformComponent* transform;

};