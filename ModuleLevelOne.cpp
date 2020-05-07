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
    //background = { 8, 8, 384, 208 };
    backgroundAdapter = { 0, 0, 384, 208 };
}

ModuleLevelOne::~ModuleLevelOne() {}

// Load assets
bool ModuleLevelOne::Start() {
    LOG("Loading background assets");
    once = true;
    hasStarted = false;

    game->GetModuleParticles()->Enable();

    for (int i = 0; i < 9; i++) {
        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->ready, (backgroundAdapter.w / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().w / 2), (backgroundAdapter.h / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().h / 2), Collider::TYPE::NONE, i * 5);
    }
    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->ready, (backgroundAdapter.w / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().w / 2), (backgroundAdapter.h / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().h / 2), Collider::TYPE::NONE, 50);
    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->ready, (backgroundAdapter.w / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().w / 2), (backgroundAdapter.h / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().h / 2), Collider::TYPE::NONE, 60);
    p = game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->ready, (backgroundAdapter.w / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().w / 2), (backgroundAdapter.h / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().h / 2), Collider::TYPE::NONE, 70);
    
    backgroundTexture = game->GetModuleTextures()->Load("Resources/Sprites/Backgrounds/Background1.png");

    // Colliders ---
    game->GetModuleCollisions()->AddCollider({ 0, backgroundAdapter.h-TILE_SIZE, backgroundAdapter.w, TILE_SIZE }, Collider::TYPE::FLOOR); // {0,186,384,7} BOTTOM
    game->GetModuleCollisions()->AddCollider({0, 0, TILE_SIZE , backgroundAdapter.h }, Collider::TYPE::WALL); // {0,0,8,193} LEFT
    game->GetModuleCollisions()->AddCollider({ 0, 0, backgroundAdapter.w, TILE_SIZE }, Collider::TYPE::FLOOR); // {0,0,384,7} TOP
    game->GetModuleCollisions()->AddCollider({ backgroundAdapter.w-TILE_SIZE, 0, TILE_SIZE, backgroundAdapter.h }, Collider::TYPE::WALL); // {376,0,8,193} RIGHT

    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::CHUNGUS_BALLOON, 36, 16, true);
    
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
        if (!game->GetModulePlayer()->CheckIfDestroyed() && game->GetModulePlayer()->GetTimer() >= 50) { game->GetModuleAudio()->ChangeAtEnd("Resources/BGM/fuji.ogg"); }
    }
    
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleLevelOne::PostUpdate() {
    // Draw everything --------------------------------------
    sprintf_s(levelTitle, 10, "mt.fuji");
    game->GetModuleFonts()->BlitText(TILE_SIZE * 20, backgroundAdapter.h, game->GetModulePlayer()->GetFontIndex(), levelTitle);
    sprintf_s(stageText, 10, "1-1 stage");
    game->GetModuleFonts()->BlitText(TILE_SIZE * 20, backgroundAdapter.h + (TILE_SIZE * 2), game->GetModulePlayer()->GetFontIndex(), stageText);
    sprintf_s(highScore, 15, "hi: 100000");
    game->GetModuleFonts()->BlitText(TILE_SIZE * 19, backgroundAdapter.h + (TILE_SIZE * 3), game->GetModulePlayer()->GetFontIndex(), highScore);
    game->GetModuleRender()->Blit(backgroundTexture, 0, 0, GetInvertValue(), NULL);
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleLevelOne::CleanUp() {
    game->GetModuleTextures()->Unload(backgroundTexture);
    game->GetModuleTextures()->Unload(game->GetModulePlayer()->GetTexture());
    game->GetModuleTextures()->Unload(game->GetModulePlayer()->GetBlueTextTexture());
	game->GetModuleFonts()->Unload(game->GetModulePlayer()->GetFontIndex());
	game->GetModuleFonts()->Unload(game->GetModulePlayer()->GetTimerFontIndex());
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
SDL_Rect ModuleLevelOne::GetBackgroundAdapter() const { return backgroundAdapter; };

