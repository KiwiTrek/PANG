#include "ModuleParticles.h"

#include "Game.h"

#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleLevelOne.h"
#include "ModuleCollisions.h"



ModuleParticles::ModuleParticles(bool startEnabled) : Module(startEnabled) {
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
        normalWire.SetAnimPushBack({ 0 + (i * 17),155 - (i * 2) - j,9,33 + (i * 2) + j });
    }

    counter = 0;
    j = 0;

    for (int i = 0; i < 23; i++) {
        counter++;
        if (counter == 4) {
            j++;
            counter = 0;
        }
        normalWire.SetAnimPushBack({ 407 + (i * 17),103 - (i * 2) - j,9,85 + (i * 2) + j });

    }
    counter = 2;
    j = 0;

    for (int i = 0; i < 23; i++) {
        counter++;
        if (counter == 4) {
            j++;
            counter = 0;
        }
        normalWire.SetAnimPushBack({ 797 + (i * 17),52 - (i * 2) - j,9,136 + (i * 2) + j });
    }
    normalWire.SetAnimLoop(false);
    normalWire.SetAnimSpeed(0.888f); //0.888f
    normalWire.SetFSpeedY(-1.5f); //-1.5f

    //Balloon Explosions
    for (int i = 0; i < 4; ++i) { bigBalloonExplosion.SetAnimPushBack({ 8 + i * 48,511,48,46 }); }
    bigBalloonExplosion.SetAnimLoop(false);
    bigBalloonExplosion.SetAnimSpeed(0.5f);

    for (int i = 0; i < 4; ++i) { notThatMehBalloonExplosion.SetAnimPushBack({ 8 + i * 31,457,31,29 }); }
    notThatMehBalloonExplosion.SetAnimLoop(false);
    notThatMehBalloonExplosion.SetAnimSpeed(0.5f);

    for (int i = 0; i < 4; ++i) { mehBalloonExplosion.SetAnimPushBack({ 8 + i * 15,416,15,15 }); }
    mehBalloonExplosion.SetAnimLoop(false);
    mehBalloonExplosion.SetAnimSpeed(0.5f);

    for (int i = 0; i < 3; ++i) { smolBalloonExplosion.SetAnimPushBack({ 8 + i * 9,384,9,7 }); }
    smolBalloonExplosion.SetAnimLoop(false);
    smolBalloonExplosion.SetAnimSpeed(0.5f);

    //Muzzle Flash
    for (int i = 0; i < 4; ++i) { muzzleFlash.SetAnimPushBack({ 23 + i * 16,7,16,13 }); }
    muzzleFlash.SetAnimLoop(false);
    muzzleFlash.SetAnimSpeed(0.2f);

    //Hit Screen
    hitScreen.SetAnimPushBack({ 0,0,384,208 });
    hitScreen.SetAnimLoop(false);
    hitScreen.SetAnimSpeed(0.0f);
    hitScreen.SetLifetime(10);

    //Ready
    ready.SetAnimPushBack({ 5,132,203,67 });
    ready.SetAnimLoop(false);
    ready.SetLifetime(5);
}

ModuleParticles::~ModuleParticles() {}

bool ModuleParticles::Start() {
    LOG("Loading particles");

    normalWire.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/normalWire.png"));
    bigBalloonExplosion.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/boom.png"));
    notThatMehBalloonExplosion.SetParticleTexture(bigBalloonExplosion.GetParticleTexture());
    mehBalloonExplosion.SetParticleTexture(bigBalloonExplosion.GetParticleTexture());
    smolBalloonExplosion.SetParticleTexture(bigBalloonExplosion.GetParticleTexture());
    muzzleFlash.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/powerUps.png"));
    hitScreen.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/hit.png"));
    ready.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/blueText.png"));

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

    game->GetModuleTextures()->Unload(normalWire.GetParticleTexture());
    game->GetModuleTextures()->Unload(bigBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(notThatMehBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(mehBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(smolBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(muzzleFlash.GetParticleTexture());
    game->GetModuleTextures()->Unload(hitScreen.GetParticleTexture());
    game->GetModuleTextures()->Unload(ready.GetParticleTexture());

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
        if (particle != nullptr && particle->CheckIsAlive()) {
            SDL_Rect backgroundAdapter = { 0, 0, 384, 193 };
            if (particle->GetParticleTexture() == hitScreen.GetParticleTexture()) { game->GetModuleRender()->Blit(particle->GetParticleTexture(), particle->GetPositionX(), particle->GetPositionY(), false, &(particle->GetCurrentAnim()), &backgroundAdapter); }
            else if (particle->GetParticleTexture() == ready.GetParticleTexture()) {
                SDL_Rect gameOverAdapter = { SCREEN_WIDTH+85,backgroundAdapter.h+75,60,17 };
                game->GetModuleRender()->Blit(particle->GetParticleTexture(), particle->GetPositionX(), particle->GetPositionY(), false, &(particle->GetCurrentAnim()), &gameOverAdapter);
            }
            else { game->GetModuleRender()->Blit(particle->GetParticleTexture(), particle->GetPositionX(), particle->GetPositionY(), false, &(particle->GetCurrentAnim())); }
            
            if (particle->GetParticleTexture() == normalWire.GetParticleTexture()) { game->GetModuleRender()->Blit(game->GetModulePlayer()->GetTexture(), game->GetModulePlayer()->GetPosition().x, game->GetModulePlayer()->GetPosition().y, game->GetModulePlayer()->GetInvertValue(), &(game->GetModulePlayer()->GetCurrentAnimation())->GetCurrentFrame()); }
        }
    }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

Particle* ModuleParticles::AddParticle(const Particle& particle, int x, int y, Collider::TYPE colliderType, uint delay) {
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
            return particles[i];
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
