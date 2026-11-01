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

auto& player(manager.addEntity());

//sounds
//sounds
Mix_Chunk* Game::brickSound = nullptr;
Mix_Chunk* Game::hitPlayerSound = nullptr;
Mix_Chunk* Game::newBallSound = nullptr;
Mix_Chunk* Game::scoreUpSound = nullptr;
Mix_Chunk* Game::loseBallSound = nullptr;
Mix_Chunk* Game::hitWallSound = nullptr;
Mix_Chunk* Game::bonusBallSound = nullptr;

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

		brickSound = Mix_LoadWAV("assets/sounds/BreakOut.wav");
		hitPlayerSound = Mix_LoadWAV("assets/sounds/HitPlayer.wav");
		newBallSound = Mix_LoadWAV("assets/sounds/NewBall.wav");
		scoreUpSound = Mix_LoadWAV("assets/sounds/NewScore.wav");
		loseBallSound = Mix_LoadWAV("assets/sounds/LoseBall.wav");
		hitWallSound = Mix_LoadWAV("assets/sounds/HitWall.wav");
		bonusBallSound = Mix_LoadWAV("assets/sounds/BonusBall.wav");

		//if (!brickSound)     std::cout << "BreakOut.wav error: " << Mix_GetError() << std::endl;
		//if (!hitPlayerSound) std::cout << "HitPlayer.wav error: " << Mix_GetError() << std::endl;
		//if (!newBallSound)   std::cout << "NewBall.wav error: " << Mix_GetError() << std::endl;
		//if (!scoreUpSound)   std::cout << "NewScore.wav error: " << Mix_GetError() << std::endl;

		Mix_VolumeMusic(64);		isRunning = true;

	}
	else
	{
		isRunning = false;
	}

	//Create Objects;

	assets->AddTexture("terrain", "assets/tiles/breakout.png");
	assets->AddTexture("player", "assets/Player/bricket2.png");
	assets->AddTexture("projectile", "assets/objects/projectile.png");
	assets->AddTexture("ball", "assets/objects/ball2.png");
	
	assets->AddTexture("brick0", "assets/objects/bricks/brick_0.png");
	assets->AddTexture("brick1", "assets/objects/bricks/brick_1.png");
	assets->AddTexture("brick2", "assets/objects/bricks/brick_2.png");
	assets->AddTexture("brick3", "assets/objects/bricks/brick_3.png");
	assets->AddTexture("brick4", "assets/objects/bricks/brick_4.png");
	assets->AddTexture("brick_bonus", "assets/objects/bricks/brick_bonus.png");

	assets->AddTexture("background", "assets/tiles/background.png");

	map = new Map("terrain", 3, 32);

    map->LoadMap("assets/tiles/map.map", 25, 20);

	player.addComponent<TransformComponent>(368,600,32,8,4);
	player.addComponent<SpriteComponent>("player", false);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	isPlayerHasAnimation = false;

	//assets->CreateProjectile(Vector2D(400, 400), Vector2D(2,0) , 200, 2, "projectile");
	//assets->CreateProjectile(Vector2D(500, 460), Vector2D(2,-1) , 200, 2, "projectile");
	//assets->CreateProjectile(Vector2D(600, 570), Vector2D(-2,0) , 200, 2, "projectile");

	//assets->CreateBall(Vector2D(400, 500), Vector2D(0, 1), 8, "ball", true);
	assets->CreateBackground(Vector2D(0, 0), "background");


	//Create Bricks
	int bricksX = 14;
	int bricksY = 8;

	int paddingX = 56;

	int spaceX = 52;
	int spaceY = 36;

	float xPos = 0;
	float yPos = 48;

	for (int y = 0; y < bricksY; y++)
	{
		xPos = paddingX;


		for (int x = 0; x < bricksX; x++)
		{

			int randomBonus = rand() % 11;

			if (randomBonus != 1)
			{
				int randomNum = rand() % 5;

				const char* genId = "brick";

				std::string randNumStr = std::to_string(randomNum);

				std::string idResult = std::string(genId) + randNumStr;

				const char* res = idResult.c_str();
				assets->CreateBrick(Vector2D(xPos, yPos), res, false);

				std::cout << res << std::endl;
			}
			else
			{
				assets->CreateBrick(Vector2D(xPos, yPos), "brick_bonus", true);
			}

			//generate random num

			xPos += spaceX;
		}

		yPos += spaceY;
		xPos = paddingX;
	}
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& balls(manager.getGroup(Game::groupBalls));
auto& bricks(manager.getGroup(Game::groupBricks));
auto& oldBricks(manager.getGroup(Game::groupOldBricks));
auto& backgrounds(manager.getGroup(Game::groupBackgrounds));

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
		/*
		for (auto& b : balls)
		{
			SDL_Rect bCol = b->getComponent<ColliderComponent>().collider;
			
			if (Collision::AABB(cCol, bCol))
			{
				float xPos = b->getComponent<TransformComponent>().velocity.x;
				float yPos = b->getComponent<TransformComponent>().velocity.y;

				b->getComponent<TransformComponent>().velocity = Vector2D(-xPos, yPos);
			}
		}*/

	}
	for (auto& p : players)
	{
		for (auto& b : balls)
		{
			SDL_Rect pCol = p->getComponent<ColliderComponent>().collider;
			SDL_Rect bCol = b->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(pCol, bCol))
			{
				std::cout << "Hit" << std::endl;

				Vector2D pVec = p->getComponent<TransformComponent>().position;
				Vector2D bVec = b->getComponent<TransformComponent>().position;

				//std::cout << "Player : " << pVec << "\n" << "Ball" << bVec << std::endl;

				int calculatedX = pVec.x - bVec.x;

				if (calculatedX < -32)
				{
					//std::cout << "Right" << std::endl;
				}
				else if (calculatedX > -32)
				{
					//std::cout << "Left" << std::endl;
				}
				else if (calculatedX == -32)
				{
					//std::cout << "Mid" << std::endl;
				}

				float result = calculatedX / 80.0f + 0.4f;

				std::cout << result << std::endl;
				
				Vector2D newVec = Vector2D::Vector2D(-result, -1);

				b->getComponent<TransformComponent>().velocity = newVec;
				Mix_PlayChannel(-1, hitPlayerSound, 0);

				//std::cout << pVec.x - bVec.x << std::endl;
			}
		}
		
	}
	for (auto& p : balls)
	{
		for (auto& b : bricks)
		{
			SDL_Rect pCol = p->getComponent<ColliderComponent>().collider;
			SDL_Rect bCol = b->getComponent<ColliderComponent>().collider;

			if (Collision::AABB(pCol, bCol))
			{
				std::cout << "Brick Hit" << std::endl;

				Vector2D pVec = b->getComponent<TransformComponent>().position;
				Vector2D bVec = p->getComponent<TransformComponent>().position;

				//std::cout << "Player : " << pVec << "\n" << "Ball" << bVec << std::endl;

				int calculatedX = pVec.x - bVec.x;

				if (calculatedX < -32)
				{
					//std::cout << "Right" << std::endl;
				}

				else if (calculatedX > -32)
				{
					//std::cout << "Left" << std::endl;
				}

				else if (calculatedX == -32)
				{
					//std::cout << "Mid" << std::endl;
				}

				float result = calculatedX / 80.0f + 0.4f;

				std::cout << result << std::endl;
				
				Vector2D newVec = Vector2D::Vector2D(-result, p->getComponent<TransformComponent>().velocity.y * -1);

				p->getComponent<TransformComponent>().velocity = newVec;

				particleSystem.Emit(
					b->getComponent<TransformComponent>().position.x + b->getComponent<TransformComponent>().width / 2,
					b->getComponent<TransformComponent>().position.y + b->getComponent<TransformComponent>().height / 2,
					SDL_Color{ 255, 255, 255, 255 }
				);

				if (b->getComponent<BrickComponent>().isBonus == true)
				{
					assets->CreateBall(Vector2D(b->getComponent<TransformComponent>().position.x, b->getComponent<TransformComponent>().position.y), Vector2D(0, 1), 8, "ball", false);
					//play sfx
					Mix_PlayChannel(-1, bonusBallSound, 0);
				}

				//b->getComponent<BrickComponent>().entity->destroy();
				//fade out animation

			    b->getComponent<SpriteComponent>().FadeOut(0.25f);

				score++;
				if (score % 100 == 0)
				{
					Mix_PlayChannel(-1, scoreUpSound, 0);
				}
				scoreChanged = true;

				Mix_PlayChannel(-1, brickSound, 0);

				//std::cout << pVec.x - bVec.x << std::endl;

				break;
			}
		}
	}
	/*
	if (player.getComponent<TransformComponent>().position.x < 1)
	{
		player.getComponent<TransformComponent>().velocity.x = 1;
	}
	if (player.getComponent<TransformComponent>().position.x > 712)
	{
		player.getComponent<TransformComponent>().velocity.x = -1;
	}
	*/

	/* CAMERA FOLLOW

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
		*/
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

	for (auto& b : backgrounds)
		b->draw();

	for (auto& t : tiles)
		//t->draw();

	for (auto& c : colliders)
		c->draw();

	for (auto& p : players)
		p->draw();
	
	for (auto& c : colliders)
		c->draw();

	for (auto& b : balls)
		b->draw();

	for (auto& b : bricks)
		b->draw();
	for (auto& b : oldBricks)
		b->draw();

	particleSystem.Render(renderer);

	//manager.draw();

	if (scoreChanged)
	{
		updateScoreTexture();
		scoreChanged = false;
	}

	if (scoreTexture)
	{
		SDL_Rect dst = { (800 - scoreW) / 2, 8, scoreW, scoreH };
		SDL_RenderCopy(renderer, scoreTexture, nullptr, &dst);
	}

	SDL_RenderPresent(renderer);
}

void Game::updateScoreTexture()
{
	if (!font) return;

	if (scoreTexture)
		SDL_DestroyTexture(scoreTexture);

	char buf[16];
	snprintf(buf, sizeof(buf), "%03d", score);   //000, 001, 002 etc.

	SDL_Color white = { 255, 255, 255, 255 };
	SDL_Surface* surf = TTF_RenderText_Blended(font, buf, white);
	if (!surf) return;

	scoreW = surf->w;
	scoreH = surf->h;
	scoreTexture = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
}

void Game::clean()
{
	if (scoreTexture) SDL_DestroyTexture(scoreTexture);
	if (font) TTF_CloseFont(font);

	Mix_FreeChunk(brickSound);
	Mix_FreeChunk(hitPlayerSound);
	Mix_FreeChunk(newBallSound);
	Mix_FreeChunk(scoreUpSound);
	Mix_FreeChunk(loseBallSound);
	Mix_FreeChunk(hitWallSound);

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
	assets->CreateBall(Vector2D(player.getComponent<TransformComponent>().position.x + 32, player.getComponent<TransformComponent>().position.y - 48), Vector2D(0, -1), 8, "ball", true);
}
