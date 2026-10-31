#include "TrailComponent.h"
#include "../Game/Game.h"

void TrailComponent::init()
{
    transform = &entity->getComponent<TransformComponent>();
}

void TrailComponent::update()
{
    timer += Game::deltaTime;

    if (timer >= spawnInterval)
    {
        timer = 0.0f;

        points.insert(points.begin(), { transform->position.x + 6, transform->position.y, 255.0f });

        if (points.size() > maxPoints)
            points.pop_back();
    }
}

void TrailComponent::draw()
{
    SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);

    for (size_t i = 0; i < points.size(); i++)
    {
        float progress = 1.0f -
            (float)i / points.size();

        Uint8 alpha = static_cast<Uint8>(
            180.0f * progress
            );

        int size = 2 + (int)(4 * progress);

        SDL_SetRenderDrawColor(
            Game::renderer,
            255, 255, 255, alpha
        );

        SDL_Rect rect = {
            (int)points[i].x,
            (int)points[i].y,
            size,
            size
        };

        SDL_RenderFillRect(Game::renderer, &rect);
    }
}