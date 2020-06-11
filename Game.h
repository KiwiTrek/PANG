#ifndef __GAME_H__
#define __GAME_H__

#include "Globals.h"

#define NUM_MODULES 19

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleTextures;
class ModuleAudio;
class ModulePlayer;
class ModuleProjectSheet;
class ModuleTitleScreen;
class ModuleLevelOne;
class ModuleLevelTwo;
class ModuleLevelThree;
class ModuleLevelFour;
class ModuleParticles;
class ModuleWinScreen;
class ModuleCollisions;
class ModuleEnemies;
class ModuleRender;
class ModuleTransition;
class ModuleFonts;
class ModuleTileset;

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

    ModuleWindow* GetModuleWindow() const;
    ModuleInput* GetModuleInput() const;
    ModuleTextures* GetModuleTextures() const;
    ModuleAudio* GetModuleAudio() const;
    ModuleProjectSheet* GetModuleProjectSheet() const;
    ModuleTitleScreen* GetModuleTitleScreen() const;

    ModuleLevelOne* GetModuleLevelOne() const;
    ModuleLevelTwo* GetModuleLevelTwo() const;
    ModuleLevelThree* GetModuleLevelThree() const;
    ModuleLevelFour* GetModuleLevelFour() const;

    ModulePlayer* GetModulePlayer() const;
    ModuleEnemies* GetModuleEnemies() const;
    ModuleParticles* GetModuleParticles() const;
    ModuleWinScreen* GetModuleWinScreen() const;
    ModuleCollisions* GetModuleCollisions() const;
    ModuleRender* GetModuleRender() const;
    ModuleTransition* GetModuleTransition() const;
    ModuleFonts* GetModuleFonts() const;
    ModuleTileset* GetModuleTileset() const;

private:
    // An array to store all modules
    Module* modules[NUM_MODULES];
    // All the modules stored individually
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleTextures* textures = nullptr;
    ModuleAudio* audio = nullptr;
    ModuleProjectSheet* projectSheet = nullptr;
    ModuleTitleScreen* titleScreen = nullptr;

    ModuleLevelOne* levelOne = nullptr;
    ModuleLevelTwo* levelTwo = nullptr;
    ModuleLevelThree* levelThree = nullptr;
    ModuleLevelFour* levelFour = nullptr;

    ModulePlayer* player = nullptr;
    ModuleEnemies* enemies = nullptr;
    ModuleParticles* particles = nullptr;
    ModuleWinScreen* winScreen = nullptr;
    ModuleCollisions* collisions = nullptr;
    ModuleRender* render = nullptr;
    ModuleTransition* transition = nullptr;
    ModuleFonts* fonts = nullptr;
    ModuleTileset* tileset = nullptr;
};

// Global var made extern for Game ---
extern Game* game;

#endif // __GAME_H__