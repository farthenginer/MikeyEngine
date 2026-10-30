#ifndef Game_hpp
#define Game_hpp
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

class AssetsManager;   //forward declaration
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

	bool running() { return isRunning; };

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static bool isDebugging;
	static bool isPlayerHasAnimation;
	static SDL_Rect camera;
	static AssetsManager* assets;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupBalls,
		groupBricks
	};

private:

	bool up;
	bool down;
	bool right;
	bool left;

	SDL_Window* window;
};
#endif // !Game_hpp
