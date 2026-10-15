
#pragma once

#include "Game.h"
#include "../ECS/ECS.h"
#include "../Engine/Components.h"

class KeyboardController : public Component
{
public:
    TransformComponent* transform;
    SpriteComponent* sprite;

    std::string lastDirection = "bottom";

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }

    void update() override
    {
        const Uint8* keys = SDL_GetKeyboardState(nullptr);

        bool up = keys[SDL_SCANCODE_W];
        bool down = keys[SDL_SCANCODE_S];
        bool left = keys[SDL_SCANCODE_A];
        bool right = keys[SDL_SCANCODE_D];

        bool escape = keys[SDL_SCANCODE_ESCAPE]; //escape for exit

        transform->velocity.x = 0;
        transform->velocity.y = 0;

        if (up)    transform->velocity.y = -1;
        if (down)  transform->velocity.y = 1;
        if (left)  transform->velocity.x = -1;
        if (right) transform->velocity.x = 1;

        if (up)
        {
            lastDirection = "top";

            sprite->Play("topRun");
        }

        else if (down)
        {
            lastDirection = "bottom";

            sprite->Play("bottomRun");
        }

        else if (left)
        {
            lastDirection = "side";

            sprite->spriteFlip = SDL_FLIP_NONE;
            sprite->Play("sideRun");
        }

        else if (right)
        {
            lastDirection = "side";

            sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
            sprite->Play("sideRun");
        }

        else
        {
            if (lastDirection == "top")
            {
                sprite->Play("topIdle");
            }

            else if (lastDirection == "side")
            {
                sprite->Play("sideIdle");
            }

            else
            {
                sprite->Play("bottomIdle");
            }
        }

        if (escape)
        {
            Game::isRunning = false;
        }
    }
};
