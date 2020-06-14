#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "p2Point.h"

struct Physics {
public:
    void UpdatePhysics(int& _x, int& _y, float& _sx, float& _sy) {
        if (axisX) { _x = _x + _sx * deltaTime; }
        if (axisY) {
            _sy = _sy + gravity * deltaTime;
            _y = _y + _sy * deltaTime + gravity * deltaTime * deltaTime / 2;
        }
    }
    bool CheckMovementCollision(SDL_Rect A, SDL_Rect B) {
        //The sides of the rectangles
        int leftA, leftB;
        int rightA, rightB;
        int topA, topB;
        int bottomA, bottomB;

        //Calculate the sides of rect A
        leftA = A.x;
        rightA = A.x + A.w;
        topA = A.y;
        bottomA = A.y + A.h;

        //Calculate the sides of rect B
        leftB = B.x;
        rightB = B.x + B.w;
        topB = B.y;
        bottomB = B.y + B.h;
        //If any of the sides from A are outside of B
        if (bottomA <= topB) { return false; }

        if (topA >= bottomB) { return false; }

        if (rightA <= leftB) { return false; }

        if (leftA >= rightB) { return false; }

        //If none of the sides from A are outside B
        return true;
    }
    float CalculateSpeed(int _enemyType, iPoint _position, fPoint _speed, int _height,int wallH , int maxH) {
        float maxSpeedY = 0;
        float result = 0;
        if (_enemyType == 0) {
            maxSpeedY = _speed.sqrtf(2 * gravity * ((TILE_SIZE * 25) - _height - maxH));
            result = _speed.sqrtf(maxSpeedY * maxSpeedY - 2 * gravity * ((TILE_SIZE * 25) - _height - _position.y));
        }
        else {
            maxSpeedY = _speed.sqrtf(2 * gravity * ((TILE_SIZE * 25) - _height - maxH));
            result = _speed.sqrtf(maxSpeedY * maxSpeedY - 2 * gravity * ((TILE_SIZE * wallH) - _height - _position.y));
        }
        //LOG("MaxSpeedY = %f, SpeedY = %f, PositionY = %d, EnemyType = %d\n", maxSpeedY, result, _position.y, _enemyType);
        return result;
    }

    void SetAxis(bool _axisX, bool _axisY) {
        axisX = _axisX;
        axisY = _axisY;
    }
    float GetGravity() const { return gravity; }
    float GetDeltaTime() const { return deltaTime; }
private:
    bool axisX;
    bool axisY;
    float gravity = 600.0f;
    float deltaTime = 1.0f / 60.0f;
};

#endif // !__COLLIDER_H__