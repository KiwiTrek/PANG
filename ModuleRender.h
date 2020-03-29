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
    update_status PreUpdate() override;

    // Updates the screen with the rendered content
    update_status PostUpdate() override;

    // Destroys the renderer
    bool CleanUp() override;

    // Blit prints a texture
    // Param texture    - A valid SDL Texture
    // Param x,y        - Position x,y in the screen
    // Param section    - The part of the texture we want to copy (nullptr = the entire texture)
    bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section = nullptr);

public:
    //Used for any rendering action
    SDL_Renderer* renderer = nullptr;

private:
    SDL_Texture* testTexture = nullptr;
};

#endif //__MODULE_RENDER_H__