#include "Game.h"
#include "../Managers/TextureManager.h"
#include "Map.h"
#include "../Engine/Components.h"
#include "../Engine/Vector2D.h"
#include "../Engine/Collision.h"
#include "../Managers/AssetsManager.h"
#include <cstdlib>
#include <cstdio>

Map* map;

SDL_Renderer* Game::renderer = nullptr;

Manager manager;
SDL_Event Game::event;

SDL_Rect Game::camera = {0,0,800,640};

AssetsManager* Game::assets = new AssetsManager(&manager);

ParticleComponent particleSystem;

bool Game::isRunning = false;
bool Game::isDebugging = false;
bool Game::isPlayerHasAnimation = false;
bool Game::gameIsStarted = false;

float Game::deltaTime = 0.0f;

Game::Game()
{
}
Game ::~Game()
{
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen)
{
	//initialize
	//SDL_Init(SDL_INIT_AUDIO);

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
		if (TTF_Init() == -1)
			std::cout << "TTF_Init error: " << TTF_GetError() << std::endl;

		font = TTF_OpenFont("terminal.ttf", 48);
		if (!font)
			std::cout << "Font error: " << TTF_GetError() << std::endl;

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			std::cout << "Mix_OpenAudio error: " << Mix_GetError() << std::endl;

		Mix_VolumeMusic(64);		isRunning = true;

	}
	else
	{
		isRunning = false;
	}
}

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
	manager.refresh();
	manager.update();
	
	Uint64 currentTime = SDL_GetPerformanceCounter();

	Game::deltaTime = 0.0f;

	if (lastFrameTime != 0)
	{
		Game::deltaTime = (float)(currentTime - lastFrameTime)
			/ SDL_GetPerformanceFrequency();
	}

	lastFrameTime = currentTime;

	particleSystem.Update(Game::deltaTime);
}

void Game::render()
{
	SDL_RenderClear(renderer);

	for (auto& c : colliders)
		c->draw();

	for (auto& p : players)
		p->draw();

	particleSystem.Render(renderer);

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	if (textTexture) SDL_DestroyTexture(textTexture);
	if (font) TTF_CloseFont(font);

	Mix_CloseAudio();
	Mix_Quit();

	TTF_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}

void Game::playSound(Mix_Chunk* sound)
{
	if (sound) Mix_PlayChannel(-1, sound, 0);
}

void Game::Start()
{
	gameIsStarted = true;
}
