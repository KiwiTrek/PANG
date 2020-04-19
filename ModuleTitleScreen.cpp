#include "ModuleTitleScreen.h"

#include "Game.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleTransition.h"

#include "SDL/include/SDL_scancode.h"

ModuleTitleScreen::ModuleTitleScreen(bool startEnabled) : Module(startEnabled) {
    for (int i = 0; i != 2; ++i) {
        for (int j = 0; j != 32; ++j) { titleScreenAnimation.PushBack({ j * 450,i * 338,450,338 }); }
    }
    titleScreenAnimation.SetLoop(false);
    titleScreenAnimation.SetSpeed(0.5f);
}

ModuleTitleScreen::~ModuleTitleScreen() {}

// Load assets
bool ModuleTitleScreen::Start() {
    LOG("Loading background assets");
    once = true;

    backgroundTexture = game->GetModuleTextures()->Load("Resources/Sprites/pangTitleCard.png");
    titleScreenAnimationTexture = game->GetModuleTextures()->Load("Resources/Sprites/pangAnimationTitleScreen.png");
    titleScreenAnimationSFX = game->GetModuleAudio()->LoadFx("Resources/SFX/titleScreenAnimationSound.wav");
    creditSFX = game->GetModuleAudio()->LoadFx("Resources/SFX/credit.wav");

    return true;
}

UPDATE_STATUS ModuleTitleScreen::Update() {
    if (game->GetModuleInput()->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_DOWN) {
        game->GetModuleAudio()->PlayFx(creditSFX);
        game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleLevelOne(), 4);
    }
    if (once) {
        once = false;
        titleScreenAnimation.Reset();
        game->GetModuleAudio()->PlayFx(titleScreenAnimationSFX);
    }
    titleScreenAnimation.Update();

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleTitleScreen::PostUpdate() {
    // Draw everything --------------------------------------
    SDL_Rect backgroundRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

    if (titleScreenAnimation.HasFinished()) { game->GetModuleRender()->Blit(backgroundTexture, 0, 0, false, nullptr, &backgroundRect); }
    else {game->GetModuleRender()->Blit(titleScreenAnimationTexture, 0, 0, false, &titleScreenAnimation.GetCurrentFrame(), &backgroundRect);}

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleTitleScreen::CleanUp() {
    game->GetModuleTextures()->Unload(backgroundTexture);
    game->GetModuleTextures()->Unload(titleScreenAnimationTexture);
    return true;
}