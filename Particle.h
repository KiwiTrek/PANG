#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Animation.h"
#include "p2Point.h"

struct Collider;
struct SDL_Texture;

struct Particle
{
public:
    // Constructor
    Particle();

    // Copy constructor
    Particle(const Particle& p);

    // Destructor
    ~Particle();

    // Called in ModuleParticles' Update
    // Handles the logic of the particle
    // Returns false when the particle reaches its lifetime
    bool Update();

    void SetPositionX(const int _positionX);
    int GetPositionX() const;
    void SetPositionY(const int _positionY);
    int GetPositionY() const;
    void SetFSpeedY(const float _speedY);
    void SetAnimPushBack(const SDL_Rect _rect);
    SDL_Rect GetCurrentAnim();
    void SetAnimLoop(bool _loop);
    void SetAnimSpeed(float _speed);
    bool CheckIsAlive() const;
    void SetFrameCount(const int _frameCount);
    int GetFrameCount()const;
    void SetLifetime(const uint _lifetime);
    uint GetLifetime() const;
    void SetCollider(Collider* _collider);
    Collider* GetCollider() const;
    void SetParticleTexture(SDL_Texture* _particleTexture);
    SDL_Texture* GetParticleTexture();
    

private:
    // Defines the position in the screen
    iPoint position;

    // Defines the speed at which the particle will move (pixels per second)
    fPoint speed;

    // A set of rectangle sprites
    Animation anim;

    // Defines wether the particle is alive or not
    // Particles will be set to not alive until "spawnTime" is reached
    bool isAlive = false;

    // Defines the amout of frames this particle has been active
    // Negative values mean the particle is waiting to be activated
    int frameCount = 0;

    // Defines the total amount of frames during which the particle will be active
    uint lifetime = 0;

    // The particle's collider
    Collider* collider = nullptr;

    // A pointer to the particle's texture
    SDL_Texture* particleTexture = nullptr;
};

#endif //__PARTICLE_H__