#ifndef __MODULE_PARTICLES_H__
#define __MODULE_PARTICLES_H__

#include "Module.h"
#include "Globals.h"
#include "Particle.h"
#include "Game.h"

#define MAX_ACTIVE_PARTICLES 2500

struct SDL_Texture;

class ModuleParticles : public Module
{
public:
    // Constructor
    // Initializes all the particles in the array to nullptr
    ModuleParticles();

    //Destructor
    ~ModuleParticles();

    // Called when the module is activated
    // Loads the necessary textures for the particles
    bool Start() override;

    // Called at the middle of the application loop
    // Iterates all the particles and calls its Update()
    // Removes any "dead" particles
    UPDATE_STATUS Update() override;

    // Called at the end of the application loop
    // Iterates all the particles and draws them
    UPDATE_STATUS PostUpdate() override;

    // Called on application exit
    // Destroys all active particles left in the array
    bool CleanUp() override;

    // Creates a new particle and adds it to the array
    // Param particle	- A template particle from which the new particle will be created
    // Param x, y		- Position x,y in the screen (upper left axis)
    // Param delay		- Delay time from the moment the function is called until the particle is displayed in screen
    void AddParticle(const Particle& particle, int x, int y, uint delay = 0);

private:
    // Shot (and other) textures go here
    SDL_Texture* NormalWireTexture = nullptr;

    // An array to store and handle all the particles
    Particle* particles[MAX_ACTIVE_PARTICLES] = { nullptr };

    // An index to the last added particle
    uint lastParticle = 0;

public:
    //Shots (and others) go here
    Particle NormalWire;
    SDL_Rect NormalWireRectStart = { 0,55,8,33 };
};

#endif // !__MODULEPARTICLES_H__