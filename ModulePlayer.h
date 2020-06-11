#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "Physics.h"

struct SDL_Texture;
struct Collider;

enum class SHOT_TYPES {
    NORMAL,
    DWIRE,
    POWER,
    VULCAN
};

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
    SHOT_TYPES GetCurrentShotType() const;
    uint GetNormalShotSoundIndex() const;
    void SetNormalShotSoundIndex(uint _shotSoundIndex);
    uint GetVulcanShotSoundIndex() const;
    void SetVulcanShotSoundIndex(uint _shotSoundIndex);
    uint GetDedSoundIndex() const;
    void SetDedSoundIndex(uint _dedSoundIndex);
    int GetFontIndex()const;
	int GetTimerFontIndex()const;
    void IncreaseShoot();
    void DecreaseShoot();
    bool CheckIfGodMode() const;
    bool CheckIfDestroyed() const;
    void AddScore(int _score);
    int GetTimer() const;
    int GetPlayerLives() const;
    void SetPlayerLives(int lives);
    void SetScore(int _score);
    int GetBallTypeDebug() const;

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
    uint normalShotSoundIndex;
    uint vulcanShotSoundIndex;
    uint dedSoundIndex;

    // The player's collider
    Collider* collider = nullptr;

    //Le physics
    Physics physics;

    // A flag to detect when the player has been destroyed
    bool destroyed = false;
    bool isTimeOver = false;
    uint onceTimeIsOver = 0;
    //bool onceDeath = true;
    bool onceDeathSpaguett = true;
    bool once = true; // Given the QA work that i have been given here to avoid italian code (spaguetti code, chapuzadas) 12 bools are a direct insult to my being. - Abraham
    bool onceMusic = true; // Use flags in the near future
    bool onceHurry0 = true;
    bool onceHurry1 = true;
    bool onceHurry2 = true;
    bool onceSetNumShots = true;

    //Counter on the number of jumps dead player has made
    int deathJumps = 0;

    // A flag to detect when the player has shot
    int shot = 1;
    int maxShots = 1;
    SHOT_TYPES shotType = SHOT_TYPES::NORMAL;

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

    // Font score index
    int score = 0;
    int normalFont = -1;
    char playerText[10] = { "\0" };
    char scoreText[10] = { "\0" };

    int timerFont = -1;
    char timerText[10] = { "\0" };
    int timer = 0;

    float deltaTime = 1.0f / 60.0f;
    float time = -31.0f / 60.0f;

    //Player life
    int playerLifes = 2;
    SDL_Rect life;

    char cursorDebugText[25] = { "\0" };
    int ballTypeDebug = 0;
};

#endif //__MODULE_PLAYER_H__