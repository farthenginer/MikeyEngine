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

	int speed = 5;

	TransformComponent()
	{
		position.zero();
	}
	TransformComponent(int sc)
	{
		position.x = 400;
		position.y = 320;
		scale = sc;
	}

	TransformComponent(float x, float y)
	{
		position.zero();
	}	
	TransformComponent(float x, float y, int w, int h, int sc)
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

	}
};