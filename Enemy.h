#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "p2Point.h"
#include "Animation.h"

struct SDL_Texture;
struct Collider;

enum ENEMY_TYPE {
    NO_TYPE = -1,
    CHUNGUS_BALLOON,
    NOT_THAT_MEH_BALLOON,
    MEH_BALLOON,
    SMOL_BALLOON,
    ANIMALS,
    MAX_ENEMY
};

class Enemy {
public:

    // Constructor
    // Saves the spawn position for later movement calculations
    Enemy(int x, int y);

    // Destructor
    virtual ~Enemy();

    // Returns the enemy's collider
    Collider* GetCollider() const;

    // Called from inhering enemies' Udpate
    // Updates animation and collider position
    virtual void Update();

    // Called from ModuleEnemies' Update
    virtual void Draw();

    // Collision response
    // Triggers an animation and a sound fx
    virtual void OnCollision(Collider* c2) = 0;

    void SetLethality(bool _lethality);
    bool GetLethality();
    ENEMY_TYPE GetEnemyType();
    void SetEnemyTexture(SDL_Texture* _texture);
    uint GetDestroyedFx();
    void SetDestroyedFx(uint _destroyedFx);
    int GetPositionX();
    int GetPositionY();

protected:
    // The current position in the world
    iPoint position;

    // The enemy's texture
    SDL_Texture* texture = nullptr;

    // Sound fx when destroyed
    uint destroyedFx = 0;

    // A ptr to the current animation
    Animation* currentAnim = nullptr;

    // The enemy's collider
    Collider* collider = nullptr;

    // A bool to know if the collision is lethal for the enemy
    bool lethal = false;

    ENEMY_TYPE type;

    // Original spawn position. Stored for movement calculations
    iPoint spawnPos;
};

#endif // __ENEMY_H__