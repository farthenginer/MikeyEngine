
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
        //const Uint8* mKeys = SDL_GetMouseState(nullptr);
        bool up = keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP];
        bool down = keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN];
        bool left = keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT];
        bool right = keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT];
        //other keys
        
        if (Game::event.type == SDL_KEYDOWN)
        {
            switch (Game::event.key.keysym.sym)
            {
            case SDLK_p:
                Game::isDebugging = !Game::isDebugging;//debugging flip flop
                break;

            case SDLK_ESCAPE:
                Game::isRunning = false;
                break;
            }
        }
    }
};
