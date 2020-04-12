#include "Enemy_Balloon.h"
#include "Game.h"
#include "ModuleCollisions.h"

Enemy_Balloon::Enemy_Balloon(int x, int y) : Enemy(x, y) {
    idle.PushBack({ 1,6,48,40 });
    currentAnim = &idle;

    collider = game->GetModuleCollisions()->AddCollider({ 1,6,48,40 }, Collider::TYPE::BALLOON, (Module*)game->GetModuleEnemies());
}

void Enemy_Balloon::Update() {
    //This is where the physics go

    // Call to the base class. It must be called at the end
    // It will update the collider depending on the position
    Enemy::Update();
}
