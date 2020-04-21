#ifndef __MODULE_WIN_SCREEN_H
#define __MODULE_WIN_SCREEN_H


#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class ModuleWinScreen : public Module {
public:
    //Constructor
    ModuleWinScreen(bool startEnabled);

    //Destructor
    ~ModuleWinScreen();

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
    SDL_Texture* splashArtTexture = nullptr;
    Animation splashArt;

	int bonusScore = 0;
	int normalFont2 = -1;

    int counter = 0;
    int nextExtendNumber = 20000;
    char stage[10] = { "\0" };
    char timeBonus[25] = { "\0" };
    char nextExtend[25] = { "\0" };
};

#endif