#ifndef __MODULE_INPUT_H__
#define __MODULE_INPUT_H__

#include "Module.h"
#include "Globals.h"

#define MAX_KEYS 256
#define MAX_PADS 4

struct _SDL_GameController;
struct _SDL_Haptic;

enum KEY_STATE {
    KEY_IDLE,
    KEY_DOWN,
    KEY_REPEAT,
    KEY_UP
};

struct GamePad
{
    //Input data
    bool start, back, guide;
    bool x, y, a, b, l1, r1, l3, r3;
    bool up, down, left, right;
    float l2, r2;
    float l_x, l_y, r_x, r_y, l_dz, r_dz;

    //Controller data
    bool enabled;
    int index;
    _SDL_GameController* controller;
    _SDL_Haptic* haptic;

    //Rumble controller
    int rumble_countdown;
    float rumble_strength;
};

class ModuleInput : public Module {
public:
    ModuleInput(bool startEnabled);
    ~ModuleInput();

    bool Init() override;
    UPDATE_STATUS PreUpdate() override;
    bool CleanUp() override;

    // Activates SDL device funcionallity when a gamepad has been connected
    void HandleDeviceConnection(int index);

    // Deactivates SDL device funcionallity when a gamepad has been disconnected
    void HandleDeviceRemoval(int index);

    // Called at PreUpdate
    // Iterates through all active gamepads and update all input data
    void UpdateGamepadsInput();

    bool ShakeController(int id, int duration, float strength = 0.5f);
    const char* GetControllerName(int id) const;


    void SetKey(const KEY_STATE _key, int i);
    KEY_STATE GetKey(int i) const;
    GamePad GetGamePad();
    GamePad GetGamePad(int i) const;

private:
    KEY_STATE keys[MAX_KEYS] = { KEY_IDLE };

    // An array to fill in all detected gamepads
    GamePad pads[MAX_PADS];
};

#endif // ! __MODULE_INPUT_H__
