#ifndef __MODULE_LEVEL_ONE_H
#define __MODULE_LEVEL_ONE_H

#include "Module.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class ModuleLevelOne : public Module {
public:
    //Constructor
    ModuleLevelOne();
    //Destructor
    ~ModuleLevelOne();
    // Called when the module is activated
    // Loads the necessary textures for the map background
    bool Start() override;
    // Called while updating
    // Detects if the intro has ended
    UPDATE_STATUS Update();

    // Called at the end of the application loop.
    // Performs the render call of all the parts of the scene's background
    UPDATE_STATUS PostUpdate();
    
public:
    // The sprite section for the background
    SDL_Rect background;
    SDL_Rect backgroundAdapter;
    // The scene sprite sheet loaded into an SDL_Texture
    SDL_Texture* backgroundTexture = nullptr;

    
};

#endif