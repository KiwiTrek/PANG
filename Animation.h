#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 1000

class Animation {
public:
    bool loop = true; 
    float speed = 1.0f;
    SDL_Rect frames[MAX_FRAMES];

private:
    float current_frame = 0.0f;
    int total_frames = 0;
    int loop_count = 0;

public:
    
    float GetSpeed() const { return speed; }
    void SetSpeed(float _speed) { speed = _speed; }
    int GetHeight() { return frames[0].h; }
    int GetWidth() { return frames[0].w; }

    void PushBack(const SDL_Rect& rect) { frames[total_frames++] = rect; }
    void Reset() { current_frame = 0; }
    bool HasFinished() { return !loop && loop_count > 0; }
    void Update() {
        current_frame += speed;
        if (current_frame >= total_frames) {
            current_frame = (loop) ? 0.0f : total_frames - 1;
            ++loop_count;
        }
    }
    SDL_Rect& GetCurrentFrame() { return frames[(int)current_frame]; }
};

#endif