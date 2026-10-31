#pragma once
#include "../Engine/Components.h"

struct Particle
{
    float x, y;
    float velocityX, velocityY;

    float size;
    float lifetime;
    float maxLifetime;

    SDL_Color color;
};