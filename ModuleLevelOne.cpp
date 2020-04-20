#include "ModuleLevelOne.h"

#include "Game.h"

#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleParticles.h"
#include "ModuleCollisions.h"
#include "ModulePlayer.h"
#include "ModuleEnemies.h"
#include "ModuleFonts.h"
#include "ModuleRender.h"

#include <stdio.h>

ModuleLevelOne::ModuleLevelOne(bool startEnabled) : Module(startEnabled) {
    // Background
    background = { 8, 8, 384, 208 };
    backgroundAdapter = { 0, 0, 384, 194 };
}

ModuleLevelOne::~ModuleLevelOne() {}

// Load assets
bool ModuleLevelOne::Start() {
    LOG("Loading background assets");
    once = true;
    hasStarted = false;

    game->GetModuleParticles()->Enable();

    for (int i = 0; i < 9; i++) {
        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->ready, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, Collider::TYPE::NONE, i*5);
    }
    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->ready, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, Collider::TYPE::NONE, 50);
    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->ready, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, Collider::TYPE::NONE, 60);
    p = game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->ready, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, Collider::TYPE::NONE, 70);
    
    backgroundTexture = game->GetModuleTextures()->Load("Resources/Sprites/backgrounds.png");

    // Colliders ---
    game->GetModuleCollisions()->AddCollider({ 0, 186, 384, 100 }, Collider::TYPE::FLOOR); // {0,186,384,7} BOTTOM
    game->GetModuleCollisions()->AddCollider({ -92, 0, 100 , 193 }, Collider::TYPE::WALL); // {0,0,8,193} LEFT
    game->GetModuleCollisions()->AddCollider({ 0, -93, 384, 100 }, Collider::TYPE::FLOOR); // {0,0,384,7} TOP
    game->GetModuleCollisions()->AddCollider({ 376, 0, 100, 193 }, Collider::TYPE::WALL); // {376,0,8,193} RIGHT

    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::CHUNGUS_BALLOON, 177, 36);
    
    game->GetModulePlayer()->Enable();
    game->GetModuleEnemies()->Enable(); 
    game->GetModuleCollisions()->Enable();
    

    return true;
}

UPDATE_STATUS ModuleLevelOne::Update() {
    if (p->GetFrameCount() >= 5) { SetIfStarted(true); }
    if (hasStarted) {
        if (once) {
            once = false;
            game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/introFuji.ogg");
        }
        if (!game->GetModulePlayer()->CheckIfDestroyed()) { game->GetModuleAudio()->ChangeAtEnd("Resources/BGM/fuji.ogg"); }
    }
    
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleLevelOne::PostUpdate() {
    // Draw everything --------------------------------------
    sprintf_s(levelTitle, 10, "mt.fuji");
    game->GetModuleFonts()->BlitText(160, 194, game->GetModulePlayer()->GetFontIndex(), levelTitle);
    sprintf_s(stageText, 10, "1-1 stage");
    game->GetModuleFonts()->BlitText(159, 209, game->GetModulePlayer()->GetFontIndex(), stageText);
    sprintf_s(highScore, 15, "hi: 100000");
    game->GetModuleFonts()->BlitText(152, 217, game->GetModulePlayer()->GetFontIndex(), highScore);
    game->GetModuleRender()->Blit(backgroundTexture, 0, 0, GetInvertValue(), &background, &backgroundAdapter);
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleLevelOne::CleanUp() {
    game->GetModuleTextures()->Unload(backgroundTexture);
    game->GetModuleTextures()->Unload(game->GetModulePlayer()->GetTexture());
    game->GetModuleTextures()->Unload(game->GetModulePlayer()->GetBlueTextTexture());
    game->GetModuleAudio()->UnloadFx(game->GetModulePlayer()->GetShotSoundIndex());
    game->GetModuleAudio()->UnloadFx(game->GetModulePlayer()->GetDedSoundIndex());
    game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/noMusic.ogg");
    game->GetModulePlayer()->Disable();
    game->GetModuleEnemies()->Disable();
    game->GetModuleParticles()->Disable();
    game->GetModuleCollisions()->Disable();
    
    return true;
}

bool ModuleLevelOne::CheckIfStarted() const { return hasStarted; }
void ModuleLevelOne::SetIfStarted(bool _hasStarted) { hasStarted = _hasStarted; }

