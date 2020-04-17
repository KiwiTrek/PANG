#include "ModuleTitleScreen.h"

#include "Game.h"
#include "ModuleWindow.h"
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

	backgroundTexture = game->GetModuleTextures()->Load("Resources/Sprites/pangTitleCard.png");

	return true;
}

UPDATE_STATUS ModuleTitleScreen::Update() {
	if (game->GetModuleInput()->GetKey(SDL_SCANCODE_SPACE) == KEY_STATE::KEY_DOWN) {
		game->GetModuleTransition()->Transition(this, (Module*)game->GetModuleLevelOne(), 4);
	}

	return UPDATE_STATUS::UPDATE_CONTINUE;
}

// Update: draw background
UPDATE_STATUS ModuleTitleScreen::PostUpdate() {
	// Draw everything --------------------------------------
	SDL_Rect backgroundRect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	game->GetModuleRender()->Blit(backgroundTexture, 0, 0, false, nullptr, &backgroundRect);

	return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleTitleScreen::CleanUp() {
	game->GetModuleTextures()->Unload(backgroundTexture);
	return true;
}