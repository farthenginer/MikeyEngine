#ifndef Game_hpp
#define Game_hpp
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

class ColliderComponent;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen);
	void handleEvents();
	void update();
	void render();
	void clean();

	static void AddTile(int srcX, int srcY, int xpos, int ypos);

	bool running() { return isRunning; };

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static std::vector<ColliderComponent*> colliders;
	static bool isRunning;

private:

	bool up;
	bool down;
	bool right;
	bool left;

	SDL_Window* window;
};
#endif // !Game_hpp
