#pragma once
#pragma once

#include <vector>
#include <SDL.h>
#include "Particle.h"

class ParticleComponent
{
private:
    std::vector<Particle> particles;

public:
    void Emit(float x, float y, SDL_Color color);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);
};