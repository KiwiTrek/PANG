#include "ModuleParticles.h"

#include "Game.h"

#include "ModuleAudio.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleCollisions.h"

#include "ModuleLevelOne.h"
#include "ModuleLevelTwo.h"
#include "ModuleLevelThree.h"
#include "ModuleLevelFour.h"



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
        normalWire.SetAnimPushBack({ 0 + (i * 17),220 - (i * 2) - j,9,33 + (i * 2) + j });
    }

    counter = 0;
    j = 0;

    for (int i = 0; i < 23; i++) {
        counter++;
        if (counter == 4) {
            j++;
            counter = 0;
        }
        normalWire.SetAnimPushBack({ 407 + (i * 17),168 - (i * 2) - j,9,85 + (i * 2) + j });

    }
    counter = 2;
    j = 0;
	
    for (int i = 0; i < 26; i++) {
        counter++;
        if (counter == 4) {
            j++;
            counter = 0;
        }
        normalWire.SetAnimPushBack({ 797 + (i * 17),117 - (i * 2) - j,9,136 + (i * 2) + j });
    }
    normalWire.SetAnimLoop(false);
    normalWire.SetAnimSpeed(0.888f); //0.888f
    normalWire.SetFSpeedY(-1.5f); //-1.5f

	//Power Wire animation
	int counterTwo = -1;
	int k = 0;
	for (int i = 0; i < 73; i++) {
		counterTwo++;
		if (counterTwo == 4) {
			k++;
			counterTwo = 0;
		}
		powerWire.SetAnimPushBack({ 8 + i * 17,161 - (i * 2) - k,9,33 + (i * 2) + k });
	}

	powerWire.SetAnimLoop(false);
	powerWire.SetAnimSpeed(0.888f); //0.888f
	powerWire.SetFSpeedY(-1.5f); //-1.5f

	//Power Shot animation
	powerShot.SetAnimPushBack({ 0,1,16,8 });
	powerShot.SetAnimPushBack({ 14,1,16,8 });
	powerShot.SetAnimPushBack({ 32,1,16,8 });

	for (int i = 0; i < 40; i++) {
		powerShot.SetAnimPushBack({ 54,2,16,7 });
		powerShot.SetAnimPushBack({ 77,0,16,14 });
	}

	powerShot.SetAnimLoop(false);
	powerShot.SetAnimSpeed(0.25f);
	powerShot.SetFSpeedY(-3.0f);

    //Balloon Explosions
    for (int i = 0; i < 4; ++i) { rBigBalloonExplosion.SetAnimPushBack({ 8 + i * 48,511,48,46 }); }
    rBigBalloonExplosion.SetAnimLoop(false);
    rBigBalloonExplosion.SetAnimSpeed(0.5f);

    for (int i = 0; i < 4; ++i) { rNotThatMehBalloonExplosion.SetAnimPushBack({ 8 + i * 31,457,31,29 }); }
    rNotThatMehBalloonExplosion.SetAnimLoop(false);
    rNotThatMehBalloonExplosion.SetAnimSpeed(0.5f);

    for (int i = 0; i < 4; ++i) { rMehBalloonExplosion.SetAnimPushBack({ 8 + i * 15,416,15,15 }); }
    rMehBalloonExplosion.SetAnimLoop(false);
    rMehBalloonExplosion.SetAnimSpeed(0.5f);

    for (int i = 0; i < 3; ++i) { rSmolBalloonExplosion.SetAnimPushBack({ 8 + i * 9,384,9,7 }); }
    rSmolBalloonExplosion.SetAnimLoop(false);
    rSmolBalloonExplosion.SetAnimSpeed(0.5f);
    


    for (int i = 0; i < 4; ++i) { bBigBalloonExplosion.SetAnimPushBack({ 8 + i * 48,326,48,46 }); }
    bBigBalloonExplosion.SetAnimLoop(false);
    bBigBalloonExplosion.SetAnimSpeed(0.5f);

    for (int i = 0; i < 4; ++i) { bNotThatMehBalloonExplosion.SetAnimPushBack({ 8 + i * 31,269,31,29 }); }
    bNotThatMehBalloonExplosion.SetAnimLoop(false);
    bNotThatMehBalloonExplosion.SetAnimSpeed(0.5f);

    for (int i = 0; i < 4; ++i) { bMehBalloonExplosion.SetAnimPushBack({ 8 + i * 15,228,15,15 }); }
    bMehBalloonExplosion.SetAnimLoop(false);
    bMehBalloonExplosion.SetAnimSpeed(0.5f);

    for (int i = 0; i < 3; ++i) { bSmolBalloonExplosion.SetAnimPushBack({ 8 + i * 9,196,9,7 }); }
    bSmolBalloonExplosion.SetAnimLoop(false);
    bSmolBalloonExplosion.SetAnimSpeed(0.5f);



    for (int i = 0; i < 4; ++i) { gBigBalloonExplosion.SetAnimPushBack({ 8 + i * 48,139,48,46 }); }
    gBigBalloonExplosion.SetAnimLoop(false);
    gBigBalloonExplosion.SetAnimSpeed(0.5f);

    for (int i = 0; i < 4; ++i) { gNotThatMehBalloonExplosion.SetAnimPushBack({ 8 + i * 31,82,31,29 }); }
    gNotThatMehBalloonExplosion.SetAnimLoop(false);
    gNotThatMehBalloonExplosion.SetAnimSpeed(0.5f);

    for (int i = 0; i < 4; ++i) { gMehBalloonExplosion.SetAnimPushBack({ 8 + i * 15,41,15,15 }); }
    gMehBalloonExplosion.SetAnimLoop(false);
    gMehBalloonExplosion.SetAnimSpeed(0.5f);

    for (int i = 0; i < 3; ++i) { gSmolBalloonExplosion.SetAnimPushBack({ 8 + i * 9,9,9,7 }); }
    gSmolBalloonExplosion.SetAnimLoop(false);
    gSmolBalloonExplosion.SetAnimSpeed(0.5f);

    //Muzzle Flash
    for (int i = 0; i < 4; ++i) { muzzleFlash.SetAnimPushBack({ 23 + i * 16,7,16,13 }); }
    muzzleFlash.SetAnimLoop(false);
    muzzleFlash.SetAnimSpeed(0.2f);

    //Vulcan Hits Ceiling
    vulcanCeiling.SetAnimPushBack({ 212,16,14,5 });
    vulcanCeiling.SetAnimPushBack({ 232,16,14,5 });
    vulcanCeiling.SetAnimLoop(false);
    vulcanCeiling.SetAnimSpeed(0.2f);

    //PowerWire attached
    hookPowerWire.SetAnimPushBack({ 0,0,9,195 });
    hookPowerWire.SetAnimPushBack({ 17,0,9,195 });
    hookPowerWire.SetAnimPushBack({ 34,0,9,195 });
    hookPowerWire.SetAnimLoop(false);
    hookPowerWire.SetAnimSpeed(0.01f);

    finalPowerWireSrc = {0,0,9,0};
    finalPowerWireDst = {};

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
    oncePowerWire = true;

    normalWire.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/normalWire.png"));
	powerWire.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/PowerWire.png"));
	powerShot.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/PowerShot.png"));
    rBigBalloonExplosion.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/boom.png"));
    rNotThatMehBalloonExplosion.SetParticleTexture(rBigBalloonExplosion.GetParticleTexture());
    rMehBalloonExplosion.SetParticleTexture(rBigBalloonExplosion.GetParticleTexture());
    rSmolBalloonExplosion.SetParticleTexture(rBigBalloonExplosion.GetParticleTexture());
    bBigBalloonExplosion.SetParticleTexture(rBigBalloonExplosion.GetParticleTexture());
    bNotThatMehBalloonExplosion.SetParticleTexture(rBigBalloonExplosion.GetParticleTexture());
    bMehBalloonExplosion.SetParticleTexture(rBigBalloonExplosion.GetParticleTexture());
    bSmolBalloonExplosion.SetParticleTexture(rBigBalloonExplosion.GetParticleTexture());
    gBigBalloonExplosion.SetParticleTexture(rBigBalloonExplosion.GetParticleTexture());
    gNotThatMehBalloonExplosion.SetParticleTexture(rBigBalloonExplosion.GetParticleTexture());
    gMehBalloonExplosion.SetParticleTexture(rBigBalloonExplosion.GetParticleTexture());
    gSmolBalloonExplosion.SetParticleTexture(rBigBalloonExplosion.GetParticleTexture());
    muzzleFlash.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/powerUps.png"));
    hitScreen.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/hit.png"));
    ready.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/blueText.png"));
    vulcanCeiling.SetParticleTexture(muzzleFlash.GetParticleTexture());
    hookPowerWire.SetParticleTexture(game->GetModuleTextures()->Load("Resources/Sprites/hookPowerWire.png"));

    vulcanCeilingSoundIndex = game->GetModuleAudio()->LoadFx("Resources/SFX/shotgunCeiling.wav");
    powerWireSoundIndex = game->GetModuleAudio()->LoadFx("Resources/SFX/hookshot.wav");

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
    game->GetModuleTextures()->Unload(powerWire.GetParticleTexture());
    game->GetModuleTextures()->Unload(powerShot.GetParticleTexture());
    game->GetModuleTextures()->Unload(rBigBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(rNotThatMehBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(rMehBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(rSmolBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(bBigBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(bNotThatMehBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(bMehBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(bSmolBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(gBigBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(gNotThatMehBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(gMehBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(gSmolBalloonExplosion.GetParticleTexture());
    game->GetModuleTextures()->Unload(muzzleFlash.GetParticleTexture());
    game->GetModuleTextures()->Unload(hitScreen.GetParticleTexture());
    game->GetModuleTextures()->Unload(ready.GetParticleTexture());
    game->GetModuleTextures()->Unload(vulcanCeiling.GetParticleTexture());
    game->GetModuleTextures()->Unload(hookPowerWire.GetParticleTexture());

    game->GetModuleAudio()->UnloadFx(vulcanCeilingSoundIndex);
    game->GetModuleAudio()->UnloadFx(powerWireSoundIndex);

    return true;
}


UPDATE_STATUS ModuleParticles::Update() {
    particleCounter = 0;
    for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
        Particle* particle = particles[i];
        if (particle == nullptr) { continue; }
        else if (particle->GetParticleTexture() == powerShot.GetParticleTexture()) { particleCounter++; }
        // Call particle Update. If it has reached its lifetime, destroy it
        if (particle->Update() == false) {
            if (particle->GetParticleTexture() == hookPowerWire.GetParticleTexture()) {
                oncePowerWire = true;
                game->GetModulePlayer()->IncreaseShoot();
            }
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
            if (particle->GetParticleTexture() == hitScreen.GetParticleTexture()) { game->GetModuleRender()->Blit(particle->GetParticleTexture(), particle->GetPositionX(), particle->GetPositionY(), false, &(particle->GetCurrentAnim())); }
            else if (particle->GetParticleTexture() == ready.GetParticleTexture()) {
                SDL_Rect gameOverAdapter = { (game->GetModuleLevelOne()->GetBackgroundAdapter().w / 2) - 30,(game->GetModuleLevelOne()->GetBackgroundAdapter().h / 2) - (TILE_SIZE),50,(TILE_SIZE*2) };
                game->GetModuleRender()->Blit(particle->GetParticleTexture(), particle->GetPositionX(), particle->GetPositionY(), false, &(particle->GetCurrentAnim()), &gameOverAdapter);
            }
            else if (particle->GetParticleTexture() == hookPowerWire.GetParticleTexture()) {
                if (hookPowerWire.GetFrameCount() <= 1) {
                    finalPowerWireSrc.x = 0;
                }
                else if (hookPowerWire.GetFrameCount() <= 2) {
                    finalPowerWireSrc.x = 17;
                }
                else if (hookPowerWire.GetFrameCount() <= 3) {
                    finalPowerWireSrc.x = 34;
                }
                game->GetModuleRender()->Blit(particle->GetParticleTexture(), finalPowerWireDst.x, finalPowerWireDst.y, false, &(finalPowerWireSrc));
            }
            else { game->GetModuleRender()->Blit(particle->GetParticleTexture(), particle->GetPositionX(), particle->GetPositionY(), false, &(particle->GetCurrentAnim())); }
            
            if (particle->GetParticleTexture() == normalWire.GetParticleTexture()) { 
                game->GetModuleRender()->Blit(game->GetModulePlayer()->GetTexture(), game->GetModulePlayer()->GetPosition().x, game->GetModulePlayer()->GetPosition().y, game->GetModulePlayer()->GetInvertValue(), &(game->GetModulePlayer()->GetCurrentAnimation())->GetCurrentFrame());
            }
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
            p->SetPositionX(x);                       // so when frameCount reaches 0 the particle will be activated
            p->SetPositionY(y);

            //Adding the particle's collider
            p->SetCollider(game->GetModuleCollisions()->AddCollider({ p->GetCurrentAnim().x,p->GetCurrentAnim().y,p->GetCurrentAnim().w,p->GetCurrentAnim().h }, colliderType, this));

            particles[i] = p;
            return particles[i];
        }
    }
}

void ModuleParticles::OnCollision(Collider* c1, Collider* c2) {
    for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
        // Always destroy particles that collide
        if (particles[i] != nullptr && particles[i]->GetCollider() == c1 && particles[i]->GetCollider()->GetType() == Collider::TYPE::PLAYER_SHOT && game->GetModulePlayer()->GetCurrentShotType() == SHOT_TYPES::POWER && c2->GetType() == Collider::TYPE::BALLOON) {
            oncePowerWire = true;
            game->GetModulePlayer()->IncreaseShoot();
            delete particles[i];
            particles[i] = nullptr;
            break;
        }
        else if (particles[i] != nullptr && particles[i]->GetCollider() == c1 && particles[i]->GetCollider()->GetType() == Collider::TYPE::PLAYER_SHOT && particles[i]->GetParticleTexture() != hookPowerWire.GetParticleTexture()) {
            if (game->GetModulePlayer()->GetCurrentShotType() == SHOT_TYPES::POWER
                && c2->GetType() != Collider::TYPE::BALLOON
                && oncePowerWire == true) {
                oncePowerWire = false;
                finalPowerWireDst.x = particles[i]->GetPositionX();
                finalPowerWireDst.y = particles[i]->GetPositionY();
                finalPowerWireDst.h = finalPowerWireSrc.h = particles[i]->GetCurrentAnim().h;
                game->GetModuleParticles()->AddParticle(hookPowerWire, finalPowerWireDst.x, finalPowerWireDst.y, Collider::TYPE::PLAYER_SHOT, 0U);
                game->GetModuleAudio()->PlayFx(powerWireSoundIndex, 0);
            }
            if (game->GetModulePlayer()->GetCurrentShotType() != SHOT_TYPES::POWER) { game->GetModulePlayer()->IncreaseShoot(); }
            if (game->GetModulePlayer()->GetCurrentShotType() == SHOT_TYPES::NORMAL && particleCounter > 0) { game->GetModulePlayer()->DecreaseShoot(); }
            if (game->GetModulePlayer()->GetCurrentShotType() == SHOT_TYPES::VULCAN && c2->GetType() != Collider::TYPE::BALLOON) {
                game->GetModuleParticles()->AddParticle(vulcanCeiling, particles[i]->GetPositionX(), particles[i]->GetPositionY(), Collider::TYPE::NONE);
                game->GetModuleAudio()->PlayFx(vulcanCeilingSoundIndex, 0);
            }
            delete particles[i];
            particles[i] = nullptr;
            break;
        }
    }
}

int ModuleParticles::GetParticleCounter() const { return particleCounter; }
