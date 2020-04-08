#include "ModuleParticles.h"
#include "Game.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "SDL/include/SDL_timer.h"



ModuleParticles::ModuleParticles() {
    for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
        particles[i] = nullptr;
}

ModuleParticles::~ModuleParticles() {}

bool ModuleParticles::Start() {
    LOG("Loading particles");
    NormalWireTexture = game->textures->Load("Resources/Sprites/NormalWire.png");

    //Normal wire shot animation
    int counter = -1;
    int j = 0;

    for (int i = 0; i < 23; i++){
        counter++;
        if (counter == 4) {
            j++;
            counter = 0;
        }
        NormalWire.anim.PushBack({ 0 + (i * 17),155 - (i * 2)  - j,9,33 + (i * 2) + j});	
    }

    counter = 0;
    j = 0;

    for (int i = 0; i < 23; i++) {
        counter++;
        if (counter == 4) {
            j++;
            counter = 0;
        }
        NormalWire.anim.PushBack({ 407 + (i * 17),103 - (i * 2) - j,9,85 + (i * 2) + j});
        
    }
    counter = 2;
    j = 0;

    for (int i = 0; i < 23; i++) {
        counter++;
        if (counter == 4) {
            j++;
            counter = 0;
        }
        NormalWire.anim.PushBack({ 797 + (i * 17),52 - (i * 2) - j,9,136 + (i * 2) + j});
    }


    NormalWire.anim.loop = false;
    NormalWire.anim.speed = 0.44f; //0.44f
    NormalWire.speed.y = -0.66f; //-0.66f
    
    NormalWire.lifetime = 250;

    return true;
}


bool ModuleParticles::CleanUp() {
    LOG("Unloading particles");

    // Delete all remaining active particles on application exit 
    for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
        if (particles[i] != nullptr) {
            delete particles[i];
            particles[i] = nullptr;
        }
    }
    return true;
}


UPDATE_STATUS ModuleParticles::Update() {
    for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
        Particle* particle = particles[i];
        if (particle == nullptr) { continue; }
        // Call particle Update. If it has reached its lifetime, destroy it
        if (particle->Update() == false) {
            delete particle;
            particles[i] = nullptr;
        }
    }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleParticles::PostUpdate() {
    //Iterating all particle array and drawing any active particles
    for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
        Particle* particle = particles[i];
        if (particle != nullptr && particle->isAlive) { game->render->Blit(NormalWireTexture, particle->position.x, particle->position.y, false, &(particle->anim.GetCurrentFrame())); }
    }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y, uint delay) {
    for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
        //Finding an empty slot for a new particle
        if (particles[i] == nullptr) {
            Particle* p = new Particle(particle);

            p->frameCount = -(int)delay;            // We start the frameCount as the negative delay
            p->position.x = x;                        // so when frameCount reaches 0 the particle will be activated
            p->position.y = y;

            particles[i] = p;
            break;
        }
    }
}
