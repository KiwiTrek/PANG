#include "ModuleWinScreen.h"

#include "Game.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleTransition.h"

#include "SDL/include/SDL_scancode.h"

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
    once = true;

    splashArtTexture = game->GetModuleTextures()->Load("Resources/Sprites/FirstStageSplashArt.png");
    game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/levelComplete.ogg");

    return true;
}

UPDATE_STATUS ModuleWinScreen::Update() {
    if (once) {
        once = false;
        splashArt.Reset();
    }
    splashArt.Update();

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleWinScreen::PostUpdate() {
    // Draw everything --------------------------------------
    //Edit arguments of Blit
    SDL_Rect backgroundAdapter = { 0, 0, 384, 193 };
    SDL_Rect splashArtAdapter = { (SCREEN_WIDTH / 2 + 150),backgroundAdapter.h-100,150,85 };
    game->GetModuleRender()->Blit(splashArtTexture,0,0,false,&splashArt.GetCurrentFrame(),&splashArtAdapter);
    
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleWinScreen::CleanUp() {
    game->GetModuleTextures()->Unload(splashArtTexture);
    return true;
}