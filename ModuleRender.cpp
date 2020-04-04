#include "ModuleRender.h"
#include "Game.h"
#include "ModuleWindow.h"
#include "ModuleTextures.h"

#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_scancode.h"

ModuleRender::ModuleRender() : Module() {}

ModuleRender::~ModuleRender() {}

bool ModuleRender::Init() {
    LOG("Creating Renderer context");
    bool ret = true;

    Uint32 flags = 0;
    if (VSYNC == true) { flags |= SDL_RENDERER_PRESENTVSYNC; }

    renderer = SDL_CreateRenderer(game->window->GetWindow(), -1, flags);
    if (renderer == nullptr) {
    LOG("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    ret = false;
    }

    return ret;
}

// Called every draw update
UPDATE_STATUS ModuleRender::PreUpdate() {
    //Set the color used for drawing operations
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    //Clear rendering target
    SDL_RenderClear(renderer);

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleRender::PostUpdate() {
    //Update the screen
    SDL_RenderPresent(renderer);

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleRender::CleanUp() {
    LOG("Destroying renderer");

    //Destroy the rendering context
    if (renderer != nullptr) { SDL_DestroyRenderer(renderer); }

    return true;
}

// Blit to screen
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float speed) {
    bool ret = true;

    SDL_Rect rect{ (int)(x * SCREEN_SIZE), (int)(y * SCREEN_SIZE), 0, 0 };

    if (section != nullptr) {
        rect.w = section->w;
        rect.h = section->h;
    }
    else { SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h); }             //Collect texture size

    rect.w *= SCREEN_SIZE;
    rect.h *= SCREEN_SIZE;

    if (SDL_RenderCopy(renderer, texture, section, &rect) != 0) {
        LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
        ret = false;
    }

    return ret;
}

void ModuleRender::SetRenderer(SDL_Renderer* _renderer) {
    renderer = _renderer;
}

SDL_Renderer* ModuleRender::GetRenderer() const {
    return renderer;
}