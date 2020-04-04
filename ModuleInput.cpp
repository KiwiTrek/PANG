#include "Globals.h"
#include "Game.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"

ModuleInput::ModuleInput() : Module() {}
ModuleInput::~ModuleInput() {}

bool ModuleInput::Init() {
    SDL_Init(0);
    if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
        LOG("SDL_Events could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

UPDATE_STATUS ModuleInput::PreUpdate() {
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) { return UPDATE_STATUS::UPDATE_STOP; }
    }

    SDL_PumpEvents();
    const Uint8* keyboard = SDL_GetKeyboardState(NULL);

    for (int i = 0; i < MAX_KEYS; ++i) {
        if (keyboard[i]) {
            if (keys[i] == KEY_IDLE) { keys[i] = KEY_DOWN; }
            else { keys[i] = KEY_REPEAT; }
        }
        else {
            if (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) { keys[i] = KEY_UP; }
            else { keys[i] = KEY_IDLE; }
        }
    }

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleInput::CleanUp() {

    SDL_QuitSubSystem(SDL_INIT_EVENTS);

    return true;
}

void ModuleInput::SetKey(const KEY_STATE _key, int i) {
    keys[i] = _key;
}

KEY_STATE ModuleInput::GetKey(int i) const {
    return keys[i];
}