#include "ModuleTileset.h"

#include "Game.h"

#include "ModuleCollisions.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleLevelOne.h"
//#include "ModuleLevelTwo.h"

#include "Globals.h"

ModuleTileset::ModuleTileset(bool startEnabled): Module(startEnabled){}
// Destructor
ModuleTileset::~ModuleTileset(){}
// Called on application start.
bool ModuleTileset::Init() {
    return true;
}
//By now we will consider all modules to be permanently active
bool ModuleTileset::Start() {
    if (game->GetModuleLevelOne()->IsEnabled) {
		foreground = game->GetModuleTextures()->Load("Resources/Sprites/Foreground1.png");
        for (int i = 0; i != 26; ++i) {
            for (int j = 0; j != 48; ++j) {
                switch (level[i][j]) {
                case 0:
                    levelTiled[i][j] = {
                        TileType::AIR,
                        {i * TILE_SIZE,j * TILE_SIZE,TILE_SIZE,TILE_SIZE},
                        game->GetModuleCollisions()->AddCollider(levelTiled[i][j].tileBlit,Collider::TYPE::NONE)
                    };
                    break;
                case 1:
                    levelTiled[i][j] = {
                        TileType::WALL,
                        {i * TILE_SIZE,j * TILE_SIZE,TILE_SIZE,TILE_SIZE},
                        game->GetModuleCollisions()->AddCollider(levelTiled[i][j].tileBlit,Collider::TYPE::WALL)
                    };
                    break;
                default:
                    break;
                }
            }
        }
    }
    /*else if (game->GetModuleLevelTwo()->IsEnabled)*/
    else { return false; }
    return true;
}
//Called at the beginning of each application loop
UPDATE_STATUS ModuleTileset::PreUpdate() { return UPDATE_STATUS::UPDATE_CONTINUE; }
//Called at the middle of each application loop
UPDATE_STATUS ModuleTileset::Update() { return UPDATE_STATUS::UPDATE_CONTINUE; }
//Called at the end of each application loop
UPDATE_STATUS ModuleTileset::PostUpdate() { 
	for (int y = 0; y < 26; y++) {
		for (int x = 0; x < 48; x++) {
			if (level[y][x] > 1) {
				SDL_Rect eightBit = { x * TILE_SIZE,y * TILE_SIZE,TILE_SIZE,TILE_SIZE };
				game->GetModuleRender()->Blit(foreground, x * TILE_SIZE, y * TILE_SIZE, false, &eightBit);
			}
		}
	}

	return UPDATE_STATUS::UPDATE_CONTINUE; 
}
// Called on application exit.
bool ModuleTileset::CleanUp() { return true; }