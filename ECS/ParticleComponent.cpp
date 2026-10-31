#include "ParticleComponent.h"
#include <cstdlib>

void ParticleComponent::Emit(float x, float y, SDL_Color color)
{
    for (int i = 0; i < 25; i++)
    {
        Particle p;

        p.x = x;
        p.y = y;

        p.velocityX = (rand() % 201 - 100) * 2.0f;
        p.velocityY = (rand() % 201 - 100) * 2.0f;

        p.size = 3.0f;

        p.lifetime = 0.4f;
        p.maxLifetime = 0.4f;

        p.color = color;

        particles.push_back(p);
    }
}

void ParticleComponent::Update(float deltaTime)
{
    for (auto& p : particles)
    {
        p.x += p.velocityX * deltaTime;
        p.y += p.velocityY * deltaTime;

        p.lifetime -= deltaTime;
    }

    particles.erase( std::remove_if(particles.begin(), particles.end(), 
    [](const Particle& p)
    {
        return p.lifetime <= 0;
    }),
    particles.end());
}

void ParticleComponent::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (const auto& p : particles)
    {
        float progress = p.lifetime / p.maxLifetime;

        Uint8 alpha = static_cast<Uint8>(255 * progress);

        SDL_SetRenderDrawColor(
            renderer,
            p.color.r,
            p.color.g,
            p.color.b,
            alpha
        );

        SDL_FRect rect = {
            p.x,
            p.y,
            p.size,
            p.size
        };

        SDL_RenderFillRectF(renderer, &rect);
    }
}