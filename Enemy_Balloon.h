#ifndef __ENEMY_BALLOON_H__
#define __ENEMY_BALLOON_H__

#include "Enemy.h"
#include "Physics.h"

class Enemy_Balloon : public Enemy {
public:
    // Constructor (x y coordinates in the world)
    // Creates animation and movement data and the collider
    Enemy_Balloon(int x, int y, ENEMY_TYPE _type, bool _isMovingRight);

    // The enemy is going to follow the different steps in the path
    // Position will be updated depending on the speed defined at each step
    void Update() override;
    void Enemy::OnCollision(Collider* c1, Collider* c2) override;

private:
    // This enemy has one sprite and one frame
    // We are keeping it an animation for consistency with other enemies
    Animation idle;
    fPoint speed;
    Physics physics;
};

#endif // __ENEMY_BALLOON_H__