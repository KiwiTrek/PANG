#ifndef __MODULE_WINDOW_H__
#define __MODULE_WINDOW_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module {
public:
    //Constructor
    ModuleWindow(bool startEnabled);
    // Destructor
    ~ModuleWindow();
    // Called on application start.
    // Initializes the SDL Library and creates a window.
    bool Init() override;
    // Called on application exit.
    // Destroys the window and uninitializes the SDL library
    bool CleanUp() override;

    void SetWindow(SDL_Window* _window);
    SDL_Window* GetWindow() const;
    void SetScreenSurface(SDL_Surface* _screenSurface);
    SDL_Surface* GetScreenSurface() const;

private:
    //The window we'll be rendering to
    SDL_Window* window = nullptr;
    //The window's surface
    SDL_Surface* screenSurface = nullptr;
};

#endif // !__MODULE_WINDOW_H__

