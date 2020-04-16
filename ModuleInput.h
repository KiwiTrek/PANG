#ifndef __MODULE_INPUT_H__
#define __MODULE_INPUT_H__

#include "Module.h"
#include "Globals.h"

#define MAX_KEYS 256

enum KEY_STATE {
    KEY_IDLE,
    KEY_DOWN,
    KEY_REPEAT,
    KEY_UP
};

class ModuleInput : public Module {
public:
    ModuleInput(bool startEnabled);
    ~ModuleInput();

    bool Init() override;
    UPDATE_STATUS PreUpdate() override;
    bool CleanUp() override;

    void SetKey(const KEY_STATE _key, int i);
    KEY_STATE GetKey(int i) const;

private:
    KEY_STATE keys[MAX_KEYS] = { KEY_IDLE };
};

#endif // ! __MODULE_INPUT_H__
