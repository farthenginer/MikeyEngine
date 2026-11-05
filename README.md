# Mikey Engine

A small, lightweight 2D game engine written in **C++** on top of **SDL2**, built around an **Entity Component System (ECS)**. It gives you a window, a game loop, sprite rendering, collisions, tile maps, audio, text, and a particle system, so you can focus on writing your own game logic.

### Demo Games

***[Breakout2D (Made by Mikey Engine)](https://mikey-manji.itch.io/breakout-made-by-mikey-engine)***

## Features

- **Entity Component System**: build objects by attaching components (`TransformComponent`, `SpriteComponent`, `ColliderComponent`, ...) to entities.
- **Entity groups**: tag entities with a group (map, players, colliders, ...) and loop over just the ones you need.
- **Sprite rendering** with optional sprite-sheet animation, flipping, and fade-out effects.
- **AABB collision detection** through `ColliderComponent` and `Collision::AABB`.
- **Tile map loader** that reads a simple text-based `.map` file (tiles + collision layer).
- **Asset manager** for loading and fetching textures by string ID.
- **Audio** via SDL_mixer (`Mix_Chunk` sound effects).
- **Text rendering** via SDL_ttf.
- **Particle system and trail effects** (`ParticleComponent`, `TrailComponent`).
- **Fixed 60 FPS loop** with a per-frame `Game::deltaTime`.
- **Debug mode**: press `P` to toggle collider drawing.

## Dependencies

- A C++17-capable compiler
- [SDL2](https://github.com/libsdl-org/SDL)
- [SDL2_image](https://github.com/libsdl-org/SDL_image)
- [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
- [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer)

Make sure the SDL2 include and library paths are set up in your IDE or build system, and that the SDL `.dll` / `.so` files are next to your executable.

## Project Structure

The source files include each other using these folder names, so keep this layout in your project:

```
.
├── main.cpp              # Entry point: window size, title, game loop
├── Game/                 # <-- YOUR GAME LIVES HERE
│   ├── Game.h / Game.cpp
│   ├── KeyboardController.h
│   └── Map.h / Map.cpp
├── ECS/                  # Entity Component System + built-in components
│   ├── ECS.h / ECS.cpp
│   ├── TransformComponent.h
│   ├── SpriteComponent.h
│   ├── ColliderComponent.h
│   ├── TileComponent.h
│   ├── ParticleComponent.h / .cpp
│   └── TrailComponent.h / .cpp
├── Engine/               # Core helpers
│   ├── Components.h      # Includes every component in one place
│   ├── Collision.h / .cpp
│   ├── Vector2D.h / .cpp
│   ├── Animation.h
│   └── Particle.h
├── Managers/
│   ├── AssetsManager.h / .cpp
│   └── TextureManager.h / .cpp
├── assets/               # Textures, sounds, maps
└── terminal.ttf          # Font used for on-screen text
```

> **Note:** the engine loads assets with relative paths (e.g. `assets/...`, `terminal.ttf`), so run the executable from a working directory that contains them.

## Quick Start

1. Clone the repository.
2. Install SDL2, SDL2_image, SDL2_ttf and SDL2_mixer and add them to your project.
3. Build the project and run it from the folder that contains `assets/`.
4. Open `main.cpp` to change the window title and size:

```cpp
game->init("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 840, 672, false);
//                     ^ title                                         ^ w   ^ h   ^ fullscreen
```

## Where to Write Your Own Game

All of your game code goes into the **`Game/`** folder. The files there contain the default demo game, which you can edit or replace completely.

| What you want to do | Where to do it |
| --- | --- |
| Load textures, sounds, fonts, and create your starting entities | `Game::init()` in `Game/Game.cpp` |
| Per-frame game logic (collision responses, scoring, spawning, win/lose rules) | `Game::update()` in `Game/Game.cpp` |
| Draw order and UI (score, text, HUD) | `Game::render()` in `Game/Game.cpp` |
| Free resources on exit | `Game::clean()` in `Game/Game.cpp` |
| Player input | `Game/KeyboardController.h` |
| Add entity groups (layers/categories) | the `groupLabels` enum in `Game/Game.h` |
| Add new textures/entity factories | `Managers/AssetsManager.cpp` (`AddTexture`, `Create...` functions) |
| Add a new reusable behavior | create a new component in `ECS/` (see below) |
| Window title, size, fullscreen, FPS | `main.cpp` |
| Levels / tile maps | `Game/Map.cpp` and your `.map` file in `assets/` |

### 1. Load your assets

In `Game::init()`:

```cpp
assets->AddTexture("hero", "assets/hero.png");
assets->AddTexture("enemy", "assets/enemy.png");
```

### 2. Create entities

Entities are empty containers. Give them behavior by adding components:

```cpp
auto& hero(manager.addEntity());
hero.addComponent<TransformComponent>(100, 100, 32, 32, 2); // x, y, width, height, scale
hero.addComponent<SpriteComponent>("hero", false);          // texture id, animated?
hero.addComponent<ColliderComponent>("hero");               // collision tag
hero.addGroup(Game::groupPlayers);
```

### 3. Add your own groups

In `Game/Game.h`, extend the enum:

```cpp
enum groupLabels : std::size_t
{
    groupMap,
    groupPlayers,
    groupColliders,
    // ...
    groupEnemies      // <-- your new group
};
```

Then fetch the group wherever you need it:

```cpp
auto& enemies(manager.getGroup(Game::groupEnemies));

for (auto& e : enemies)
{
    // e->getComponent<TransformComponent>().position ...
}
```

### 4. Write game logic

Put your rules in `Game::update()`:

```cpp
for (auto& e : enemies)
{
    SDL_Rect enemyBox = e->getComponent<ColliderComponent>().collider;
    SDL_Rect heroBox  = hero.getComponent<ColliderComponent>().collider;

    if (Collision::AABB(enemyBox, heroBox))
    {
        // hit!
    }
}
```

And make sure your group gets drawn in `Game::render()`:

```cpp
for (auto& e : enemies)
    e->draw();
```

### 5. Create your own component

Components derive from `Component` and can override `init()`, `update()` and `draw()`:

```cpp
// ECS/HealthComponent.h
#pragma once
#include "../ECS/ECS.h"
#include "../Engine/Components.h"

class HealthComponent : public Component
{
public:
    int health = 100;

    void init() override
    {
        // runs once when the component is added
    }

    void update() override
    {
        // runs every frame
        if (health <= 0)
            entity->destroy();   // removes the entity on the next refresh
    }
};
```

Register it by adding one line to `Engine/Components.h`:

```cpp
#include "../ECS/HealthComponent.h"
```

Use it like any other component:

```cpp
hero.addComponent<HealthComponent>();
hero.getComponent<HealthComponent>().health -= 10;
```

> Tip: `maxComponents` and `maxGroups` in `ECS/ECS.h` are both set to 32. Raise them if you need more.

### 6. Input

`KeyboardController` reads the keyboard state each frame (`W/A/S/D` and arrow keys by default, `P` toggles debug mode, `Esc` quits). Edit it to change controls, or write your own controller component and attach it to any entity.

### 7. Sound and text

- Load sounds in `Game::init()` with `Mix_LoadWAV("assets/sounds/yourSound.wav")`, and play them with `Game::playSound(sound)`.
- On-screen text is rendered with SDL_ttf using `terminal.ttf`. See `Game::updateScoreTexture()` for a ready-to-copy example.

### 8. Tile maps

`Map::LoadMap(path, sizeX, sizeY)` reads a plain text file in two parts: first a tile grid (each tile is two digits: row and column in the tileset), then a collision grid where `1` creates a solid collider.

```cpp
map = new Map("terrain", 3, 32);          // texture id, scale, tile size
map->LoadMap("assets/tiles/map.map", 25, 20); // path, width in tiles, height in tiles
```

## Built-in Components

| Component | Purpose |
| --- | --- |
| `TransformComponent` | Position, velocity, size, scale, speed |
| `SpriteComponent` | Draws a texture, supports animation, flip and fade-out |
| `ColliderComponent` | Axis-aligned collision box with a string tag |
| `TileComponent` | A single tile of a tile map |
| `ProjectileComponent` | Moves an entity in a direction up to a range |
| `ParticleComponent` | Particle emitter (`Emit(x, y, color)`) |
| `TrailComponent` | Draws a trail behind a moving entity |
| `KeyboardController` | Keyboard-driven movement and debug/quit keys |

## Contributing

Issues and pull requests are welcome. If you build something with Mikey Engine, feel free to share it!

## License

This project is licensed under the MIT License.
