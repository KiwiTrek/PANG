#ifndef __MODULE_H__
#define __MODULE_H__

#include "Globals.h"

struct Collider;

class Module {
public:
    Module(bool startEnabled);
    //Called at the beginning of the application execution
    virtual bool Init();
    //Called when the module is activated
    //By now we will consider all modules to be permanently active
    virtual bool Start();
    //Called at the beginning of each application loop
    virtual UPDATE_STATUS PreUpdate();
    //Called at the middle of each application loop
    virtual UPDATE_STATUS Update();
    //Called at the end of each application loop
    virtual UPDATE_STATUS PostUpdate();
    //Called at the end of the application
    virtual bool CleanUp();
    //Called when two colliders are intersecting
    //and the module is registered as the listener
    virtual void OnCollision(Collider* c1, Collider* c2);

    void Enable();
    void Disable();


    virtual bool GetInvertValue() const;
    virtual void ChangeInvert();
    inline bool IsEnabled() const;
    
private:
    bool invert = false;
    bool isEnabled = true;
};

#endif // __MODULE_H__