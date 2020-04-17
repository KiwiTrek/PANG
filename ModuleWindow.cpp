#include "ModuleWindow.h"

#include "Game.h"

#include "Globals.h"

#include "SDL/include/SDL.h"


ModuleWindow::ModuleWindow(bool startEnabled) : Module(startEnabled) {}

ModuleWindow::~ModuleWindow() {}

bool ModuleWindow::Init() {
    LOG("Init SDL window & surface---------------");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    else {
        //Create window
        Uint32 flags = SDL_WINDOW_SHOWN;

        if (WIN_FULLSCREEN == true) { flags |= SDL_WINDOW_FULLSCREEN; }	
        if (WIN_BORDERLESS == true) { flags |= SDL_WINDOW_BORDERLESS; }
        if (WIN_RESIZABLE == true) { flags |= SDL_WINDOW_RESIZABLE; }	
        if (WIN_FULLSCREEN_DESKTOP == true) { flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; }

        window = SDL_CreateWindow("PANG! (By Bruhmentium INC.)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE, flags);
        if (window == nullptr) {
            LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }
        else { screenSurface = SDL_GetWindowSurface(window); }
    }

    if (SDL_Init(SDL_INIT_TIMER) < 0) {
        LOG("SDL_TIMER could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool ModuleWindow::CleanUp() {
    LOG("Destroying SDL window and quitting all SDL systems");

    //Destroy window
    if (window != nullptr) { SDL_DestroyWindow(window); }

    //Quit SDL subsystems
    SDL_Quit();

    return true;
}

void ModuleWindow::SetWindow(SDL_Window* _window) { window = _window; }
SDL_Window* ModuleWindow::GetWindow() const { return window; }
void ModuleWindow::SetScreenSurface(SDL_Surface* _screenSurface) { screenSurface = _screenSurface; }
SDL_Surface* ModuleWindow::GetScreenSurface() const { return screenSurface; }