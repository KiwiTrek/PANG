#include "ModuleLevelOne.h"
#include "Game.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleCollisions.h"
#include "ModuleEnemies.h"
#include "ModuleRender.h"

ModuleLevelOne::ModuleLevelOne(bool startEnabled) : Module(startEnabled) {
    // Background
    background = { 8, 8, 384, 207 };
    backgroundAdapter = { 0, 0, 384, 193 };
}

ModuleLevelOne::~ModuleLevelOne() {}

// Load assets
bool ModuleLevelOne::Start() {
    LOG("Loading background assets");

    backgroundTexture = game->GetModuleTextures()->Load("Resources/Sprites/backgrounds.png");
    game->GetModuleAudio()->PlayMusic("Resources/BGM/introFuji.ogg");

    // Colliders ---
    game->GetModuleCollisions()->AddCollider({ 0, 186, 384, 7 }, Collider::TYPE::FLOOR);
    game->GetModuleCollisions()->AddCollider({ 0, 0, 8, 193 }, Collider::TYPE::WALL);
    game->GetModuleCollisions()->AddCollider({ 0, 0, 384, 7 }, Collider::TYPE::FLOOR);
    game->GetModuleCollisions()->AddCollider({ 376, 0, 8, 193 }, Collider::TYPE::WALL);

    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::CHUNGUS_BALLOON, 177, 36);
    return true;
}

UPDATE_STATUS ModuleLevelOne::Update() {
    game->GetModuleAudio()->DetectIntroEnd("Resources/BGM/fuji.ogg", 467);
    return UPDATE_STATUS::UPDATE_CONTINUE;
}


// Update: draw background
UPDATE_STATUS ModuleLevelOne::PostUpdate() {
    // Draw everything --------------------------------------
    game->GetModuleRender()->Blit(backgroundTexture, 0, 0, GetInvertValue(), &background, &backgroundAdapter);
    return UPDATE_STATUS::UPDATE_CONTINUE;
}