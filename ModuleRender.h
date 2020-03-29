#ifndef __MODULE_RENDER_H__
#define __MODULE_RENDER_H__
#include "Module.h"

struct SDL_Rect;
struct SDL_Texture;
struct SDL_Renderer;

class ModuleRender : public Module {
public:
    ModuleRender();
    ~ModuleRender();

    // Creates the render
    bool Init() override;

    // Resets the render
    UPDATE_STATUS PreUpdate() override;

    // Updates the screen with the rendered content
    UPDATE_STATUS PostUpdate() override;

    // Called on application exit.
    // Destroys the rendering context
    bool CleanUp() override;

    // Prints a texture to the rendering context.
    // Param texture	- A valid SDL Texture, validation checks are not performed
    // Param x,y		- Position x,y in the screen (upper left axis)
    // Param section	- The portion of the texture we want to copy. nullptr for the entire texture
    bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section = nullptr);

public:
    // Rendering context used for any rendering action
    SDL_Renderer* renderer = nullptr;

private:
    SDL_Texture* testTexture = nullptr;

};

#endif //__MODULE_RENDER_H__
