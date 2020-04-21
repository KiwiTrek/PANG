#include "ModuleTitleScreen.h"

#include "Game.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModulePlayer.h"
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
    counter = 0;
    switchOn = true;

    titleScreenAnimation.Reset();
    game->GetModulePlayer()->SetPlayerLives(2);

    backgroundTexture = game->GetModuleTextures()->Load("Resources/Sprites/pangTitleCard.png");
    insertCoinTexture = game->GetModuleTextures()->Load("Resources/Sprites/insertCoin.png");
    game->GetModuleTextures()->GetTextureSize(insertCoinTexture, insertCoin_w, insertCoin_h);
    titleScreenAnimationTexture = game->GetModuleTextures()->Load("Resources/Sprites/pangAnimationTitleScreen.png");
    titleScreenAnimationSFX = game->GetModuleAudio()->LoadFx("Resources/SFX/titleScreenAnimationSound.wav");
    creditSFX = game->GetModuleAudio()->LoadFx("Resources/SFX/credit.wav");

    return true;
}

UPDATE_STATUS ModuleTitleScreen::Update() {
    ++counter;
    if (once) {
        once = false;
        game->GetModuleAudio()->PlayFx(titleScreenAnimationSFX);
    }
    if (game->GetModuleInput()->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_DOWN && titleScreenAnimation.HasFinished()) {
        game->GetModuleAudio()->PlayFx(creditSFX);
        game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleLevelOne(), 4);
    }
    if (counter == 30){
        counter = 0;
        switchOn = !switchOn;
    }
    titleScreenAnimation.Update();

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleTitleScreen::PostUpdate() {
    // Draw everything --------------------------------------
    SDL_Rect backgroundRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

    if (titleScreenAnimation.HasFinished()) {
        game->GetModuleRender()->Blit(backgroundTexture, 0, 0, false, nullptr, &backgroundRect);
        if (switchOn) { game->GetModuleRender()->Blit(insertCoinTexture, ((SCREEN_WIDTH / 2) - (insertCoin_w / 2) - 5), 192, false); }
    }
    else {game->GetModuleRender()->Blit(titleScreenAnimationTexture, 0, 0, false, &titleScreenAnimation.GetCurrentFrame(), &backgroundRect);}

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleTitleScreen::CleanUp() {
    game->GetModuleTextures()->Unload(backgroundTexture);
    game->GetModuleTextures()->Unload(titleScreenAnimationTexture);
    game->GetModuleAudio()->UnloadFx(titleScreenAnimationSFX);
    //game->GetModuleAudio()->UnloadFx(creditSFX);

    return true;
}