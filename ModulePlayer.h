#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "Physics.h"

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
    SDL_Texture* GetBlueTextTexture() const;
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
    fPoint mruaSpeed;
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
    Animation* currentAnimation = &idle;

    //Player Sound
    uint shotSoundIndex;
    uint dedSoundIndex;

    // The player's collider
    Collider* collider = nullptr;

    //Le physics
    Physics physics;

    // A flag to detect when the player has been destroyed
    bool destroyed = false;
    bool once = true;
    bool onceMusic = true;

    // A flag to detect when the player has shot
    bool shot = false;

    // Cheat Code Enabler: God Mode
    bool godMode = false;

    // Countdown for when the player wins. After a while, the game changes screen
    uint winCountdown = 0;

    //Ready, Game Over & Time Out
    SDL_Rect ready;
    SDL_Rect gameOver;
    SDL_Rect timeOver;

    //Blue Text sprite sheet
    SDL_Texture* blueText = nullptr;
};

#endif //__MODULE_PLAYER_H__