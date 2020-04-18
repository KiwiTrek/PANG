#include "ModuleAudio.h"

#include "Game.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio(bool startEnabled): Module(startEnabled) { for (uint i = 0; i < MAX_FX; ++i) { soundFx[i] = nullptr; } }

ModuleAudio::~ModuleAudio() {}

bool ModuleAudio::Init() {
    LOG("Loading Audio Mixer");

    //Initialize audio subsystem
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    //Load support for OGG format
    int flags = MIX_INIT_OGG;
    int init = Mix_Init(flags);

    if ((init & flags) != flags) {
        LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
        return false;
    }

    //Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    return true;
}

// Called before quitting
bool ModuleAudio::CleanUp() {
    LOG("Freeing sound FX, closing Mixer and Audio subsystem");

    if (music != NULL) {
        Mix_FreeMusic(music);
    }

    for (uint i = 0; i < MAX_FX; ++i) {
        if (soundFx[i] != nullptr)
            Mix_FreeChunk(soundFx[i]);
    }

    Mix_CloseAudio();
    Mix_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);

    return true;
}

bool ModuleAudio::PlayMusicOnce(const char* path) {
    if (music != NULL) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
    }

    music = Mix_LoadMUS(path);

    if (music == NULL) {
        LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
        return false;
    }
    else {
        if (Mix_PlayMusic(music, 0) < 0) {
            LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
            return false;
        }
    }

    LOG("Successfully playing %s", path);

    return true;
}

bool ModuleAudio::DetectIfEnd() const { return Mix_PlayingMusic(); };
void ModuleAudio::ChangeAtEnd(const char* newSong) { if (!DetectIfEnd()) { PlayMusic(newSong); } }

void ModuleAudio::PlayMusic(const char* path) {
    if (music != NULL) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
    }

    music = Mix_LoadMUS(path);

    if (music == NULL) {
        LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
    }
    else {
        if (Mix_PlayMusic(music, -1) < 0) {
            LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
        }
    }
    
    LOG("Successfully playing %s", path);
}

uint ModuleAudio::LoadFx(const char* path)
{
    uint indexReturned = 0;
    Mix_Chunk* chunk = Mix_LoadWAV(path);

    if (chunk == nullptr) { LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError()); }
    else {
        for (indexReturned = 0; indexReturned < MAX_FX; ++indexReturned) {
            if (soundFx[indexReturned] == nullptr) {
                soundFx[indexReturned] = chunk;
                break;
            }
        }
    }

    return indexReturned;
}

bool ModuleAudio::PlayFx(uint index, int repeat)
{
    if (soundFx[index] != nullptr) {
        Mix_PlayChannel(-1, soundFx[index], repeat);
        return true;
    }

    return false;
}