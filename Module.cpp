#include "Module.h"

bool Module::Init() { return true; }

bool Module::Start() { return true; }

UPDATE_STATUS Module::PreUpdate() { return UPDATE_STATUS::UPDATE_CONTINUE; }

UPDATE_STATUS Module::Update() { return UPDATE_STATUS::UPDATE_CONTINUE; }

UPDATE_STATUS Module::PostUpdate() { return UPDATE_STATUS::UPDATE_CONTINUE; }

bool Module::CleanUp() { return true; }

void Module::OnCollision(Collider* c1, Collider* c2) {}

bool Module::GetInvertValue() const { return invert; };

void Module::ChangeInvert() {
    if (invert == true) { invert = false; }
    else if (invert == false) { invert = true; }
};
