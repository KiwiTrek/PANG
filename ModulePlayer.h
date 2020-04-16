#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class ModulePlayer : public Module {
public:
    ModulePlayer(bool startEnabled);
    ~ModulePlayer();

    int GetSpeed() const;
    void SetSpeed(int _speed);
    iPoint GetPosition() const;
    void SetPosition(iPoint _position);
    SDL_Texture* GetTexture() const;
    Animation* GetCurrentAnimation() const;
    uint GetShotSoundIndex() const;
    void SetShotSoundIndex(uint _shotSoundIndex);
    void SetIfShot(bool _shot);
    bool CheckIfGodMode() const;
    bool CheckIfDestroyed() const;

    bool Start() override;
    UPDATE_STATUS Update() override;
    UPDATE_STATUS PostUpdate() override;

    // Collision callback, called when the player intersects with another collider
    void OnCollision(Collider* c1, Collider* c2) override;

private:
    iPoint position;
    int speed = 1;
    //Player Spritesheet
    SDL_Texture* texture = nullptr;

    //Player Animations
    Animation idle;
    Animation shoot;
    Animation moving;
    Animation ded;
    bool playerInvert = false;
    int returnToIdle = 0;

    // The pointer to the current player animation
    // It will be switched depending on the player's movement direction
    Animation* currentAnimation = nullptr;

    //Player Sound
    uint shotSoundIndex;
    uint dedSoundIndex;

    // The player's collider
    Collider* collider = nullptr;

    // A flag to detect when the player has been destroyed
    bool destroyed = false;
    bool once = true;

    // A flag to detect when the player has shot
    bool shot = false;

    // Cheat Code Enabler: God Mode
    bool godMode = false;

    // A countdown to when the player gets destroyed. After a while, the game exits
    uint destroyedCountdown = 120;
};

#endif //__MODULE_PLAYER_H__