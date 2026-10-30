#include "BallComponent.h"
#include "../Managers/AssetsManager.h"
#include "../Game/Game.h"

void BallComponent::CreateNewBall()
{
	Game::assets->CreateBall(Vector2D(400, 300), Vector2D(0, 1), 8, "ball");
}
