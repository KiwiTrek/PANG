#include "Enemy.h"

#include "Game.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"

Enemy::Enemy(int x, int y) : position(x, y) { spawnPos = position; }

Enemy::~Enemy() { if (collider != nullptr) { collider->SetPendingToDelete(true); } }

Collider* Enemy::GetCollider() const { return collider; }

void Enemy::Update() {
    if (currentAnim != nullptr) { currentAnim->Update(); }
    if (collider != nullptr) { collider->SetPos(position.x, position.y, currentAnim->GetWidth(), currentAnim->GetHeight()); }
}

void Enemy::Draw() { if (currentAnim != nullptr) { game->GetModuleRender()->Blit(texture, position.x, position.y, false, &(currentAnim->GetCurrentFrame())); } }

void Enemy::SetLethality(bool _lethality) { lethal = _lethality; }

bool Enemy::GetLethality() { return lethal; }