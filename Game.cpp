#include "Game.h"

#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModuleLevelOne.h"
#include "ModuleParticles.h"
#include "ModuleEnemies.h"
#include "ModuleCollisions.h"
#include "ModuleRender.h"
#include "ModuleTransition.h"

Game::Game() {
    // The order in which the modules are added is very important.
    // It will define the order in which Pre/Update/Post will be called
    // Render should always be last, as our last action should be updating the screen
    modules[0] = window = new ModuleWindow(true);
    modules[1] = input = new ModuleInput(true);
    modules[2] = textures = new ModuleTextures(true);
    modules[3] = audio = new ModuleAudio(true);
    
    modules[4] = levelOne = new ModuleLevelOne(true);
    modules[5] = player = new ModulePlayer(true);
    modules[6] = enemies = new ModuleEnemies(true);
    modules[7] = particles = new ModuleParticles(true);

    modules[8] = collisions = new ModuleCollisions(true);

	modules[9] = transition = new ModuleTransition(true);

    modules[10] = render = new ModuleRender(true);
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
    //By now we will consider that all modules are always active
    for (int i = 0; i < NUM_MODULES && ret; ++i) { ret = modules[i]->Start(); }

    return ret;
}

UPDATE_STATUS Game::Update() {
    UPDATE_STATUS ret = UPDATE_STATUS::UPDATE_CONTINUE;

    for (int i = 0; i < NUM_MODULES && ret == UPDATE_STATUS::UPDATE_CONTINUE; ++i) { ret = modules[i]->PreUpdate(); }
    for (int i = 0; i < NUM_MODULES && ret == UPDATE_STATUS::UPDATE_CONTINUE; ++i) { ret = modules[i]->Update(); }
    for (int i = 0; i < NUM_MODULES && ret == UPDATE_STATUS::UPDATE_CONTINUE; ++i) { ret = modules[i]->PostUpdate(); }

    return ret;
}

bool Game::CleanUp() {
    bool ret = true;

    for (int i = NUM_MODULES - 1; i >= 0 && ret; --i) { ret = modules[i]->CleanUp(); }

    return ret;
}

ModuleWindow* Game::GetModuleWindow() const { return window; }
ModuleInput* Game::GetModuleInput() const { return input; }
ModuleTextures* Game::GetModuleTextures() const { return textures; }
ModuleAudio* Game::GetModuleAudio() const { return audio; }
ModulePlayer* Game::GetModulePlayer() const { return player; }
ModuleLevelOne* Game::GetModuleLevelOne() const { return levelOne; }
ModuleEnemies* Game::GetModuleEnemies() const { return enemies; }
ModuleParticles* Game::GetModuleParticles() const { return particles; }
ModuleCollisions* Game::GetModuleCollisions() const { return collisions; }
ModuleRender* Game::GetModuleRender() const { return render; }
ModuleTransition* Game::GetModuleTransition() const { return transition; }