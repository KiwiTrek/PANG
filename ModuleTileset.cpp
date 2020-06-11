#include "ModuleTileset.h"

#include "Game.h"

#include "ModulePlayer.h"
#include "ModuleCollisions.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

#include "ModuleLevelOne.h"
#include "ModuleLevelTwo.h"
#include "ModuleLevelThree.h"
#include "ModuleLevelFour.h"

#include "Globals.h"
#include "Tilesets.h"

ModuleTileset::ModuleTileset(bool startEnabled): Module(startEnabled){}
// Destructor
ModuleTileset::~ModuleTileset(){}
// Called on application start.
bool ModuleTileset::Init() {
    return true;
}
//By now we will consider all modules to be permanently active
bool ModuleTileset::Start() {
    if (game->GetModuleLevelOne()->IsEnabled()) {
        foreground = game->GetModuleTextures()->Load("Resources/Sprites/Foregrounds/Foreground1.png");
        level = level1;
    }
    else if (game->GetModuleLevelTwo()->IsEnabled()) {
        foreground = game->GetModuleTextures()->Load("Resources/Sprites/Foregrounds/Foreground2.png");
        level = level2;
    }
    else if (game->GetModuleLevelThree()->IsEnabled()) {
        foreground = game->GetModuleTextures()->Load("Resources/Sprites/Foregrounds/Foreground3.png");
        level = level3;
    }
    else if (game->GetModuleLevelFour()->IsEnabled()) {
        foreground = game->GetModuleTextures()->Load("Resources/Sprites/Foregrounds/Foreground4.png");
        level = level4;
    }
    else { return false; }

    for (int i = 0; i != 26; ++i) {
        for (int j = 0; j != 48; ++j) {
            switch (level[i][j]) {
            case 0:
                levelTiled[i][j] = {
                    TileType::AIR,
                    {i * TILE_SIZE,j * TILE_SIZE,TILE_SIZE,TILE_SIZE},
                    nullptr
                };
                break;
            case 1:
                levelTiled[i][j] = {
                    TileType::WALL,
                    {i * TILE_SIZE,j * TILE_SIZE,TILE_SIZE,TILE_SIZE},
                    nullptr
                };
                break;
            case 2:
                levelTiled[i][j] = {
                    TileType::DESTRUCTIBLE1,
                    {i * TILE_SIZE,j * TILE_SIZE,TILE_SIZE,TILE_SIZE},
                    game->GetModuleCollisions()->AddCollider({ j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE }, Collider::TYPE::FLOOR, (Module*)game->GetModuleTileset())
                };
                break;
            case 3:
                levelTiled[i][j] = {
                    TileType::DESTRUCTIBLE2,
                    {i * TILE_SIZE,j * TILE_SIZE,TILE_SIZE,TILE_SIZE},
					game->GetModuleCollisions()->AddCollider({ j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE }, Collider::TYPE::FLOOR, (Module*)game->GetModuleTileset())
                };
                break;
            case 4:
                levelTiled[i][j] = {
                    TileType::NOT_DESTRUCTIBLE,
                    {i * TILE_SIZE,j * TILE_SIZE,TILE_SIZE,TILE_SIZE},
					game->GetModuleCollisions()->AddCollider({ j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE }, Collider::TYPE::FLOOR, (Module*)game->GetModuleTileset())
                };
                break;
            case 5:
                levelTiled[i][j] = {
                    TileType::STAIRS,
                    {i * TILE_SIZE,j * TILE_SIZE,TILE_SIZE,TILE_SIZE},
					nullptr
                };
                break;
            case 6:
                levelTiled[i][j] = {
                    TileType::TOP_STAIRS,
                    {i * TILE_SIZE,j * TILE_SIZE,TILE_SIZE,TILE_SIZE},
					nullptr
                };
                break;
            default:
                break;
            }
        }
    }

    return true;
}

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

ModuleTileset::Tile ModuleTileset::GetLevelTile(int y, int x) { return levelTiled[y][x]; }

void ModuleTileset::OnCollision(Collider* c1, Collider* c2) {
    if (c2->GetType() == Collider::TYPE::PLAYER_SHOT && game->GetModulePlayer()->GetCurrentShotType() != SHOT_TYPES::VULCAN) {
        LOG("Wire collision!");
        if (GetLevelTile(c2->GetRect().y/TILE_SIZE, c2->GetRect().x/TILE_SIZE).id == TileType::DESTRUCTIBLE1 || GetLevelTile(c2->GetRect().y / TILE_SIZE, c2->GetRect().x / TILE_SIZE).id == TileType::DESTRUCTIBLE2) {
            LOG("It was a destructible!");
        }
    }
}