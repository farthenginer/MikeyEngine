#pragma once
struct Animation
{
	int index; //animation index at sprite sheet
	int frames; //max row count
	int speed; //animation frame speed

	Animation(){}
	Animation(int i, int f, int s)
	{
		index = i;
		frames = f;
		speed = s;
	}
};