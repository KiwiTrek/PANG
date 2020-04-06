#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 25

class Animation {
private:
    float current_frame = 0.0f;
    int last_frame = 0;
    float speed = 1.0f;
    SDL_Rect frames[MAX_FRAMES];

public:

    float GetSpeed() const { return speed; }
    void SetSpeed(float _speed) { speed = _speed; }
    SDL_Rect& GetCurrentFrame() { return frames[(int)current_frame]; }

    void PushBack(const SDL_Rect& rect) { frames[last_frame++] = rect; }
    void Reset() { current_frame = 0; }

    void Update() {
        current_frame += speed;
        if (current_frame >= last_frame) { current_frame = 0; }
    }
};

#endif