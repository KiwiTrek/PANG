#ifndef __MODULE_TEXTURES_H__
#define __MODULE_TEXTURES_H__

#include "Module.h"
#define MAX_TEXTURES 50

struct SDL_Texture;

class ModuleTextures : public Module {
public:
    ModuleTextures();
    ~ModuleTextures();

    // Initializes the SDL_Image library
    bool Init() override;

    // Frees the textures, makes them point to nullptr and quits IMG
    bool CleanUp() override;

    // Loads textures inside the array
    SDL_Texture* const Load(const char* path);

public:
    // Stores loaded textures in order to be freed on application exit
    SDL_Texture* textures[MAX_TEXTURES];
};

#endif // __ModuleTextures_H__