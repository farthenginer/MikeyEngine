#pragma once
#include "../Engine/Components.h"
#include "../Engine/Vector2D.h"
class TransformComponent : public Component
{

public:

	Vector2D position;
	Vector2D velocity;

	int height = 32;
	int width = 32;
	int scale = 1;

	int speed = 12;

	TransformComponent()
	{
		position.zero();
	}
	TransformComponent(float sc)
	{
		position.x = 400;
		position.y = 320;
		scale = sc;
	}

	TransformComponent(float x, float y)
	{
		position.zero();
	}

	TransformComponent(float x, float y, int w, int h, float sc, int sp)
	{
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		scale = sc;
		speed = sp;
	}

	TransformComponent(float x, float y, int w, int h, float sc)
	{
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		scale = sc;
	}	

	void init() override
	{
		velocity.zero();
	}

	void update() override
	{
		if (position.x < 1)
		{
			velocity.x = 1;
		}
		if (position.x > 735)
		{
			velocity.x = -1;
		}

		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};