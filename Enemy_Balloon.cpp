#include "Enemy_Balloon.h"
#include "Game.h"
#include "ModuleCollisions.h"

Enemy_Balloon::Enemy_Balloon(int x, int y) : Enemy(x, y)
{
	fly.PushBack({ 5,72,21,22 });
	currentAnim = &fly;


	collider = game->GetModuleCollisions()->AddCollider({ 0, 0, 24, 24 }, Collider::TYPE::BALLOON, (Module*)game->GetModuleEnemies());
}

void Enemy_Balloon::Update()
{
	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();
}
