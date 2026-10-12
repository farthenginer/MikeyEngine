#include "Game.h"

Game* game = nullptr;
int main(int argc, char* argv[])
{
	const int FPS = 60; //FPS int
	const int frameDelay = 1000 / FPS; //Per Second

	Uint32 frameStart;
	int frameTime;

	game = new Game(); //Init Game
	game->init("Mikey Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);//Init settings 

	while (game->running()) //Game Loop Check
	{
		frameStart = SDL_GetTicks(); //FPS

		game->handleEvents(); //event loop
		game->update(); //update
		game->render(); //render

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime); //FPS Set
		}
	}
	game->clean(); //clean Memory

	return 0;
}