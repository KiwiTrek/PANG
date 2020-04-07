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
	NormalWireTexture = game->textures->Load("Resources/Sprites/Normal_Wire.png");

	//Normal wire shot animation
	NormalWire.anim.PushBack({ 0,55,8,33 });
	NormalWire.anim.PushBack({ 17,53,8,35 });
	NormalWire.anim.loop = true;
	NormalWire.anim.speed = 0.3f;



	
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
		if (particle != nullptr && particle->isAlive) { game->render->Blit(NormalWireTexture, particle->position.x, particle->position.y-particle->frameCount, false, &(particle->anim.GetCurrentFrame())); }
	}
	return UPDATE_STATUS::UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y, uint delay) {
	Particle* p = new Particle(particle);

	p->frameCount = -(int)delay;			// We start the frameCount as the negative delay
	p->position.x = x;						// so when frameCount reaches 0 the particle will be activated
	p->position.y = y;

	particles[lastParticle++] = p;
	lastParticle %= MAX_ACTIVE_PARTICLES;
}