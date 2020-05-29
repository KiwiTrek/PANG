#include "ModuleWinScreen.h"

#include "Game.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleLevelOne.h"
#include "ModuleTransition.h"
#include "ModulePlayer.h"
#include "ModuleFonts.h"

#include "SDL/include/SDL_scancode.h"
#include <stdio.h>

ModuleWinScreen::ModuleWinScreen(bool startEnabled) : Module(startEnabled) {
    //SplashArt Pushbacks
    for (int i = 0; i != 29; ++i) { splashArt1.PushBack({ i * 300,22,300,178 }); }
    splashArt1.SetSpeed(0.5f);
    splashArt1.SetLoop(true);

    for (int j = 0; j != 4; ++j) {
        for (int i = 0; i != 17; ++i) {
            planeTransition.PushBack({ i * 600,j * 450,600,450 });
        }
    }
    planeTransition.SetSpeed(0.5f);
    planeTransition.SetLoop(false);
}

ModuleWinScreen::~ModuleWinScreen() {}

// Load assets
bool ModuleWinScreen::Start() {
    LOG("Loading background assets");
    counter = 0;

	bonusScore = game->GetModulePlayer()->GetTimer() * 100;
    splashArt1.Reset();
    planeTransition.Reset();
    switch (currentLevel) {
    case 1: {
        splashArtTexture = game->GetModuleTextures()->Load("Resources/Sprites/Splasharts/FirstStageSplashArt.png"); 
        game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/levelComplete.ogg");
        break;
    }
    case 2: {
        splashArtTexture = game->GetModuleTextures()->Load("Resources/Sprites/Splasharts/SecondStageSplashArt.png");
        game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/levelComplete.ogg");
        break;
    }
    case 3: {
        splashArtTexture = game->GetModuleTextures()->Load("Resources/Sprites/Splasharts/PlaneTransition1.png");
        game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/plane.ogg");
        break;
    }
    default: {
        break;
    }
    }

	normalFont2 = game->GetModuleFonts()->Load("Resources/Sprites/Font.png", "a ',.0123456789:bcdefghijklmnopqrstuvwxyz-", 6);

    return true;
}

UPDATE_STATUS ModuleWinScreen::Update() {
    splashArt1.Update();
    planeTransition.Update();
    switch (currentLevel) {
    case 1: {
        game->GetModuleAudio()->ChangeModuleAtEnd("Resources/BGM/noMusic.ogg", (Module*)game->GetModuleLevelTwo());
        break;
    }
    case 2: {
        game->GetModuleAudio()->ChangeModuleAtEnd("Resources/BGM/noMusic.ogg", (Module*)game->GetModuleLevelThree());
        break;
    }
    case 3: {
        if (planeTransition.HasFinished()) {
            game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleTitleScreen(),4);
        }
        break;
    }
    default: {
        break;
    }
    }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleWinScreen::PostUpdate() {
    // Draw everything --------------------------------------
    ++counter;
    SDL_Rect backgroundRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    switch (currentLevel) {
    case 1: {
        SDL_Rect splashArtAdapter = { (game->GetModuleLevelOne()->GetBackgroundAdapter().w / 2) - 100,(game->GetModuleLevelOne()->GetBackgroundAdapter().y) + (TILE_SIZE * 4),200,95 };
        game->GetModuleRender()->Blit(splashArtTexture, 0, 0, false, &splashArt1.GetCurrentFrame(), &splashArtAdapter);
        if (counter >= 1) {
            sprintf_s(stage, 10, "1stage");
            game->GetModuleFonts()->BlitText(174, 142, normalFont2, stage);
            sprintf_s(timeBonus, 25, "time bonus    %5d pts.", bonusScore);
            game->GetModuleFonts()->BlitText(102, 166, normalFont2, timeBonus);
        }
        if (counter >= 51) {
            sprintf_s(nextExtend, 25, "next extend   %5d pts.", nextExtendNumber);
            game->GetModuleFonts()->BlitText(102, 182, normalFont2, nextExtend);
        }
        break;
    }
    case 2: {
        SDL_Rect splashArtAdapter = { (game->GetModuleLevelOne()->GetBackgroundAdapter().w / 2) - 100,(game->GetModuleLevelOne()->GetBackgroundAdapter().y) + (TILE_SIZE * 4),200,95 };
        game->GetModuleRender()->Blit(splashArtTexture, 0, 0, false, &staticSplashArt2, &splashArtAdapter);
        if (counter >= 1) {
            sprintf_s(stage, 10, "2stage");
            game->GetModuleFonts()->BlitText(174, 142, normalFont2, stage);
            sprintf_s(timeBonus, 25, "time bonus    %5d pts.", bonusScore);
            game->GetModuleFonts()->BlitText(102, 166, normalFont2, timeBonus);
        }
        if (counter >= 51) {
            sprintf_s(nextExtend, 25, "next extend   %5d pts.", nextExtendNumber);
            game->GetModuleFonts()->BlitText(102, 182, normalFont2, nextExtend);
        }
        break;
    }
    case 3: {
        game->GetModuleRender()->Blit(splashArtTexture, 0, 0, false, &planeTransition.GetCurrentFrame(), &backgroundRect);
    }
    default: {
        break;
    }
    }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleWinScreen::CleanUp() {
    game->GetModuleTextures()->Unload(splashArtTexture);
	game->GetModuleFonts()->Unload(normalFont2);
    return true;
}

int ModuleWinScreen::GetCurrentLevel() const { return currentLevel; };
void ModuleWinScreen::SetCurrentLevel(int _currentLevel) { currentLevel = _currentLevel; };