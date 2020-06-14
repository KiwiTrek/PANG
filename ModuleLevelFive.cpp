#include "ModuleLevelFive.h"

#include "Game.h"

#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleParticles.h"
#include "ModuleCollisions.h"
#include "ModulePlayer.h"
#include "ModuleEnemies.h"
#include "ModuleFonts.h"
#include "ModuleRender.h"
#include "ModuleTileset.h"
#include "ModuleWinScreen.h"

#include <stdio.h>

ModuleLevelFive::ModuleLevelFive(bool startEnabled) : Module(startEnabled) {
    // Background
    backgroundAdapter = { 0, 0, 384, 208 };
}

ModuleLevelFive::~ModuleLevelFive() {}

// Load assets
bool ModuleLevelFive::Start() {
    LOG("Loading background assets");
    once = true;
    hasStarted = false;

    game->GetModuleWinScreen()->SetCurrentLevel(5);

    game->GetModuleParticles()->Enable();

    for (int i = 0; i < 9; i++) {
        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->ready, (backgroundAdapter.w / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().w / 2), (backgroundAdapter.h / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().h / 2), Collider::TYPE::NONE, i * 5);
    }
    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->ready, (backgroundAdapter.w / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().w / 2), (backgroundAdapter.h / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().h / 2), Collider::TYPE::NONE, 50);
    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->ready, (backgroundAdapter.w / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().w / 2), (backgroundAdapter.h / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().h / 2), Collider::TYPE::NONE, 60);
    p2 = game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->ready, (backgroundAdapter.w / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().w / 2), (backgroundAdapter.h / 2) - (game->GetModuleParticles()->ready.GetCurrentAnim().h / 2), Collider::TYPE::NONE, 70);
    
    backgroundTexture = game->GetModuleTextures()->Load("Resources/Sprites/Backgrounds/Background5.png");

    // Colliders ---
    //game->GetModuleCollisions()->AddCollider({ 0, backgroundAdapter.h-TILE_SIZE, backgroundAdapter.w, TILE_SIZE }, Collider::TYPE::FLOOR); // {0,186,384,7} BOTTOM
    //game->GetModuleCollisions()->AddCollider({0, 0, TILE_SIZE , backgroundAdapter.h }, Collider::TYPE::WALL); // {0,0,8,193} LEFT
    game->GetModuleCollisions()->AddCollider({ 0, 0, backgroundAdapter.w, TILE_SIZE }, Collider::TYPE::FLOOR); // {0,0,384,7} TOP
    //game->GetModuleCollisions()->AddCollider({ backgroundAdapter.w-TILE_SIZE, 0, TILE_SIZE, backgroundAdapter.h }, Collider::TYPE::WALL); // {376,0,8,193} RIGHT

    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::CHUNGUS_BALLOON, 16, 16, true);
    game->GetModuleEnemies()->AddEnemy(ENEMY_TYPE::MEH_BALLOON, 200, 67, true);
    
    game->GetModulePlayer()->Enable();
    game->GetModuleEnemies()->Enable(); 
    game->GetModuleCollisions()->Enable();
    game->GetModuleTileset()->Enable();
    

    return true;
}

UPDATE_STATUS ModuleLevelFive::Update() {
    if (p2->GetFrameCount() >= 5) { SetIfStarted(true); }
    if (hasStarted) {
        if (once) {
            once = false;
            game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/introKeirin.ogg");
        }
        if (game->GetModulePlayer()->GetPlayerLives() >= 0 && game->GetModulePlayer()->GetTimer() >= 50) { game->GetModuleAudio()->ChangeAtEnd("Resources/BGM/keirin.ogg"); }
    }
    
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleLevelFive::PostUpdate() {
    // Draw everything --------------------------------------
    sprintf_s(levelTitle, 10, "mt.keirin");
    game->GetModuleFonts()->BlitText(TILE_SIZE * 20, backgroundAdapter.h, game->GetModulePlayer()->GetFontIndex(), levelTitle);
    sprintf_s(stageText, 10, "2-5 stage");
    game->GetModuleFonts()->BlitText(TILE_SIZE * 20, backgroundAdapter.h + (TILE_SIZE * 2), game->GetModulePlayer()->GetFontIndex(), stageText);
    sprintf_s(highScore, 15, "hi: 100000");
    game->GetModuleFonts()->BlitText(TILE_SIZE * 19, backgroundAdapter.h + (TILE_SIZE * 3), game->GetModulePlayer()->GetFontIndex(), highScore);
    game->GetModuleRender()->Blit(backgroundTexture, 0, 0, GetInvertValue(), NULL);
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleLevelFive::CleanUp() {
    game->GetModuleTextures()->Unload(backgroundTexture);
    game->GetModuleTextures()->Unload(game->GetModulePlayer()->GetTexture());
    game->GetModuleTextures()->Unload(game->GetModulePlayer()->GetBlueTextTexture());
    game->GetModuleTextures()->Unload(game->GetModulePlayer()->GetPowerIconTexture());
	game->GetModuleFonts()->Unload(game->GetModulePlayer()->GetFontIndex());
	game->GetModuleFonts()->Unload(game->GetModulePlayer()->GetTimerFontIndex());
    game->GetModuleAudio()->UnloadFx(game->GetModulePlayer()->GetNormalShotSoundIndex());
    game->GetModuleAudio()->UnloadFx(game->GetModulePlayer()->GetVulcanShotSoundIndex());
    game->GetModuleAudio()->UnloadFx(game->GetModulePlayer()->GetDedSoundIndex());

    game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/noMusic.ogg");

    game->GetModulePlayer()->Disable();
    game->GetModuleEnemies()->Disable();
    game->GetModuleParticles()->Disable();
    game->GetModuleCollisions()->Disable();
    game->GetModuleTileset()->Disable();
    
    hasStarted = false;
    return true;
}

bool ModuleLevelFive::CheckIfStarted() const { return hasStarted; }
void ModuleLevelFive::SetIfStarted(bool _hasStarted) { hasStarted = _hasStarted; }
SDL_Rect ModuleLevelFive::GetBackgroundAdapter() const { return backgroundAdapter; }