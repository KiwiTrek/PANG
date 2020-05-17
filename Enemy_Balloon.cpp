#include "Enemy_Balloon.h"

#include "Game.h"

#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleEnemies.h"
#include "ModuleRender.h"
#include "Enemy.h"

Enemy_Balloon::Enemy_Balloon(int x, int y, ENEMY_TYPE _type, bool _isMovingRight) : Enemy(x, y, _type, _isMovingRight) {
    if (GetEnemyType() == ENEMY_TYPE::CHUNGUS_BALLOON ) {
        idle.PushBack({ 1,6,48,40 });
        collider = game->GetModuleCollisions()->AddCollider({ 36,16,48,40 }, Collider::TYPE::BALLOON, (Module*)game->GetModuleEnemies());
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
        idle.PushBack({ 106,23,8,7 });
        collider = game->GetModuleCollisions()->AddCollider({ 106,23,8,7 }, Collider::TYPE::BALLOON, (Module*)game->GetModuleEnemies());
    }
    currentAnim = &idle;
    if (isMovingRight) { speed.x = 119.5f; } // needs changing
    else { speed.x = -59.25f; }
    speed.y = -40.0f;
    physics.SetAxis(true, true);
}

void Enemy_Balloon::Update() {
    // This is where the physics go
    physics.UpdatePhysics(position.x, position.y, speed.x, speed.y);

    // This is where the movement collisions go
    //LEFT WALL
    if (physics.CheckMovementCollision({ position.x, position.y,currentAnim->GetWidth(), currentAnim->GetHeight() }, { 0,0, TILE_SIZE, TILE_SIZE * 26 })) {
        position.x = 2 * TILE_SIZE - position.x;
        speed.x = -(speed.x * 2);
    }
    //TOP FLOOR

    //RIGHT WALL
    if (physics.CheckMovementCollision({ position.x, position.y,currentAnim->GetWidth(), currentAnim->GetHeight() }, { TILE_SIZE * 47,0, TILE_SIZE, TILE_SIZE * 26 })) {
        position.x = 2 * TILE_SIZE * 47 - currentAnim->GetWidth() * 2 - position.x;
        speed.x = -(speed.x / 2);
    }
    //BOTTOM FLOOR
    if (physics.CheckMovementCollision({ position.x, position.y,currentAnim->GetWidth(), currentAnim->GetHeight() }, { 0,TILE_SIZE * 25, TILE_SIZE * 48, TILE_SIZE })) {
        position.y = 2 * TILE_SIZE * 25 - currentAnim->GetHeight() * 2 - position.y;
        if (GetEnemyType() == CHUNGUS_BALLOON) { speed.y = -physics.CalculateSpeed(position, speed, currentAnim->GetHeight(), TILE_SIZE * 3); } // -375
        if (GetEnemyType() == NOT_THAT_MEH_BALLOON) { speed.y = -physics.CalculateSpeed(position, speed, currentAnim->GetHeight(), TILE_SIZE * 10); } // - 312
        if (GetEnemyType() == MEH_BALLOON) { speed.y = -physics.CalculateSpeed(position, speed, currentAnim->GetHeight(), TILE_SIZE * 14); } // -252
        if (GetEnemyType() == SMOL_BALLOON) { speed.y = -physics.CalculateSpeed(position, speed, currentAnim->GetHeight(), TILE_SIZE * 20); } // -197
    }
    //LOG("PositionY = %d\n", position.y);
    // Call to the base class. It must be called at the end
    // It will update the collider depending on the position
    Enemy::Update();
}

void Enemy_Balloon::OnCollision(Collider* c1, Collider* c2) { if (c2->GetType() == Collider::TYPE::PLAYER_SHOT) { SetLethality(true); } }