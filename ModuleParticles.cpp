#include "ModuleParticles.h"
#include "Game.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollisions.h"
#include "SDL/include/SDL_timer.h"



ModuleParticles::ModuleParticles() {
    for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) { particles[i] = nullptr; }

    //Normal wire shot animation
    int counter = -1;
    int j = 0;

    for (int i = 0; i < 23; i++) {
        counter++;
        if (counter == 4) {
            j++;
            counter = 0;
        }
        NormalWire.SetAnimPushBack({ 0 + (i * 17),155 - (i * 2) - j,9,33 + (i * 2) + j });
    }

    counter = 0;
    j = 0;

    for (int i = 0; i < 23; i++) {
        counter++;
        if (counter == 4) {
            j++;
            counter = 0;
        }
        NormalWire.SetAnimPushBack({ 407 + (i * 17),103 - (i * 2) - j,9,85 + (i * 2) + j });

    }
    counter = 2;
    j = 0;

    for (int i = 0; i < 23; i++) {
        counter++;
        if (counter == 4) {
            j++;
            counter = 0;
        }
        NormalWire.SetAnimPushBack({ 797 + (i * 17),52 - (i * 2) - j,9,136 + (i * 2) + j });
    }

    NormalWire.SetAnimLoop(false);
    NormalWire.SetAnimSpeed(0.44f); //0.44f
    NormalWire.SetFSpeedY(-0.66f); //-0.66f
    NormalWire.SetLifetime(250);
}

ModuleParticles::~ModuleParticles() {}

bool ModuleParticles::Start() {
    LOG("Loading particles");
    NormalWireTexture = game->GetModuleTextures()->Load("Resources/Sprites/NormalWire.png");




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
        if (particle != nullptr && particle->CheckIsAlive() && particle->GetLifetime() != 0) { game->GetModuleRender()->Blit(NormalWireTexture, particle->GetPositionX(), particle->GetPositionY(), false, &(particle->GetCurrentAnim())); }
    }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y, Collider::TYPE colliderType, uint delay) {
    for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
        //Finding an empty slot for a new particle
        if (particles[i] == nullptr) {
            Particle* p = new Particle(particle);

            p->SetFrameCount(-(int)delay);            // We start the frameCount as the negative delay
            p->SetPositionX(x);                        // so when frameCount reaches 0 the particle will be activated
            p->SetPositionY(y);

            //Adding the particle's collider
            p->SetCollider(game->GetModuleCollisions()->AddCollider(p->GetCurrentAnim(), colliderType, this));

            particles[i] = p;
            break;
        }
    }
}

void ModuleParticles::OnCollision(Collider* c1, Collider* c2) {
    for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
        // Always destroy particles that collide
        if (particles[i] != nullptr && particles[i]->GetCollider() == c1 && particles[i]->GetCollider()->GetType() == Collider::TYPE::PLAYER_SHOT) {
            game->GetModulePlayer()->SetIfShot(false);
            delete particles[i];
            particles[i] = nullptr;
            break;
        }
    }
}
