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
}

ModuleWinScreen::~ModuleWinScreen() {}

// Load assets
bool ModuleWinScreen::Start() {
    LOG("Loading background assets");

    splashArtTexture = game->GetModuleTextures()->Load("Resources/Sprites/splashArts.png"); //Should be Group Project info
    game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/levelComplete.ogg");

    return true;
}

UPDATE_STATUS ModuleWinScreen::Update() {
    if (game->GetModuleAudio()->DetectIfEnd() == false) { game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleTitleScreen(), 4); }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleWinScreen::PostUpdate() {
    // Draw everything --------------------------------------
    //Edit arguments of Blit
    game->GetModuleRender()->Blit(splashArtTexture, 0, 0, false, &splashArt.GetCurrentFrame());
    
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleWinScreen::CleanUp() {
    game->GetModuleTextures()->Unload(splashArtTexture);
    return true;
}