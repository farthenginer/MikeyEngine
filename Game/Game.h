#ifndef Game_hpp
#define Game_hpp
#include "SDL.h"
#include <SDL_ttf.h>
#include "SDL_image.h"
#include <SDL_mixer.h>
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

	static void playSound(Mix_Chunk* sound);

	bool running() { return isRunning; };

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static bool isDebugging;
	static bool isPlayerHasAnimation;
	static SDL_Rect camera;
	static AssetsManager* assets;
	Uint64 lastFrameTime = 0;
	static float deltaTime;

	static Mix_Chunk* brickSound;
	static Mix_Chunk* hitPlayerSound;
	static Mix_Chunk* newBallSound;
	static Mix_Chunk* scoreUpSound;
	static Mix_Chunk* loseBallSound;
	static Mix_Chunk* hitWallSound;
	static Mix_Chunk* bonusBallSound;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupBalls,
		groupBricks,
		groupOldBricks,
		groupBackgrounds
	};

	static bool gameIsStarted;

	static void Start();

private:

	bool up;
	bool down;
	bool right;
	bool left;

	SDL_Window* window;

	int score = 0;
	bool scoreChanged = true;
	TTF_Font* font = nullptr;
	SDL_Texture* scoreTexture = nullptr;
	int scoreW = 0, scoreH = 0;

	void updateScoreTexture();
};
#endif // !Game_hpp
