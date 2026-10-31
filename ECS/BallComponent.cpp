#include "BallComponent.h"
#include "../Managers/AssetsManager.h"
#include "../Game/Game.h"

void BallComponent::CreateNewBall(bool create)
{
	Game::assets->CreateBall(Vector2D(400, 300), Vector2D(0, 1), 8, "ball", create);
}

void BallComponent::PlayHitWallSound()
{
	Game::playSound(Game::hitWallSound);
}

void BallComponent::PlayLoseBallSound()
{
	Game::playSound(Game::loseBallSound);
}