#include "ModuleEnemies.h"

#include "Game.h"

#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleParticles.h"
#include "ModulePlayer.h"
#include "ModuleAudio.h"

#include "ModuleLevelOne.h"
#include "ModuleLevelTwo.h"
#include "ModuleLevelThree.h"

#include "Enemy.h"
#include "Enemy_Balloon.h"

#define SPAWN_MARGIN 50

ModuleEnemies::ModuleEnemies(bool startEnabled) : Module(startEnabled) { for (uint i = 0; i < MAX_ENEMIES; ++i) { enemies[i] = nullptr; } }

ModuleEnemies::~ModuleEnemies() {}

bool ModuleEnemies::Start() {
    enemyTexture = game->GetModuleTextures()->Load("Resources/Sprites/enemies.png");
    balloogiExplosioni = game->GetModuleAudio()->LoadFx("Resources/SFX/balloonPopped.wav");

    return true;
}

UPDATE_STATUS ModuleEnemies::Update() {
    HandleEnemiesSpawn();
    if (game->GetModuleLevelOne()->CheckIfStarted() ||
        game->GetModuleLevelTwo()->CheckIfStarted() ||
        game->GetModuleLevelThree()->CheckIfStarted()) {
        if (game->GetModulePlayer()->CheckIfDestroyed() == false) {
            for (uint i = 0; i < MAX_ENEMIES; ++i) {
                if (enemies[i] != nullptr) { enemies[i]->Update(); }
            }
        }
    }
    
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleEnemies::PostUpdate() {
    for (uint i = 0; i < MAX_ENEMIES; ++i)
    {
        if (enemies[i] != nullptr) { enemies[i]->Draw(); }
    }

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEnemies::CleanUp() {
    LOG("Freeing all enemies");

    for (uint i = 0; i < MAX_ENEMIES; ++i) {
        if (enemies[i] != nullptr) {
            delete enemies[i];
            enemies[i] = nullptr;
        }
    }

    game->GetModuleTextures()->Unload(enemyTexture);
    game->GetModuleAudio()->UnloadFx(balloogiExplosioni);

    return true;
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPE type, int x, int y, bool isMovingRight) {
    for (uint i = 0; i < MAX_ENEMIES; ++i) {
        if (spawnQueue[i].type == ENEMY_TYPE::NO_TYPE) {
            spawnQueue[i].type = type;
            spawnQueue[i].x = x;
            spawnQueue[i].y = y;
            spawnQueue[i].isMovingRight = isMovingRight;
            return true;
        }
    }

    return false;
}

void ModuleEnemies::HandleEnemiesSpawn() {
    // Iterate all the enemies queue
    for (uint i = 0; i < MAX_ENEMIES; ++i) {
        if (spawnQueue[i].type != ENEMY_TYPE::NO_TYPE) {
            SpawnEnemy(spawnQueue[i]);
            spawnQueue[i].type = ENEMY_TYPE::NO_TYPE;
        }
    }
}

void ModuleEnemies::SpawnEnemy(const EnemySpawnpoint& info) {
    // Find an empty slot in the enemies array
    for (uint i = 0; i < MAX_ENEMIES; ++i) {
        if (enemies[i] == nullptr) {
            switch (info.type) {
            case ENEMY_TYPE::CHUNGUS_BALLOON:
                enemies[i] = new Enemy_Balloon(info.x, info.y, info.type, info.isMovingRight);
                break;
            case ENEMY_TYPE::NOT_THAT_MEH_BALLOON:
                enemies[i] = new Enemy_Balloon(info.x, info.y, info.type, info.isMovingRight);
                break;
            case ENEMY_TYPE::MEH_BALLOON:
                enemies[i] = new Enemy_Balloon(info.x, info.y, info.type, info.isMovingRight);
                break;
            case ENEMY_TYPE::SMOL_BALLOON:
                enemies[i] = new Enemy_Balloon(info.x, info.y, info.type, info.isMovingRight);
                break;
            }
            enemies[i]->SetEnemyTexture(enemyTexture);
            enemies[i]->SetDestroyedFx(balloogiExplosioni);
            break;
        }
    }
}

bool ModuleEnemies::CheckForBalloons() {
    for (uint i = 0; i < MAX_ENEMIES; ++i) { if (enemies[i] != nullptr && enemies[i]->GetCollider()->GetType() == Collider::TYPE::BALLOON) { return false; } }
    return true;
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2) {
    for (uint i = 0; i < MAX_ENEMIES; ++i) {
        if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1) {
            enemies[i]->OnCollision(c1, c2); //Notify the enemy of a collision
            if (enemies[i]->GetLethality()) {
                if (c1->GetType() == Collider::TYPE::BALLOON) { game->GetModuleAudio()->PlayFx(enemies[i]->GetDestroyedFx()); }

                switch (enemies[i]->GetEnemyType()) {
                case ENEMY_TYPE::CHUNGUS_BALLOON: {
                    if (game->GetModuleLevelOne()->IsEnabled()) { game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->rBigBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY()); }
                    else if (game->GetModuleLevelTwo()->IsEnabled()) { game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->bBigBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY()); }
                    else if (game->GetModuleLevelThree()->IsEnabled()) { game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->gBigBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY()); }

                    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::NOT_THAT_MEH_BALLOON, enemies[i]->GetPositionX(), enemies[i]->GetPositionY() + (enemies[i]->GetCurrentAnimation()->GetHeight() / 2));
                    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::NOT_THAT_MEH_BALLOON, enemies[i]->GetPositionX() + (enemies[i]->GetCurrentAnimation()->GetWidth() / 2), enemies[i]->GetPositionY() + (enemies[i]->GetCurrentAnimation()->GetHeight() / 2), true);
                    game->GetModulePlayer()->AddScore(50);
                    break;
                }
                case ENEMY_TYPE::NOT_THAT_MEH_BALLOON: {
                    if (game->GetModuleLevelOne()->IsEnabled()) { game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->rNotThatMehBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY()); }
                    else if (game->GetModuleLevelTwo()->IsEnabled()) { game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->bNotThatMehBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY()); }
                    else if (game->GetModuleLevelThree()->IsEnabled()) { game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->gNotThatMehBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY()); }


                    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::MEH_BALLOON, enemies[i]->GetPositionX(), enemies[i]->GetPositionY() + (enemies[i]->GetCurrentAnimation()->GetHeight() / 2));
                    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::MEH_BALLOON, enemies[i]->GetPositionX() + (enemies[i]->GetCurrentAnimation()->GetWidth() / 2), enemies[i]->GetPositionY() + (enemies[i]->GetCurrentAnimation()->GetHeight() / 2), true);
                    game->GetModulePlayer()->AddScore(100);
                    break;
                }
                case ENEMY_TYPE::MEH_BALLOON: {
                    if (game->GetModuleLevelOne()->IsEnabled()) { game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->rMehBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY()); }
                    else if (game->GetModuleLevelTwo()->IsEnabled()) { game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->bMehBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY()); }
                    else if (game->GetModuleLevelThree()->IsEnabled()) { game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->gMehBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY()); }

                    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::SMOL_BALLOON, enemies[i]->GetPositionX(), enemies[i]->GetPositionY() + (enemies[i]->GetCurrentAnimation()->GetHeight() / 2));
                    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::SMOL_BALLOON, enemies[i]->GetPositionX() + (enemies[i]->GetCurrentAnimation()->GetWidth() / 2), enemies[i]->GetPositionY() + (enemies[i]->GetCurrentAnimation()->GetHeight() / 2), true);
                    game->GetModulePlayer()->AddScore(150);
                    break;
                }
                case ENEMY_TYPE::SMOL_BALLOON: {
                    if (game->GetModuleLevelOne()->IsEnabled()) { game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->rSmolBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY()); }
                    else if (game->GetModuleLevelTwo()->IsEnabled()) { game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->bSmolBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY()); }
                    else if (game->GetModuleLevelThree()->IsEnabled()) { game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->gSmolBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY()); }

                    game->GetModulePlayer()->AddScore(200);
                    break;
                }
                default: {break; }
                }

                delete enemies[i];
                enemies[i] = nullptr;
            }
            break;
        }
    }
}