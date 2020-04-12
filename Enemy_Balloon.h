#ifndef __ENEMY_BALLOON_H__
#define __ENEMY_BALLOON_H__

#include "Enemy.h"

class Enemy_Balloon : public Enemy {
public:
    // Constructor (x y coordinates in the world)
    // Creates animation and movement data and the collider
    Enemy_Balloon(int x, int y);

    // The enemy is going to follow the different steps in the path
    // Position will be updated depending on the speed defined at each step
    void Update() override;

private:
    // This enemy has one sprite and one frame
    // We are keeping it an animation for consistency with other enemies
    Animation idle;
};

#endif // __ENEMY_BALLOON_H__