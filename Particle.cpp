#include "Game.h"

#include "ModuleParticles.h"
#include "Particle.h"
#include "Collider.h"

#include "ModulePlayer.h"
#include "SDL/include/SDL_timer.h"

Particle::Particle() {
    position.SetToZero();
    speed.SetToZero();
}

Particle::Particle(const Particle& p) : anim(p.anim), position(p.position), speed(p.speed), frameCount(p.frameCount), lifetime(p.lifetime), particleTexture(p.particleTexture) {}

Particle::~Particle() { if (collider != nullptr) { collider->SetPendingToDelete(true); } }

bool Particle::Update() {
    if (!game->GetModulePlayer()->CheckIfDestroyed() || particleTexture == game->GetModuleParticles()->hitScreen.GetParticleTexture()) {
        frameCount++;

        // The particle is set to 'alive' when the delay has been reached
        if (!isAlive && frameCount >= 0) { isAlive = true; }

        if (isAlive) {
            anim.Update();

            // If the particle has a specific lifetime, check when it has to be destroyed
            if (lifetime > 0) {
                if (frameCount >= lifetime)
                    return false;
            }
            // Otherwise the particle is destroyed when the animation is finished
            else if (anim.HasFinished()) { return false; }

            // Update the position in the screen
            position.x += speed.x;
            position.y += speed.y;

            if (collider != nullptr) { collider->SetPos(position.x, position.y, anim.GetWidth(), anim.GetHeight()); }
        }
    }
    
    return true;
}

void Particle::SetPositionX(const int _positionX) { position.x = _positionX; }
void Particle::SetPositionY(const int _positionY) { position.y = _positionY; }
int Particle::GetPositionX() const { return position.x; }
int Particle::GetPositionY() const { return position.y; }
void Particle::SetFSpeedY(const float _speedY) { speed.y = _speedY; };
void Particle::SetAnimPushBack(const SDL_Rect _rect) { anim.PushBack(_rect); }
SDL_Rect Particle::GetCurrentAnim() { return anim.GetCurrentFrame(); }
void Particle::SetAnimLoop(bool _loop) { anim.SetLoop(_loop); }
void Particle::SetAnimSpeed(float _speed) { anim.SetSpeed(_speed); }
bool Particle::CheckIsAlive() const { return isAlive; };
void Particle::SetFrameCount(const int _frameCount) { frameCount = _frameCount; }
void Particle::SetLifetime(const uint _lifetime) { lifetime = _lifetime; }
uint Particle::GetLifetime() const { return lifetime; }
void Particle::SetCollider(Collider* _collider) { collider = _collider; }
Collider* Particle::GetCollider() const { return collider; }
void Particle::SetParticleTexture(SDL_Texture* _particleTexture) { particleTexture = _particleTexture; }
SDL_Texture* Particle::GetParticleTexture() { return particleTexture; }