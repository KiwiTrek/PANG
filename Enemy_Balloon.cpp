#include "Enemy_Balloon.h"

#include "Game.h"

#include "ModuleAudio.h"
#include "ModuleCollisions.h"
#include "ModuleEnemies.h"
#include "ModuleRender.h"
#include "ModuleTileset.h"

#include "ModuleLevelOne.h"
#include "ModuleLevelTwo.h"
#include "ModuleLevelThree.h"
#include "ModuleLevelFour.h"
#include "ModuleLevelFive.h"
#include "ModuleLevelSix.h"

#include "Enemy.h"

Enemy_Balloon::Enemy_Balloon(int x, int y, ENEMY_TYPE _type, bool _isMovingRight) : Enemy(x, y, _type, _isMovingRight) {
    if (GetEnemyType() == ENEMY_TYPE::CHUNGUS_BALLOON ) {
        if (game->GetModuleLevelOne()->IsEnabled() || game->GetModuleLevelFour()->IsEnabled()) { idle.PushBack({ 1,6,48,40 }); }
        else if (game->GetModuleLevelTwo()->IsEnabled() || game->GetModuleLevelFive()->IsEnabled()) { idle.PushBack({ 1,56,48,40 }); }
        else if (game->GetModuleLevelThree()->IsEnabled() || game->GetModuleLevelSix()->IsEnabled()) { idle.PushBack({ 1,105,48,40 }); }

        collider = game->GetModuleCollisions()->AddCollider({ 36,16,48,40 }, Collider::TYPE::BALLOON, (Module*)game->GetModuleEnemies());
    }
    if (GetEnemyType() == ENEMY_TYPE::NOT_THAT_MEH_BALLOON) {
        if (game->GetModuleLevelOne()->IsEnabled() || game->GetModuleLevelFour()->IsEnabled()) { idle.PushBack({ 52,13,33,27 }); }
        else if (game->GetModuleLevelTwo()->IsEnabled() || game->GetModuleLevelFive()->IsEnabled()) { idle.PushBack({ 52,63,33,27 }); }
        else if (game->GetModuleLevelThree()->IsEnabled() || game->GetModuleLevelSix()->IsEnabled()) { idle.PushBack({ 52,112,33,27 }); }

        collider = game->GetModuleCollisions()->AddCollider({ 52,13,33,27 }, Collider::TYPE::BALLOON, (Module*)game->GetModuleEnemies());
    }
    if (GetEnemyType() == ENEMY_TYPE::MEH_BALLOON) {
        if (game->GetModuleLevelOne()->IsEnabled() || game->GetModuleLevelFour()->IsEnabled()) { idle.PushBack({ 86,19,17,15 }); }
        else if (game->GetModuleLevelTwo()->IsEnabled() || game->GetModuleLevelFive()->IsEnabled()) { idle.PushBack({ 86,69,17,15 }); }
        else if (game->GetModuleLevelThree()->IsEnabled() || game->GetModuleLevelSix()->IsEnabled()) { idle.PushBack({ 86,118,17,15 }); }

        collider = game->GetModuleCollisions()->AddCollider({ 86,19,17,15 }, Collider::TYPE::BALLOON, (Module*)game->GetModuleEnemies());
    }
    if (GetEnemyType() == ENEMY_TYPE::SMOL_BALLOON) {
        if (game->GetModuleLevelOne()->IsEnabled() || game->GetModuleLevelFour()->IsEnabled()) { idle.PushBack({ 106,23,8,7 }); }
        else if (game->GetModuleLevelTwo()->IsEnabled() || game->GetModuleLevelFive()->IsEnabled()) { idle.PushBack({ 106,73,8,7 }); }
        else if (game->GetModuleLevelThree()->IsEnabled() || game->GetModuleLevelSix()->IsEnabled()) { idle.PushBack({ 106,122,8,7 }); }

        collider = game->GetModuleCollisions()->AddCollider({ 106,23,8,7 }, Collider::TYPE::BALLOON, (Module*)game->GetModuleEnemies());
    }
    currentAnim = &idle;
    if (isMovingRight) { speed.x = 119.5f; } // Es posible que el motivo por el cual empiece con mucha velocidad es porque tenga una velocidad inicial de serie
    else { speed.x = -59.25f; }
    speed.y = -40.0f;
    physics.SetAxis(true, true);
}

void Enemy_Balloon::Update() {
    // This is where the physics go
    physics.UpdatePhysics(position.x, position.y, speed.x, speed.y);

    // This is where the movement collisions go
    iPoint tile = { position.x / TILE_SIZE, position.y / TILE_SIZE };
    //LOG("Tile = %d, %d", tile.x, tile.y);
    if (tile.x < 1) {
        tile.x = 0;
    }
    if (tile.y < 1) {
        tile.y = 0;
    }

    //BOTTOMS

    if (GetEnemyType() == CHUNGUS_BALLOON) {
        for (int i = 1; i < 6; i++) {
            if (speed.y > 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y + 5, tile.x + i).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 5, tile.x + i).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 5, tile.x + i).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 5, tile.x + i).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.y = 2 * TILE_SIZE * (tile.y + 5) - currentAnim->GetHeight() * 2 - position.y;
                speed.y = -physics.CalculateSpeed( GetEnemyType(), position, speed, currentAnim->GetHeight(),(tile.y + 5) , TILE_SIZE * 3);
                break;
            }
        }
    }
    else if (GetEnemyType() == NOT_THAT_MEH_BALLOON) {
        for (int i = 1; i < 4; i++) {
            if (speed.y > 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y + 3, tile.x + i).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 3, tile.x + i).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 3, tile.x + i).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 3, tile.x + i).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.y = 2 * TILE_SIZE * (tile.y + 3) - currentAnim->GetHeight() * 2 - position.y;
                speed.y = -physics.CalculateSpeed( GetEnemyType(), position, speed, currentAnim->GetHeight(),(tile.y + 3) , TILE_SIZE * 10);
                break;
            }
        }
    }
    else if (GetEnemyType() == MEH_BALLOON) {
        for (int i = 1; i < 2; i++) {
            if (speed.y > 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y + 2, tile.x + i).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 2, tile.x + i).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 2, tile.x + i).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 2, tile.x + i).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.y = 2 * TILE_SIZE * (tile.y + 2) - currentAnim->GetHeight() * 2 - position.y;
                speed.y = -physics.CalculateSpeed( GetEnemyType(), position, speed, currentAnim->GetHeight(),(tile.y + 2) , TILE_SIZE * 14);
                break;
            }
        }
    }
    else if (GetEnemyType() == SMOL_BALLOON) {
        if (speed.y > 0 &&
            (game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::WALL &&
            game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::WALL)
            ||
            (game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE1 &&
            game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::DESTRUCTIBLE1)
            ||
            (game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE2 &&
            game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::DESTRUCTIBLE2)
            ||
            (game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE &&
            game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)
            && game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::AIR
            && game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::AIR) {
            if (position.x > 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x - 1).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x - 1).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x - 1).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x - 1).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.y = 2 * TILE_SIZE * (tile.y + 1) - currentAnim->GetHeight() * 2 - position.y;
                speed.y = -physics.CalculateSpeed(GetEnemyType(), position, speed, currentAnim->GetHeight(), (tile.y + 1), TILE_SIZE * 20);
            }
            else if (position.x <= 0) {
                position.y = 2 * TILE_SIZE * (tile.y + 1) - currentAnim->GetHeight() * 2 - position.y;
                speed.y = -physics.CalculateSpeed(GetEnemyType(), position, speed, currentAnim->GetHeight(), (tile.y + 1), TILE_SIZE * 20);
            }
        }
    }


    //TOPS
    if (GetEnemyType() == CHUNGUS_BALLOON) {
        for (int i = 1; i < 6; i++) {
            if (speed.y < 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + i).id == ModuleTileset::TileType::WALL ||
                    game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + i).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                    game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + i).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                    game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + i).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.y = 2 * TILE_SIZE * (tile.y + 1) - position.y;
                speed.y = physics.CalculateSpeed(GetEnemyType(), position, speed, currentAnim->GetHeight(), (tile.y + 5), TILE_SIZE * 3);
                break;
            }
        }
    }
    else if (GetEnemyType() == NOT_THAT_MEH_BALLOON) {
        for (int i = 1; i < 4; i++) {
            if (speed.y < 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + i).id == ModuleTileset::TileType::WALL ||
                    game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + i).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                    game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + i).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                    game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + i).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.y = 2 * TILE_SIZE * (tile.y + 1) - position.y;
                speed.y = physics.CalculateSpeed(GetEnemyType(), position, speed, currentAnim->GetHeight(), (tile.y + 3), TILE_SIZE * 10);
                break;
            }
        }
    }
    else if (GetEnemyType() == MEH_BALLOON) {
        for (int i = 1; i < 2; i++) {
            if (speed.y < 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + i).id == ModuleTileset::TileType::WALL ||
                    game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + i).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                    game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + i).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                    game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + i).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.y = 2 * TILE_SIZE * (tile.y + 1) - position.y;
                speed.y = physics.CalculateSpeed(GetEnemyType(), position, speed, currentAnim->GetHeight(), (tile.y + 2), TILE_SIZE * 14);
                break;
            }
        }
    }
    else if (GetEnemyType() == SMOL_BALLOON) {
        if (speed.y < 0 &&
            (game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::WALL &&
            game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::WALL)
            ||
            (game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE1 &&
            game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::DESTRUCTIBLE1)
            ||
            (game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE2 &&
            game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::DESTRUCTIBLE2)
            ||
            (game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE &&
            game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)
            && game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::AIR
            && game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::AIR) {
            if (position.x > 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y, tile.x - 1).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x - 1).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x - 1).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x - 1).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.y = 2 * TILE_SIZE * (tile.y + 1) - position.y;
                speed.y = physics.CalculateSpeed(GetEnemyType(), position, speed, currentAnim->GetHeight(), (tile.y + 1), TILE_SIZE * 20);
            }
            else if (position.x <= 0) {
                position.y = 2 * TILE_SIZE * (tile.y + 1) - position.y;
                speed.y = physics.CalculateSpeed(GetEnemyType(), position, speed, currentAnim->GetHeight(), (tile.y + 1), TILE_SIZE * 20);
            }
        }
    }

    //LEFTS                                                                                                     {x-1T, y-1T, w+2, h+2}

    if (GetEnemyType() == CHUNGUS_BALLOON) {
        for (int i = 1; i < 5; i++) {
            if (speed.x < 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::WALL ||
                    game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                    game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                    game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.x = 2 * TILE_SIZE * (tile.x + 1) - position.x;
                speed.x = 119.5f;
                break;
            }
        }
    }
    else if (GetEnemyType() == NOT_THAT_MEH_BALLOON) {
        for (int i = 1; i < 3; i++) {
            if (speed.x < 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::WALL ||
                    game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                    game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                    game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.x = 2 * TILE_SIZE * (tile.x + 1) - position.x;
                speed.x = 119.5f;
                break;
            }
        }
    }
    else if (GetEnemyType() == MEH_BALLOON) {
        for (int i = 1; i < 2; i++) {
            if (speed.x < 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::WALL ||
                    game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                    game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                    game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.x = 2 * TILE_SIZE * (tile.x + 1) - position.x;
                speed.x = 119.5f;
                break;
            }
        }
    }
    else if (GetEnemyType() == SMOL_BALLOON) {
        if (speed.x < 0 &&
            (game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)
            &&
            (game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
            // top left corner
            if (game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE) {
                position.x = 2 * TILE_SIZE * (tile.x + 1) - position.x;
                speed.x = 119.5f;
                position.y = 2 * TILE_SIZE * (tile.y + 1) - currentAnim->GetHeight() * 2 - position.y;
                speed.y = physics.CalculateSpeed(GetEnemyType(), position, speed, currentAnim->GetHeight(), (tile.y + 1), TILE_SIZE * 20);
                LOG("CANTONADA TOP LEFT PositionY = %d, SpeedY = %f\n", position.y, speed.y);
            }
            // bottom left corner
            else if (game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE) {
                position.x = 2 * TILE_SIZE * (tile.x + 1) - position.x;
                speed.x = 119.5f;
                position.y = 2 * TILE_SIZE * (tile.y + 1) - position.y;
                speed.y = -physics.CalculateSpeed(GetEnemyType(), position, speed, currentAnim->GetHeight(), (tile.y + 1), TILE_SIZE * 20);
                position.y = 2 * TILE_SIZE * tile.y - currentAnim->GetHeight() * 2 - position.y;
                speed.y = -physics.CalculateSpeed(GetEnemyType(), position, speed, currentAnim->GetHeight(), (tile.y + 1), TILE_SIZE * 20);
                LOG("CANTONADA BOTTOM LEFT PositionY = %d, SpeedY = %f\n", position.y, speed.y);
            }
            //left wall
            else {
                LOG("ESQUERRA\n");
                position.x = 2 * TILE_SIZE * (tile.x + 1) - position.x;
                speed.x = 119.5f;
            }
        }
    }

    //RIGHTS
    if (GetEnemyType() == CHUNGUS_BALLOON) {
        for (int i = 1; i < 5; i++) {
            if (speed.x > 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 6).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 6).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 6).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 6).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.x = 2 * TILE_SIZE * (tile.x + 6) - currentAnim->GetWidth() * 2 - position.x;
                speed.x = -59.25f;
                break;
            }
        }
    }
    else if (GetEnemyType() == NOT_THAT_MEH_BALLOON) {
        for (int i = 1; i < 3; i++) {
            if (speed.x > 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 4).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 4).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 4).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 4).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.x = 2 * TILE_SIZE * (tile.x + 4) - currentAnim->GetWidth() * 2 - position.x;
                speed.x = -59.25f;
                break;
            }
        }
    }
    else if (GetEnemyType() == MEH_BALLOON) {
        for (int i = 1; i < 2; i++) {
            if (speed.x > 0 &&
                (game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 2).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 2).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 2).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 2).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                position.x = 2 * TILE_SIZE * (tile.x + 2) - currentAnim->GetWidth() * 2 - position.x;
                speed.x = -59.25f;
                break;
            }
        }
    }
    else if (GetEnemyType() == SMOL_BALLOON) {
        if (speed.x > 0 &&
            (game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x + 1).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)
            &&
            (game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x + 1).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
            // top right corner
            if (game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE) {
                position.x = 2 * TILE_SIZE * (tile.x + 1) - currentAnim->GetWidth() * 2 - position.x;
                speed.x = -59.25f;
                position.y = 2 * TILE_SIZE * (tile.y + 1) - currentAnim->GetHeight() * 2 - position.y;
                speed.y = physics.CalculateSpeed(GetEnemyType(), position, speed, currentAnim->GetHeight(), (tile.y + 1), TILE_SIZE * 20);
                LOG("CANTONADA TOP RIGHT PositionY = %d, SpeedY = %f\n", position.y, speed.y);
            }
            // bottom right corner
            else if (game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                game->GetModuleTileset()->GetLevelTile(tile.y + 1, tile.x).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE) {
                position.x = 2 * TILE_SIZE * (tile.x + 1) - currentAnim->GetWidth() * 2 - position.x;
                speed.x = -59.25f;
                position.y = 2 * TILE_SIZE * tile.y - position.y;
                speed.y = -physics.CalculateSpeed(GetEnemyType(), position, speed, currentAnim->GetHeight(), (tile.y + 1), TILE_SIZE * 20);
                LOG("CANTONADA BOTTOM RIGHT PositionY = %d, SpeedY = %f\n", position.y, speed.y);
            }
            //right wall
            else {
                LOG("DRETA\n");
                position.x = 2 * TILE_SIZE * (tile.x + 1) - currentAnim->GetWidth() * 2 - position.x;
                speed.x = -59.25f;
            }
        }
    }

    //LOG("PositionY = %d\n", position.y);
    // Call to the base class. It must be called at the end
    // It will update the collider depending on the position
    Enemy::Update();
}

void Enemy_Balloon::OnCollision(Collider* c1, Collider* c2) { if (c2->GetType() == Collider::TYPE::PLAYER_SHOT) { SetLethality(true); } }