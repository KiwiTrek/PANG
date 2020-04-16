#include "Enemy_Balloon.h"
#include "Game.h"
#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleEnemies.h"
#include "Enemy.h"

Enemy_Balloon::Enemy_Balloon(int x, int y, ENEMY_TYPE _type, bool _isMovingRight) : Enemy(x, y, _type, _isMovingRight) {
    if (GetEnemyType() == ENEMY_TYPE::CHUNGUS_BALLOON ) {
        idle.PushBack({ 1,6,48,40 });
        collider = game->GetModuleCollisions()->AddCollider({ 1,6,48,40 }, Collider::TYPE::BALLOON, (Module*)game->GetModuleEnemies());
    }
    if (GetEnemyType() == ENEMY_TYPE::NOT_THAT_MEH_BALLOON) {
        idle.PushBack({ 52,13,33,27 });
        collider = game->GetModuleCollisions()->AddCollider({ 52,13,33,27 }, Collider::TYPE::BALLOON, (Module*)game->GetModuleEnemies());
    }
    if (GetEnemyType() == ENEMY_TYPE::MEH_BALLOON) {
        idle.PushBack({ 86,19,17,15 });
        collider = game->GetModuleCollisions()->AddCollider({ 86,19,17,15 }, Collider::TYPE::BALLOON, (Module*)game->GetModuleEnemies());
    }
    if (GetEnemyType() == ENEMY_TYPE::SMOL_BALLOON) {
        idle.PushBack({ 106,23,9,8 });
        collider = game->GetModuleCollisions()->AddCollider({ 106,23,9,8 }, Collider::TYPE::BALLOON, (Module*)game->GetModuleEnemies());
    }
    currentAnim = &idle;
    if (isMovingRight) { speed.x = 150.0f; }
    else { speed.x = -75.0f; }
    speed.y = -50.0f;
}

void Enemy_Balloon::Update() {

    //This is where the physics go
    const float gravity = 600.0f;				// pixels / second^2
    const float deltaTime = 1.0f / 60.0f;		// More or less 60 frames per second

    position.x = position.x + speed.x * deltaTime;
    position.y = position.y + speed.y * deltaTime + gravity * deltaTime * deltaTime;
    speed.y = speed.y + gravity * deltaTime;

    // Call to the base class. It must be called at the end
    // It will update the collider depending on the position
    // LOG("Speed: X = %f, Y = %f\n",speed.x, speed.y);
    Enemy::Update();
}

void Enemy_Balloon::OnCollision(Collider* c2) {
    if (c2->GetType() == Collider::TYPE::WALL) {
        if (position.x < (c2->GetRect().x + c2->GetRect().w) && position.x > c2->GetRect().x) {
            position.x = (c2->GetRect().x + c2->GetRect().w);
            if (speed.x <= 75.0f) { speed.x = -(speed.x * 2); } //We're as clueless as you on the reason why this works.
        }
        else if ((position.x + currentAnim->GetWidth()) > c2->GetRect().x&& position.x < c2->GetRect().x) {
            position.x = (c2->GetRect().x - currentAnim->GetWidth());
            if (speed.x >= 75.0f) { speed.x = -(speed.x / 2); } //Please do not question it.
        }
        //if (position.y >= 186 - currentAnim->GetHeight()) {
        //    position.y = 186 - currentAnim->GetHeight();
        //}
    }
    else if (c2->GetType() == Collider::TYPE::FLOOR) {
        if (position.y < (c2->GetRect().y + c2->GetRect().h) && position.y > c2->GetRect().y) {
            position.y = (c2->GetRect().y + c2->GetRect().h);
            speed.y = (-speed.y + 50.0f);
        }
        else if ((position.y + currentAnim->GetHeight()) > c2->GetRect().y&& position.y < c2->GetRect().y) {
            position.y = (c2->GetRect().y - currentAnim->GetHeight());
            speed.y = (-speed.y + 50.0f);
        }
    }
    else if (c2->GetType() == Collider::TYPE::PLAYER_SHOT) { SetLethality(true); }
}