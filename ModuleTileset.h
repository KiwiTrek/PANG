#ifndef __MODULE_TILESET_H__
#define __MODULE_TILESET_H__

#include "Module.h"

class ModuleTileset : public Module {
public:
    //Constructor
    ModuleTileset(bool startEnabled);
    // Destructor
    ~ModuleTileset();
    // Called on application start.
    bool Init() override;
    //By now we will consider all modules to be permanently active
    bool Start() override;
    //Called at the beginning of each application loop
    UPDATE_STATUS PreUpdate() override;
    //Called at the middle of each application loop
    UPDATE_STATUS Update() override;
    //Called at the end of each application loop
    UPDATE_STATUS PostUpdate() override;
    // Called on application exit.
    bool CleanUp() override;

private:
};

#endif // !__MODULE_TILESET_H__