#include "Game.h"
#include "../Managers/TextureManager.h"
#include "Map.h"
#include "../Engine/Components.h"
#include "../Engine/Vector2D.h"
#include "../Engine/Collision.h"
Map* map;

SDL_Renderer* Game::renderer = nullptr;

Manager manager;
SDL_Event Game::event;

SDL_Rect Game::camera = {0,0,800,640};

bool Game::isRunning = false;
bool Game::isDebugging = false;

auto& player(manager.addEntity());

Game::Game()
{
}
Game ::~Game()
{
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen)
{
	//initialize

	int flags = 0;
	if (fullScreen)
	{
		flags = SDL_WINDOW_FULLSCREEN; //fullscreen check
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised!..." << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window Created!" << std::endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created!" << std::endl;
		}

		isRunning = true;

	}
	else
	{
		isRunning = false;
	}

	//

	//Create Objects;

	map = new Map("assets/tiles/terrain_ss.png", 3, 32);

	map->LoadMap("assets/tiles/map.map", 25, 20);

	player.addComponent<TransformComponent>(600,555,16,16,4);
	player.addComponent<SpriteComponent>("assets/Player/Animations/Movement/Player_Movement.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);
}


auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));


void Game::handleEvents()
{
	SDL_PollEvent(&Game::event);

	switch (Game::event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	}
}

void Game::update()
{
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	manager.refresh();
	manager.update();

	for (auto& c : colliders) 
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	camera.x = player.getComponent<TransformComponent>().position.x - 400;
	camera.y = player.getComponent<TransformComponent>().position.y - 320;

	if (camera.x < 0)
		camera.x = 0;

	if (camera.y < 0)
		camera.y = 0;

	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;
}

void Game::render()
{
	SDL_RenderClear(renderer);
	
	for (auto& t : tiles)
		t->draw();

	for (auto& c : colliders)
		if (isDebugging) c->draw();

	for (auto& p : players)
		p->draw();

	//manager.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}
