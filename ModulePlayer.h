#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "Module.h"
#include "p2Point.h"

struct SDL_Texture;

class ModulePlayer : public Module {
public:
    ModulePlayer();
    ~ModulePlayer();

    int GetSpeed() const;
    void SetSpeed(int _speed);
    iPoint GetPosition() const;
    void SetPosition(iPoint _position);
    SDL_Texture* GetTexture() const;

    bool Start() override;
    UPDATE_STATUS Update() override;
    UPDATE_STATUS PostUpdate() override;

private:
    iPoint position;
    int speed = 1;
    //Player Spritesheet
    SDL_Texture* texture = nullptr;
};

#endif //__MODULE_PLAYER_H__