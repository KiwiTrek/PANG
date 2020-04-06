#include "ModuleLevelOne.h"
#include "Game.h"
#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

ModuleLevelOne::ModuleLevelOne() {
    // Background
    background = { 8, 8, 384, 207 };
    backgroundAdapter = { 0, 0, 384, 193 };
}

ModuleLevelOne::~ModuleLevelOne() {}

// Load assets
bool ModuleLevelOne::Start() {
    LOG("Loading background assets");

    backgroundTexture = game->textures->Load("Resources/Sprites/Backgrounds.png");
    game->audio->PlayMusic("Resources/BGM/introFuji.ogg");
    return true;
}

UPDATE_STATUS ModuleLevelOne::Update() {
    game->audio->DetectIntroEnd("Resources/BGM/fuji.ogg", 472);
    return UPDATE_STATUS::UPDATE_CONTINUE;
}


// Update: draw background
UPDATE_STATUS ModuleLevelOne::PostUpdate() {
    // Draw everything --------------------------------------
    game->render->Blit(backgroundTexture, 0, 0, GetInvertValue(), &background, &backgroundAdapter);
    return UPDATE_STATUS::UPDATE_CONTINUE;
}