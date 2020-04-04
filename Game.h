#ifndef __GAME_H__
#define __GAME_H__

#include "Globals.h"

#define NUM_MODULES 5

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleTextures;
//class ModuleAudio;
//class ModulePlayer;
class ModuleLevelOne;
//class ModuleParticles;
//class ModuleCollisions;
//class ModuleEnemies;
class ModuleRender;

class Game {

public:

    //Constructor. Creates all necessary modules for the application
    Game();
    //Destructor. Removes all module objects
    ~Game();
    //Initializes all modules
    bool Init();
    //Updates all modules (PreUpdate, Update and PostUpdate)
    UPDATE_STATUS Update();
    //Releases all the application data
    bool CleanUp();

public:
    // An array to store all modules
    Module* modules[NUM_MODULES];
    // All the modules stored individually
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleTextures* textures = nullptr;
    //ModuleAudio* audio = nullptr;
    //ModulePlayer* player = nullptr;
    ModuleLevelOne* levelOne = nullptr;
    //ModuleEnemies* enemies = nullptr;
    //ModuleParticles* particles = nullptr;
    //ModuleCollisions* collisions = nullptr;
    ModuleRender* render = nullptr;
};

// Global var made extern for Game ---
extern Game* game;

#endif // __GAME_H__