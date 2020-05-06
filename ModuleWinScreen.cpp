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
    for (int i = 0; i != 29; ++i) { splashArt.PushBack({ i * 300,22,300,178 }); }
    splashArt.SetSpeed(0.5f);
    splashArt.SetLoop(true);
}

ModuleWinScreen::~ModuleWinScreen() {}

// Load assets
bool ModuleWinScreen::Start() {
    LOG("Loading background assets");
    counter = 0;

	bonusScore = game->GetModulePlayer()->GetTimer() * 100;
    splashArt.Reset();
    splashArtTexture = game->GetModuleTextures()->Load("Resources/Sprites/FirstStageSplashArt.png");
    game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/levelComplete.ogg");
	normalFont2 = game->GetModuleFonts()->Load("Resources/Sprites/Font.png", "a ',.0123456789:bcdefghijklmnopqrstuvwxyz-", 6);


    return true;
}

UPDATE_STATUS ModuleWinScreen::Update() {
    splashArt.Update();
    game->GetModuleAudio()->ChangeModuleAtEnd("Resources/BGM/noMusic.ogg");

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleWinScreen::PostUpdate() {
    // Draw everything --------------------------------------
    ++counter;
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

    //Edit arguments of Blit
    SDL_Rect splashArtAdapter = { (game->GetModuleLevelOne()->GetBackgroundAdapter().w / 2) - 100,(game->GetModuleLevelOne()->GetBackgroundAdapter().y)+(TILE_SIZE*4),200,95 };
    game->GetModuleRender()->Blit(splashArtTexture,0,0,false,&splashArt.GetCurrentFrame(),&splashArtAdapter);
    
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleWinScreen::CleanUp() {
    game->GetModuleTextures()->Unload(splashArtTexture);
	game->GetModuleFonts()->Unload(normalFont2);
    return true;
}