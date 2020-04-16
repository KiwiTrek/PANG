#include "Enemy.h"

#include "Game.h"
#include "ModuleCollisions.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"
#include "ModuleRender.h"

Enemy::Enemy(int x, int y, ENEMY_TYPE _type) : position(x, y) {
    spawnPos = position;
    type = _type;
}

Enemy::~Enemy() { if (collider != nullptr) { collider->SetPendingToDelete(true); } }

Collider* Enemy::GetCollider() const { return collider; }

void Enemy::Update() {
    if (currentAnim != nullptr) { currentAnim->Update(); }
    if (collider != nullptr) { collider->SetPos(position.x, position.y, currentAnim->GetWidth(), currentAnim->GetHeight()); }
}

void Enemy::Draw() { if (currentAnim != nullptr) { game->GetModuleRender()->Blit(texture, position.x, position.y, false, &(currentAnim->GetCurrentFrame())); } }

void Enemy::SetLethality(bool _lethality) { lethal = _lethality; }

bool Enemy::GetLethality() { return lethal; }

ENEMY_TYPE Enemy::GetEnemyType() { return type; }

void Enemy::SetEnemyTexture(SDL_Texture* _texture) { texture = _texture; }

uint Enemy::GetDestroyedFx() { return destroyedFx; }

void Enemy::SetDestroyedFx(uint _destroyedFx) { destroyedFx = _destroyedFx; }

int Enemy::GetPositionX() { return position.x; }

int Enemy::GetPositionY() { return position.y; }

void Enemy::SetEnemyType(ENEMY_TYPE _type) { type = _type; }