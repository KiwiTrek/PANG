#include "Collider.h"

Collider::Collider(SDL_Rect _rect, TYPE _type, Module* _listener) : rect(_rect), type(_type), listener(_listener) {}

void Collider::SetPos(int _x, int _y, int _w, int _h) {
    rect.x = _x;
    rect.y = _y;
    rect.w = _w;
    rect.h = _h;
}

bool Collider::Intersects(const SDL_Rect& r) const {
    return (rect.x < r.x + r.w &&
        rect.x + rect.w > r.x &&
        rect.y < r.y + r.h &&
        rect.h + rect.y > r.y);
}