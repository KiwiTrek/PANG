#include "ModuleEnemies.h"

#include "Game.h"

#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleParticles.h"
#include "ModuleAudio.h"

#include "Enemy.h"
#include "Enemy_Balloon.h"

#define SPAWN_MARGIN 50


ModuleEnemies::ModuleEnemies(bool startEnabled) : Module(startEnabled) {
    for (uint i = 0; i < MAX_ENEMIES; ++i) { enemies[i] = nullptr; }
}

ModuleEnemies::~ModuleEnemies() {}

bool ModuleEnemies::Start() {
    texture = game->GetModuleTextures()->Load("Resources/Sprites/enemies.png");
    balloogiExplosioni = game->GetModuleAudio()->LoadFx("Resources/SFX/balloonPopped.wav");

    return true;
}

UPDATE_STATUS ModuleEnemies::Update() {
    HandleEnemiesSpawn();

    for (uint i = 0; i < MAX_ENEMIES; ++i) {
        if (enemies[i] != nullptr)
            enemies[i]->Update();
    }

   //HandleEnemiesDespawn();

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleEnemies::PostUpdate() {
    for (uint i = 0; i < MAX_ENEMIES; ++i)
    {
        if (enemies[i] != nullptr)
            enemies[i]->Draw();
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

    return true;
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPE type, int x, int y) {
    for (uint i = 0; i < MAX_ENEMIES; ++i) {
        if (spawnQueue[i].type == ENEMY_TYPE::NO_TYPE) {
            spawnQueue[i].type = type;
            spawnQueue[i].x = x;
            spawnQueue[i].y = y;
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

void ModuleEnemies::HandleEnemiesDespawn() {
    // Iterate existing enemies
    for (uint i = 0; i < MAX_ENEMIES; ++i)
    {
    }
}

void ModuleEnemies::SpawnEnemy(const EnemySpawnpoint& info) {
    // Find an empty slot in the enemies array
    for (uint i = 0; i < MAX_ENEMIES; ++i)
    {
        if (enemies[i] == nullptr)
        {
            switch (info.type)
            {
            case ENEMY_TYPE::CHUNGUS_BALLOON:
                enemies[i] = new Enemy_Balloon(info.x, info.y, info.type);
                break;
            case ENEMY_TYPE::NOT_THAT_MEH_BALLOON:
                enemies[i] = new Enemy_Balloon(info.x, info.y, info.type);
                break;
            case ENEMY_TYPE::MEH_BALLOON:
                enemies[i] = new Enemy_Balloon(info.x, info.y, info.type);
                break;
            case ENEMY_TYPE::SMOL_BALLOON:
                enemies[i] = new Enemy_Balloon(info.x, info.y, info.type);
                break;
            }
            enemies[i]->SetEnemyTexture(texture);
            enemies[i]->SetDestroyedFx(balloogiExplosioni);
            break;
        }
    }
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2) {
    for (uint i = 0; i < MAX_ENEMIES; ++i) {
        if (enemies[i]->GetCollider() == c1 && enemies[i] != nullptr) {
            enemies[i]->OnCollision(c2); //Notify the enemy of a collision
            if (enemies[i]->GetLethality()) {
                if (c1->GetType() == Collider::TYPE::BALLOON) { game->GetModuleAudio()->PlayFx(enemies[i]->GetDestroyedFx()); }
                if (enemies[i]->GetEnemyType() == ENEMY_TYPE::CHUNGUS_BALLOON) {
                    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->bigBalloonExplosion, enemies[i]->GetPositionX(), enemies[i]->GetPositionY());
                    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::NOT_THAT_MEH_BALLOON, 147, 36);
                    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::NOT_THAT_MEH_BALLOON, 207, 36);
                }
                else if(enemies[i]->GetEnemyType() == ENEMY_TYPE::NOT_THAT_MEH_BALLOON){ // Needs explosion effect
                    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::MEH_BALLOON, 147, 36);
                    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::MEH_BALLOON, 207, 36);
                }
                else if (enemies[i]->GetEnemyType() == ENEMY_TYPE::MEH_BALLOON) { // Needs explosion effect
                    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::SMOL_BALLOON, 147, 36);
                    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::SMOL_BALLOON, 207, 36);
                }
                else if (enemies[i]->GetEnemyType() == ENEMY_TYPE::SMOL_BALLOON) {} // Needs explosion effect
                delete enemies[i];
                enemies[i] = nullptr;
            }
            break;
        }
    }
}

