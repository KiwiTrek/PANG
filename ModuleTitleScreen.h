#ifndef __MODULE_TITLE_SCREEN_H
#define __MODULE_TITLE_SCREEN_H

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleTitleScreen : public Module {
public:
    //Constructor
    ModuleTitleScreen(bool startEnabled);

    //Destructor
    ~ModuleTitleScreen();

    // Called when the module is activated
    // Loads the necessary textures for the map background
    bool Start() override;

    // Called at the middle of the application loop
    // Updates the scene's background animations
    UPDATE_STATUS Update() override;

    // Called at the end of the application loop.
    // Performs the render call of all the parts of the scene's background
    UPDATE_STATUS PostUpdate() override;

    bool CleanUp() override;

private:
    // The scene sprite sheet loaded into an SDL_Texture
    SDL_Texture* backgroundTexture = nullptr;

    // Initial Animation Variables
    SDL_Texture* titleScreenAnimationTexture = nullptr;
    Animation titleScreenAnimation;
    uint titleScreenAnimationSFX;
    bool once;
};

#endif