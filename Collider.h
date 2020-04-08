#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_Rect.h"

class Module;

struct Collider {
    enum TYPE {
        NONE = -1,
        WALL,
        PLAYER,
        BALLOON,
        ANIMAL,
        PLAYER_SHOT,
        MAX
    };

    //Methods
    Collider(SDL_Rect _rect, TYPE _type, Module* _listener = nullptr);

    void SetPos(int _x, int _y, int _w, int _h);

    bool Intersects(const SDL_Rect& r) const;

    //Variables
    SDL_Rect rect;
    bool pendingToDelete = false;
    TYPE type;
    Module* listener = nullptr;
};


#endif // !__COLLIDER_H__