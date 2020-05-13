#include "Game.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"

#include "ModuleProjectSheet.h"
#include "ModuleTitleScreen.h"
#include "ModuleTileset.h"
#include "ModuleLevelOne.h"
#include "ModulePlayer.h"
#include "ModuleEnemies.h"
#include "ModuleParticles.h"
#include "ModuleWinScreen.h"

#include "ModuleCollisions.h"

#include "ModuleTransition.h"

#include "ModuleFonts.h"
#include "ModuleRender.h"


Game::Game() {
    // The order in which the modules are added is very important.
    // It will define the order in which Pre/Update/Post will be called
    // Render should always be last, as our last action should be updating the screen
    modules[0] = window = new ModuleWindow(true);
    modules[1] = input = new ModuleInput(true);
    modules[2] = textures = new ModuleTextures(true);
    modules[3] = audio = new ModuleAudio(true);

    modules[4] = projectSheet = new ModuleProjectSheet(true);
    modules[5] = titleScreen = new ModuleTitleScreen(false);
    modules[6] = levelOne = new ModuleLevelOne(false);
    modules[7] = tileset = new ModuleTileset(false);
    modules[8] = enemies = new ModuleEnemies(false);
    modules[9] = player = new ModulePlayer(false);
    modules[10] = particles = new ModuleParticles(false);
    modules[11] = winScreen = new ModuleWinScreen(false);

    modules[12] = collisions = new ModuleCollisions(false);
    modules[13] = transition = new ModuleTransition(true);
    modules[14] = fonts = new ModuleFonts(true);
    modules[15] = render = new ModuleRender(true);
}

Game::~Game() {
    for (int i = 0; i < NUM_MODULES; ++i) {
        //Important: when deleting a pointer, set it to nullptr afterwards
        //It allows us for null check in other parts of the code
        delete modules[i];
        modules[i] = nullptr;
    }
}

bool Game::Init() {
    bool ret = true;

    for (int i = 0; i < NUM_MODULES && ret; ++i) { ret = modules[i]->Init(); }
    for (int i = 0; i < NUM_MODULES && ret; ++i) {
        if (modules[i]->IsEnabled()) { ret = modules[i]->Start(); }
        else { ret = true; }
    }

    return ret;
}

UPDATE_STATUS Game::Update() {
    UPDATE_STATUS ret = UPDATE_STATUS::UPDATE_CONTINUE;

    for (int i = 0; i < NUM_MODULES && ret == UPDATE_STATUS::UPDATE_CONTINUE; ++i) {
        if (modules[i]->IsEnabled()) { ret = modules[i]->PreUpdate(); }
        else { ret = UPDATE_STATUS::UPDATE_CONTINUE; }
    }
    for (int i = 0; i < NUM_MODULES && ret == UPDATE_STATUS::UPDATE_CONTINUE; ++i) {
        if (modules[i]->IsEnabled()) { ret = modules[i]->Update(); }
        else { ret = UPDATE_STATUS::UPDATE_CONTINUE; }
    }
    for (int i = 0; i < NUM_MODULES && ret == UPDATE_STATUS::UPDATE_CONTINUE; ++i) {
        if (modules[i]->IsEnabled()) { ret = modules[i]->PostUpdate(); }
        else { ret = UPDATE_STATUS::UPDATE_CONTINUE; }
    }

    return ret;
}

bool Game::CleanUp() {
    bool ret = true;

    for (int i = NUM_MODULES - 1; i >= 0 && ret; --i) {
        if (modules[i]->IsEnabled()) { ret = modules[i]->CleanUp(); }
        else { ret = true; }
    }

    return ret;
}

ModuleWindow* Game::GetModuleWindow() const { return window; }
ModuleInput* Game::GetModuleInput() const { return input; }
ModuleTextures* Game::GetModuleTextures() const { return textures; }
ModuleAudio* Game::GetModuleAudio() const { return audio; }
ModuleProjectSheet* Game::GetModuleProjectSheet() const { return projectSheet; }
ModuleTitleScreen* Game::GetModuleTitleScreen() const { return titleScreen; }
ModuleLevelOne* Game::GetModuleLevelOne() const { return levelOne; }
ModulePlayer* Game::GetModulePlayer() const { return player; }
ModuleEnemies* Game::GetModuleEnemies() const { return enemies; }
ModuleParticles* Game::GetModuleParticles() const { return particles; }
ModuleWinScreen* Game::GetModuleWinScreen() const { return winScreen; }
ModuleCollisions* Game::GetModuleCollisions() const { return collisions; }
ModuleRender* Game::GetModuleRender() const { return render; }
ModuleTransition* Game::GetModuleTransition() const { return transition; }
ModuleFonts* Game::GetModuleFonts() const { return fonts; }
ModuleTileset* Game::GetModuleTileset() const { return tileset; }