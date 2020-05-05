#include "Enemy_Balloon.h"

#include "Game.h"

#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleEnemies.h"
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

    //This is where the physics go

    physics.UpdatePhysics(position.x, position.y, speed.x, speed.y);

    // LOG("Speed: X = %f, Y = %f\n",speed.x, speed.y);

    // Call to the base class. It must be called at the end
    // It will update the collider depending on the position
    Enemy::Update();
}

void Enemy_Balloon::OnCollision(Collider* c1, Collider* c2) {
    if (c2->GetType() == Collider::TYPE::WALL) {
        if (c1->GetRect().x < (c2->GetRect().x + c2->GetRect().w) && c1->GetRect().x > c2->GetRect().x) {
             position.x = (c2->GetRect().x + c2->GetRect().w);
             if (speed.x <= -59.25f) { speed.x = -(speed.x * 2); } //We're as clueless as you on the reason why this works.
        }
        else if ((c1->GetRect().x + c1->GetRect().w) > c2->GetRect().x && c1->GetRect().x < c2->GetRect().x) {
            position.x = (c2->GetRect().x - currentAnim->GetWidth());
            if (speed.x >= -59.25f) { speed.x = -(speed.x / 2); } //Please do not question it.
        }
    } // (position.x + (currentAnim->GetWidth()/5) - 1, position.y, currentAnim->GetWidth() - (currentAnim->GetWidth()*2/5) + 1, currentAnim->GetHeight());
    else if (c2->GetType() == Collider::TYPE::FLOOR) {
        if (c1->GetRect().y < (c2->GetRect().y + c2->GetRect().h) && c1->GetRect().y > c2->GetRect().y && c1->GetRect().y < 50) { position.y = (c2->GetRect().y + c2->GetRect().h); }
        else if ((c1->GetRect().y + c1->GetRect().h) > c2->GetRect().y && c1->GetRect().y < c2->GetRect().y && c1->GetRect().y > 50) {
            position.y = (c2->GetRect().y - c1->GetRect().h);
            if (GetEnemyType() == CHUNGUS_BALLOON) { speed.y = (-375); }
            if (GetEnemyType() == NOT_THAT_MEH_BALLOON) { speed.y = (-312); }
            if (GetEnemyType() == MEH_BALLOON) { speed.y = (-252); }
            if (GetEnemyType() == SMOL_BALLOON) { speed.y = (-197); }
        }
    }
    else if (c2->GetType() == Collider::TYPE::PLAYER_SHOT) { SetLethality(true); }
}