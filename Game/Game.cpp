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

std::vector<ColliderComponent*> Game::colliders;

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& wall(manager.addEntity());

const char* mapfile = "assets/tiles/terrain_ss.png"; //tilemap sys

enum groupLabels : std::size_t
{
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders
};

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

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

	map = new Map();

	Map::LoadMap("assets/tiles/map.map", 25, 20);

	player.addComponent<TransformComponent>(400,320,16,16,4);
	player.addComponent<SpriteComponent>("assets/Player/Animations/Movement/Player_Movement.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);
}

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
	manager.refresh();
	manager.update();

	Vector2D pVel = player.getComponent<TransformComponent>().velocity;
	int pSpeed = player.getComponent<TransformComponent>().speed;

	for (auto t : tiles)
	{
		t->getComponent<TileComponent>().destRect.x += -(pVel.x * pSpeed);
		t->getComponent<TileComponent>().destRect.y += -(pVel.y * pSpeed);
	}

	/*
	In its simplest form, since this game engine lacked a camera component, 
	all other objects were moved in inverse proportion to the character to
	create the impression of a camera effect.
	*/

	/*
	for (auto cc : colliders)
	{
		Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
	}*/
}

void Game::render()
{
	SDL_RenderClear(renderer);
	
	for (auto& t : tiles)
		t->draw();

	for (auto& p : players)
		p->draw();

	for (auto& e : enemies)
		e->draw();

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

void Game::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile = manager.addEntity();
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapfile);
	tile.addGroup(groupMap);
}
