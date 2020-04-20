#include "ModuleProjectSheet.h"

#include "Game.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleTransition.h"

#include "SDL/include/SDL_scancode.h"

ModuleProjectSheet::ModuleProjectSheet(bool startEnabled) : Module(startEnabled) {}

ModuleProjectSheet::~ModuleProjectSheet() {}

// Load assets
bool ModuleProjectSheet::Start() {
    LOG("Loading background assets");

    projectSheet = game->GetModuleTextures()->Load("Resources/Sprites/projectSheet.png");

    return true;
}

UPDATE_STATUS ModuleProjectSheet::Update() {
    if (game->GetModuleInput()->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_DOWN) {game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleTitleScreen(), 45);}
    if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F8) == KEY_STATE::KEY_DOWN) { game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleLevelOne(), 4); }
    if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F9) == KEY_STATE::KEY_DOWN) { game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleWinScreen(), 4); }

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleProjectSheet::PostUpdate() {
    // Draw everything --------------------------------------
    SDL_Rect backgroundRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    game->GetModuleRender()->Blit(projectSheet, 0, 0, false, nullptr, &backgroundRect);

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleProjectSheet::CleanUp() {
    game->GetModuleTextures()->Unload(projectSheet);
    return true;
}