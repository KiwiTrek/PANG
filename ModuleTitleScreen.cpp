#include "ModuleTitleScreen.h"

#include "Game.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleTransition.h"

#include "SDL/include/SDL_scancode.h"

ModuleTitleScreen::ModuleTitleScreen(bool startEnabled) : Module(startEnabled) {}

ModuleTitleScreen::~ModuleTitleScreen() {}

// Load assets
bool ModuleTitleScreen::Start() {
	LOG("Loading background assets");

	projectSheet = game->GetModuleTextures()->Load("Resources/Sprites/test.png"); //Should be Group Project info
	backgroundTexture = game->GetModuleTextures()->Load("Resources/Sprites/pangTitleCard.png");

	return true;
}

UPDATE_STATUS ModuleTitleScreen::Update() {
	if (game->GetModuleInput()->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_DOWN) {
		if (!title) { title = true; }
		else { game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleLevelOne(), 2); }
	}

	return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleTitleScreen::PostUpdate() {
	// Draw everything --------------------------------------
	if (!title) { game->GetModuleRender()->Blit(projectSheet, 0, 0, NULL); }
	else { game->GetModuleRender()->Blit(backgroundTexture, 0, 0, NULL); }

	return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleTitleScreen::CleanUp() {
	game->GetModuleTextures()->Unload(projectSheet);
	game->GetModuleTextures()->Unload(backgroundTexture);
	return true;
}