#include "ModuleProjectSheet.h"

#include "Game.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleTransition.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "ModuleWinScreen.h"

#include "SDL/include/SDL_scancode.h"
#include <stdio.h>

ModuleProjectSheet::ModuleProjectSheet(bool startEnabled) : Module(startEnabled) {}

ModuleProjectSheet::~ModuleProjectSheet() {}

// Load assets
bool ModuleProjectSheet::Start() {
    LOG("Loading background assets");

    game->GetModulePlayer()->SetPlayerLives(2);
    game->GetModulePlayer()->SetScore(0);
    game->GetModuleWinScreen()->SetCurrentLevel(0);

    projectSheet = game->GetModuleTextures()->Load("Resources/Sprites/projectSheet.png");
    levelBackground = game->GetModuleTextures()->Load("Resources/Sprites/test.png");
    levelFont = game->GetModuleFonts()->Load("Resources/Sprites/Font.png", "a ',.0123456789:bcdefghijklmnopqrstuvwxyz-", 6);

    return true;
}

UPDATE_STATUS ModuleProjectSheet::Update() {
    GamePad& pad = game->GetModuleInput()->GetGamePad(0);
    if (game->GetModuleInput()->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_DOWN || pad.start) {game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleTitleScreen(), 45);}
    if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F1) == KEY_STATE::KEY_DOWN) { levelSelector = !levelSelector; }
    if (levelSelector) {
        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_1)) { game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleLevelOne(), 4); }
        else if (game->GetModuleInput()->GetKey(SDL_SCANCODE_2)) { game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleLevelTwo(), 4); }
        else if (game->GetModuleInput()->GetKey(SDL_SCANCODE_3)) { game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleLevelThree(), 4); }
    }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleProjectSheet::PostUpdate() {
    // Draw everything --------------------------------------
    SDL_Rect backgroundRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    if (levelSelector) {
        game->GetModuleRender()->Blit(levelBackground, 0, 0, false, NULL, &backgroundRect);
        sprintf_s(levelTitle, 15, "select level:");
        game->GetModuleFonts()->BlitText(TILE_SIZE * 3, TILE_SIZE * 5, levelFont, levelTitle);
        sprintf_s(stage1, 20, "level 1: press 1");
        game->GetModuleFonts()->BlitText(TILE_SIZE * 3, TILE_SIZE * 7, levelFont, stage1);
        sprintf_s(stage2, 20, "level 2: press 2");
        game->GetModuleFonts()->BlitText(TILE_SIZE * 3, TILE_SIZE * 9, levelFont, stage2);
        sprintf_s(stage3, 20, "level 3: press 3");
        game->GetModuleFonts()->BlitText(TILE_SIZE * 3, TILE_SIZE * 11, levelFont, stage3);
    }
    else { game->GetModuleRender()->Blit(projectSheet, 0, 0, false, nullptr, &backgroundRect); }

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleProjectSheet::CleanUp() {
    game->GetModuleTextures()->Unload(projectSheet);
    game->GetModuleTextures()->Unload(levelBackground);
    game->GetModuleFonts()->Unload(levelFont);

    return true;
}