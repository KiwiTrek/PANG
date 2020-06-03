#ifndef __MODULE_TILESET_H__
#define __MODULE_TILESET_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"

struct SDL_Texture;

class ModuleTileset : public Module {
public:
    enum class TileType {
        NONE = -1, //Unidentified -> El bugaso ~Abraham
        AIR,
        WALL,
        DESTRUCTIBLE1,
        DESTRUCTIBLE2,
        NOT_DESTRUCTIBLE,
        STAIRS,
        TOP_STAIRS
    };

    struct Tile {
        TileType id;
        SDL_Rect tileBlit;
		Collider* tileCollider;
    };

    //Constructor
    ModuleTileset(bool startEnabled);
    // Destructor
    ~ModuleTileset();
    // Called on application start.
    bool Init() override;
    //By now we will consider all modules to be permanently active
    bool Start() override;
    //Called at the end of each application loop
    UPDATE_STATUS PostUpdate() override;
    // Called on application exit.
    bool CleanUp() override;

    Tile GetLevelTile(int y, int x);


private:
    SDL_Texture* foreground = nullptr;
    int(*level)[48] = nullptr;
    Tile levelTiled[26][48] = {};
};

#endif // !__MODULE_TILESET_H__