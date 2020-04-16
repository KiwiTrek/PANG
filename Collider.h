#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL/include/SDL_Rect.h"

class Module;

struct Collider {
    enum TYPE {
        NONE = -1,
        WALL,
        FLOOR,
        STAIRS,
        PLAYER,
        BALLOON,
        ANIMAL,
        PLAYER_SHOT,
        MAX
    };

    //Methods
    Collider(SDL_Rect _rect, TYPE _type, Module* _listener = nullptr) : rect(_rect), type(_type), listener(_listener) {}

    void SetPos(int _x, int _y, int _w, int _h) {
        rect.x = _x;
        rect.y = _y;
        rect.w = _w;
        rect.h = _h;
    }

    bool Intersects(const SDL_Rect& r) const {
        return (rect.x < r.x + r.w &&
            rect.x + rect.w > r.x &&
            rect.y < r.y + r.h &&
            rect.h + rect.y > r.y);
    }

    bool CheckPendingToDelete() const { return pendingToDelete; }
    void SetPendingToDelete(bool _pendingToDelete) { pendingToDelete = _pendingToDelete; }
    SDL_Rect GetRect() { return rect; }
    TYPE GetType() { return type; }
    Module* GetListener() { return listener; }

private:
    SDL_Rect rect;
    bool pendingToDelete = false;
    TYPE type;
    Module* listener = nullptr;
};

#endif // !__COLLIDER_H__