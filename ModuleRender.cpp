#include "ModuleRender.h"

#include "Game.h"

#include "ModuleWindow.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"

#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_scancode.h"

ModuleRender::ModuleRender(bool startEnabled) : Module(startEnabled) {}

ModuleRender::~ModuleRender() {}

bool ModuleRender::Init() {
    LOG("Creating Renderer context");
    bool ret = true;

    Uint32 flags = 0;
    if (VSYNC == true) { flags |= SDL_RENDERER_PRESENTVSYNC; }

    renderer = SDL_CreateRenderer(game->GetModuleWindow()->GetWindow(), -1, flags);
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

UPDATE_STATUS ModuleRender::Update() {
    if (game->GetModuleInput()->GetKey(SDL_SCANCODE_ESCAPE)) { return UPDATE_STATUS::UPDATE_STOP; }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}


UPDATE_STATUS ModuleRender::PostUpdate() {
    //Update the screen
    //If put a debugging point here and press "SHIFT + F11", it is possible to move frame by frame
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
bool ModuleRender::Blit(SDL_Texture* texture, int x, int y, bool invert, SDL_Rect* section, SDL_Rect* adapter) {
    bool ret = true;

    SDL_Rect rect{ (int)(x * SCREEN_SIZE), (int)(y * SCREEN_SIZE), 0, 0 };

    if (section != nullptr && adapter == nullptr) {
        rect.w = section->w;
        rect.h = section->h;
    }
    else if (section != nullptr && adapter != nullptr) {
        rect.x = adapter->x;
        rect.y = adapter->y;
        rect.w = adapter->w;
        rect.h = adapter->h;
    }
    else {
        if (adapter != nullptr) {
            rect.w = adapter->w;
            rect.h = adapter->h;
        }
        else {
            SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h); //Collect texture size
        }
    }             

    rect.w *= SCREEN_SIZE;
    rect.h *= SCREEN_SIZE;

    if (invert == true) {
        if (SDL_RenderCopyEx(renderer, texture, section, &rect, NULL, NULL, SDL_FLIP_HORIZONTAL) != 0) {
            LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
            ret = false;
        }
    }
    else {
        if (SDL_RenderCopy(renderer, texture, section, &rect) != 0) {
            LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
            ret = false;
        }
    }
    

    return ret;
}

bool ModuleRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    bool ret = true;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    SDL_Rect dstRect{
        rect.x * SCREEN_SIZE,
        rect.y * SCREEN_SIZE,
        rect.w * SCREEN_SIZE,
        rect.h * SCREEN_SIZE
    };

    if (SDL_RenderFillRect(renderer, &dstRect) != 0) {
        LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
        ret = false;
    }
    return ret;
}

SDL_Renderer* ModuleRender::GetRenderer() const { return renderer; }
void ModuleRender::SetRenderer(SDL_Renderer* _renderer) { renderer = _renderer; }