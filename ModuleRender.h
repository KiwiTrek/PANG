#ifndef __MODULE_RENDER_H__
#define __MODULE_RENDER_H__

#include "Module.h"
#include "SDL/include/SDL_Rect.h"

struct SDL_Texture;
struct SDL_Renderer;

class ModuleRender : public Module {
public:
    //Constructor
    ModuleRender();

    //Destructor
    ~ModuleRender();

    // Called on application start
    // Creates rendering context
    bool Init() override;

    // Called at the beginning of the application loop
    // Clears the rendering context to the background color
    UPDATE_STATUS PreUpdate() override;

    // Called at the end of the application loop
    // Displays a rectangle in the rendering context
    // Updates the screen with the rendered content
    UPDATE_STATUS PostUpdate() override;

    // Called on application exit
    // Destroys the rendering context
    bool CleanUp() override;

    // Prints a texture to the rendering context
    // Param texture    - A valid SDL Texture, validation checks are not performed
    // Param x,y    - Position x,y on the screen
    // Param section    - The portion of the texture we want to copy (nullptr for the entire texture)
    // Param speed  - How much the camera movement affects the sprite
    bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section = nullptr, float speed = 1.0f);

    void SetRenderer(SDL_Renderer* _renderer);
    SDL_Renderer* GetRenderer() const;

private:
    // Rendering context used for any rendering action
    SDL_Renderer* renderer = nullptr;

    // A rectangle that represents the camera section
    // Sprites will be rendered to the screen depending on the camera position
    SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    // The speed at which the camera will be moving
    int cameraSpeed = 3;

};

#endif //__MODULE_RENDER_H__
