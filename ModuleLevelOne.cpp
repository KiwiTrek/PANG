#include "ModuleLevelOne.h"
#include "Game.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

ModuleLevelOne::ModuleLevelOne() {
	// Background
	background = { 8, 8, 383, 207 };
	backgroundAdapter = { 0, 0, 383, 185 }; //Picosituación if you feel like it
}

ModuleLevelOne::~ModuleLevelOne() {}

// Load assets
bool ModuleLevelOne::Start() {
	LOG("Loading background assets");

	backgroundTexture = game->textures->Load("Resources/Backgrounds.png");

	return true;
}


// Update: draw background
UPDATE_STATUS ModuleLevelOne::PostUpdate() {
	// Draw everything --------------------------------------
	game->render->Blit(backgroundTexture, 0, 0, &background, &backgroundAdapter);
	return UPDATE_STATUS::UPDATE_CONTINUE;
}