#include "ModuleTransition.h"

#include "Game.h"

#include "ModuleRender.h"

#include "SDL/include/SDL_render.h"

ModuleTransition::ModuleTransition(bool startEnabled) : Module(startEnabled) {
    screenRect = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };
}

ModuleTransition::~ModuleTransition() {}

bool ModuleTransition::Start() {
    LOG("Preparing Fade Screen");

    // Enable blending mode for transparency
    SDL_SetRenderDrawBlendMode(game->GetModuleRender()->GetRenderer(), SDL_BLENDMODE_BLEND);
    return true;
}

UPDATE_STATUS ModuleTransition::Update() {
    // Exit this function if we are not performing a fade
    if (currentStep == Transition_Step::NONE) { return UPDATE_STATUS::UPDATE_CONTINUE; }

    if (currentStep == Transition_Step::TO_BLACK) {
        ++frameCount;
        if (frameCount >= maxFadeFrames) {
            moduleToDisable->Disable();
            moduleToEnable->Enable();

            currentStep = Transition_Step::FROM_BLACK;
        }
    }
    else {
        --frameCount;
        if (frameCount <= 0) { currentStep = Transition_Step::NONE; }
    }

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleTransition::PostUpdate() {
    // Exit this function if we are not performing a fade
    if (currentStep == Transition_Step::NONE) { return UPDATE_STATUS::UPDATE_CONTINUE; }

    float fadeRatio = ((float)frameCount / (float)maxFadeFrames);

    // Render the black square with alpha on the screen
    SDL_SetRenderDrawColor(game->GetModuleRender()->GetRenderer(), 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
    SDL_RenderFillRect(game->GetModuleRender()->GetRenderer(), &screenRect);

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

bool ModuleTransition::Transition(Module* moduleToDisable, Module* moduleToEnable, float frames) {
    // If we are already in a fade process, ignore this call
    if (currentStep == Transition_Step::NONE) {
        currentStep = Transition_Step::TO_BLACK;
        frameCount = 0;
        maxFadeFrames = frames;

        this->moduleToDisable = moduleToDisable;
        this->moduleToEnable = moduleToEnable;

        return true;
    }

    return false;
}