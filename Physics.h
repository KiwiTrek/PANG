#ifndef __PHYSICS_H__
#define __PHYSICS_H__

struct Physics {
public:
    void UpdatePhysics(int& _x, int& _y, float& _sx, float& _sy) {
        if (axisX) { _x = _x + _sx * deltaTime; }
        if (axisY) {
            _y = _y + _sy * deltaTime + gravity * deltaTime * deltaTime;
            _sy = _sy + gravity * deltaTime;
        }
    }

    void SetAxis(bool _axisX, bool _axisY) {
        axisX = _axisX;
        axisY = _axisY;
    }

    int GetNextPositionX(int x, float sx) { return (x + sx * deltaTime); }
    int GetNextPositionY(int y, float sy) {
        sy = sy + gravity * deltaTime;
        return (y + sy * deltaTime + gravity * deltaTime * deltaTime);
    }

private:
    bool axisX;
    bool axisY;
    float gravity = 600.0f;
    float deltaTime = 1.0f / 60.0f;
};

#endif // !__COLLIDER_H__