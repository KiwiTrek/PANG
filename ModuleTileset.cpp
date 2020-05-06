#include "ModuleTileset.h"
#include "Game.h"
#include "Globals.h"

ModuleTileset::ModuleTileset(bool startEnabled): Module(startEnabled){}
// Destructor
ModuleTileset::~ModuleTileset(){}
// Called on application start.
bool ModuleTileset::Init() { return true; }
//By now we will consider all modules to be permanently active
bool ModuleTileset::Start() { return true; }
//Called at the beginning of each application loop
UPDATE_STATUS ModuleTileset::PreUpdate() { return UPDATE_STATUS::UPDATE_CONTINUE; }
//Called at the middle of each application loop
UPDATE_STATUS ModuleTileset::Update() { return UPDATE_STATUS::UPDATE_CONTINUE; }
//Called at the end of each application loop
UPDATE_STATUS ModuleTileset::PostUpdate() { return UPDATE_STATUS::UPDATE_CONTINUE; }
// Called on application exit.
bool ModuleTileset::CleanUp() { return true; }