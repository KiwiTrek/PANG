#include "ModulePlayer.h"

#include "Game.h"

#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleEnemies.h"
#include "ModuleParticles.h"
#include "ModuleCollisions.h"
#include "ModuleTransition.h"
#include "ModuleTileset.h"
#include "ModuleFonts.h"
#include "ModuleWinScreen.h"

#include "Particle.h"

#include "ModuleLevelOne.h"
#include "ModuleLevelTwo.h"
#include "ModuleLevelThree.h"
#include "ModuleLevelFour.h"
#include "ModuleLevelFive.h"
#include "ModuleLevelSix.h"

#include "SDL/include/SDL_timer.h"
#include "SDL/include/SDL_scancode.h"
#include <stdio.h>

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled) {
    //Animation setter
    idle.PushBack({ 12,112,25,32 }); //x,y,w,h
    idle.SetSpeed(0.0f);

    shoot.PushBack({ 44,112,27,32 });
    shoot.SetSpeed(0.0f);

    moving.PushBack({ 12,2,29,32 });
    moving.PushBack({ 46,2,30,32 });
    moving.PushBack({ 80,2,29,32 });
    moving.PushBack({ 114,2,27,32 });
    moving.PushBack({ 148,2,29,32 });
    moving.SetSpeed(0.2f);

    ded.PushBack({ 147,80,31,29 });
    ded.PushBack({ 81,112,40,29 });
    ded.SetSpeed(0.2f);
    ded.SetLoop(false);

    climbing.PushBack({ 13,36,26,32 });
    climbing.PushBack({ 50,36,26,32 });
    climbing.PushBack({ 81,36,26,32 });
    climbing.PushBack({ 115,36,26,32 });
    climbing.SetSpeed(0.1f);

    gameOver = { 5,5,532,58 };
    timeOver = { 5,69,532,58 };
    ready = { 5,132,203,67 };
    life = { 154,44,16,16 };
}
ModulePlayer::~ModulePlayer() {}

bool ModulePlayer::Start() {
    LOG("Loading player textures");
    destroyed = false;
    isTimeOver = false;
    onceTimeIsOver = 0;
    once = true;
    onceMusic = true;
    onceHurry0 = true;
    onceHurry1 = true;
    onceHurry2 = true;
    godMode = false;
    //onceDeath = true;
    deathJumps = 0;
    onceDeathSpaguett = true;
    shot = 1;
    maxShots = 1;
    winCountdown = 0;
    shotType = SHOT_TYPES::NORMAL;
    onceSetNumShots = true;

    blueText = game->GetModuleTextures()->Load("Resources/Sprites/blueText.png");
    texture = game->GetModuleTextures()->Load("Resources/Sprites/player.png"); // arcade version
    powerUpIcons = game->GetModuleTextures()->Load("Resources/Sprites/powerUpIcons.png");
    normalShotSoundIndex = game->GetModuleAudio()->LoadFx("Resources/SFX/shotClaw.wav");
    vulcanShotSoundIndex = game->GetModuleAudio()->LoadFx("Resources/SFX/shotgun.wav");
    dedSoundIndex = game->GetModuleAudio()->LoadFx("Resources/SFX/dead.wav");

    moving.Reset();
    ded.Reset();

    position.x = (TILE_SIZE * 20) + 2;
    position.y = game->GetModuleLevelOne()->GetBackgroundAdapter().h - TILE_SIZE - idle.GetHeight();
    mruaSpeed = { 0,0 };
    physics.SetAxis(false, false); // physics

    collider = game->GetModuleCollisions()->AddCollider({ position.x, position.y, idle.GetWidth(), idle.GetHeight() }, Collider::TYPE::PLAYER, this); // adds a collider to the player

    normalFont = game->GetModuleFonts()->Load("Resources/Sprites/Font.png", "a ',.0123456789:bcdefghijklmnopqrstuvwxyz-", 6);
    timerFont = game->GetModuleFonts()->Load("Resources/Sprites/Timer.png", "time:0123456789 ", 2);
    timer = 100;
    time = -31.0f / 60.0f;

    return true;
}

UPDATE_STATUS ModulePlayer::Update()
{
    GamePad& pad = game->GetModuleInput()->GetGamePad(0);
    keyPressed = false;
    if (!destroyed) {
        if (returnToIdle == 0 &&
            (game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 4, position.x / TILE_SIZE + 1).id == ModuleTileset::TileType::WALL ||
                game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 1, position.x / TILE_SIZE + 1).id == ModuleTileset::TileType::AIR
                )) {
            LOG("Fucking Finally");
            currentAnimation = &idle;
        }
        else {
            LOG("I want to die");
            if (returnToIdle != 0) { --returnToIdle; }
        }
    }

    if (game->GetModuleLevelOne()->CheckIfStarted() ||
        game->GetModuleLevelTwo()->CheckIfStarted() ||
        game->GetModuleLevelThree()->CheckIfStarted() ||
        game->GetModuleLevelFour()->CheckIfStarted() ||
        game->GetModuleLevelFive()->CheckIfStarted() ||
        game->GetModuleLevelSix()->CheckIfStarted()) {
        //Reset the currentAnimation back to idle before updating the logic
        if (!destroyed && !isTimeOver) {
            if (((game->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && game->GetModuleInput()->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
                || (pad.right && !pad.left)
                || pad.l_x > 0)
                && (returnToIdle == 0)
                && (game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 4, position.x / TILE_SIZE + 1).id == ModuleTileset::TileType::WALL ||
                    game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 1, position.x / TILE_SIZE + 1).id == ModuleTileset::TileType::AIR)) {
                keyPressed = true;
                currentAnimation = &moving;
                if (GetInvertValue()) { ChangeInvert(); }
                position.x += speed;
            }
            if (((game->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && game->GetModuleInput()->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
                || (pad.left && !pad.right)
                || pad.l_x < 0)
                && (returnToIdle == 0)
                && (game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 4, position.x / TILE_SIZE + 1).id == ModuleTileset::TileType::WALL ||
                    game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 1, position.x / TILE_SIZE + 1).id == ModuleTileset::TileType::AIR)) {
                keyPressed = true;
                currentAnimation = &moving;
                if (!(GetInvertValue())) { ChangeInvert(); }
                position.x -= speed;
            }

            if ((game->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT 
                && game->GetModuleInput()->GetKey(SDL_SCANCODE_S) != KEY_REPEAT)
                || (pad.up && !pad.down)
                && pad.l_y < 0
                &&(returnToIdle == 0)) {
                for (int i = 0; i < 4; i++) {
                    keyPressed = true;
                    if ((game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 3, position.x / TILE_SIZE).id == ModuleTileset::TileType::STAIRS
                        && game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 3, position.x / TILE_SIZE + 1).id == ModuleTileset::TileType::STAIRS
                        && game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 3, position.x / TILE_SIZE + 2).id == ModuleTileset::TileType::STAIRS
                        && game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 2, position.x / TILE_SIZE).id == ModuleTileset::TileType::STAIRS)) {
                        currentAnimation = &climbing;
                        if (!(GetInvertValue())) { ChangeInvert(); }
                        position.y -= speed;
                        position.x = (position.x/TILE_SIZE)* TILE_SIZE;
                        break;
                    }
                    else if (game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 3, position.x / TILE_SIZE + 1).id == ModuleTileset::TileType::STAIRS
                        && game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 3, position.x / TILE_SIZE + 2).id == ModuleTileset::TileType::STAIRS
                        && game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 3, position.x / TILE_SIZE + 3).id == ModuleTileset::TileType::STAIRS
                        && game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 2, position.x / TILE_SIZE + 1).id == ModuleTileset::TileType::STAIRS) {
                        currentAnimation = &climbing;
                        if (!(GetInvertValue())) { ChangeInvert(); }
                        position.y -= speed;
                        if (game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 3, position.x / TILE_SIZE).id == ModuleTileset::TileType::STAIRS) {
                            position.x = (position.x / TILE_SIZE) * TILE_SIZE;
                        }
                        else {
                            position.x = ((position.x / TILE_SIZE) + 1) * TILE_SIZE;
                        }
                        break;
                    }
                }
            }
            if ((game->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT
                && game->GetModuleInput()->GetKey(SDL_SCANCODE_W) != KEY_REPEAT)
                || (pad.down && !pad.up)
                && pad.l_y > 0
                && (returnToIdle == 0)) {
                keyPressed = true;
                for (int i = 0; i < 4; i++) {
                    if (game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 3, position.x / TILE_SIZE + i).id == ModuleTileset::TileType::STAIRS
                        && game->GetModuleTileset()->GetLevelTile(position.y / TILE_SIZE + 4, position.x / TILE_SIZE + i).id == ModuleTileset::TileType::STAIRS) {
                        currentAnimation = &climbing;
                        if (!(GetInvertValue())) { ChangeInvert(); }
                        position.y += speed;
                        break;
                    }
                }
            }

            if ((game->GetModuleInput()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || pad.x || pad.b || pad.r2 > 0)
                && shot >= 1) {
                shot--;
                currentAnimation = &shoot;
                switch (shotType) {
                case SHOT_TYPES::NORMAL: {
                    if (onceSetNumShots) {
                        onceSetNumShots = false;
                        shot = 0;
                        maxShots = 1;
                    }
                    if (!godMode) { game->GetModuleAudio()->PlayFx(normalShotSoundIndex); }
                    if (GetInvertValue()) {
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->normalWire, position.x + (shoot.GetWidth() / 3) - 2, position.y - 1, Collider::TYPE::PLAYER_SHOT);
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->muzzleFlash, position.x + 3, position.y - 10, Collider::TYPE::NONE);
                    }
                    else {
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->normalWire, position.x + (shoot.GetWidth() / 2) - 2, position.y - 1, Collider::TYPE::PLAYER_SHOT);
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->muzzleFlash, position.x + currentAnimation->GetWidth() / 2 - 6, position.y - 10, Collider::TYPE::NONE); //It works, it just works ~Todd Howard,from Skyrim
                    }
                    break;
                }
                case SHOT_TYPES::DWIRE: {
                    if (!godMode) { game->GetModuleAudio()->PlayFx(normalShotSoundIndex); }
                    if (onceSetNumShots) {
                        onceSetNumShots = false;
                        shot = 1;
                        maxShots = 2;
                    }
                    if (GetInvertValue()) {
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->normalWire, position.x + (shoot.GetWidth() / 3) - 2, position.y - 1, Collider::TYPE::PLAYER_SHOT);
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->muzzleFlash, position.x + 3, position.y - 10, Collider::TYPE::NONE);
                    }
                    else {
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->normalWire, position.x + (shoot.GetWidth() / 2) - 2, position.y - 1, Collider::TYPE::PLAYER_SHOT);
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->muzzleFlash, position.x + currentAnimation->GetWidth() / 2 - 6, position.y - 10, Collider::TYPE::NONE); //It works, it just works ~Todd Howard,from Skyrim
                    }
                    break;
                }
                case SHOT_TYPES::POWER: {
                    if (!godMode) { game->GetModuleAudio()->PlayFx(normalShotSoundIndex); }
                    if (onceSetNumShots) {
                        onceSetNumShots = false;
                        shot = 0;
                        maxShots = 1;
                    }
                    if (GetInvertValue()) {
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->powerWire, position.x + (shoot.GetWidth() / 3) - 2, position.y - 1, Collider::TYPE::PLAYER_SHOT);
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->muzzleFlash, position.x + 3, position.y - 10, Collider::TYPE::NONE);
                    }
                    else {
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->powerWire, position.x + (shoot.GetWidth() / 2) - 2, position.y - 1, Collider::TYPE::PLAYER_SHOT);
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->muzzleFlash, position.x + currentAnimation->GetWidth() / 2 - 6, position.y - 10, Collider::TYPE::NONE); //It works, it just works ~Todd Howard,from Skyrim
                    }
                    break;
                }
                case SHOT_TYPES::VULCAN: {
                    if (!godMode) { game->GetModuleAudio()->PlayFx(vulcanShotSoundIndex); }
                    if (onceSetNumShots) {
                        onceSetNumShots = false;
                        shot = 25;
                        maxShots = 25;
                    }
                    if (GetInvertValue()) {
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->powerShot, position.x + (shoot.GetWidth() / 3) - 2, position.y - 1, Collider::TYPE::PLAYER_SHOT);
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->muzzleFlash, position.x + 3, position.y - 10, Collider::TYPE::NONE);
                    }
                    else {
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->powerShot, position.x + (shoot.GetWidth() / 2) - 2, position.y - 1, Collider::TYPE::PLAYER_SHOT);
                        game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->muzzleFlash, position.x + currentAnimation->GetWidth() / 2 - 6, position.y - 10, Collider::TYPE::NONE); //It works, it just works ~Todd Howard,from Skyrim
                    }
                    break;
                }
                }
                returnToIdle = 12;
            }
            if (currentAnimation == &idle) {
                if (GetInvertValue()) { collider->SetPos(position.x + 7, position.y + 6, idle.GetWidth() - 13, idle.GetHeight() - 6); }
                else { collider->SetPos(position.x + 6, position.y + 6, idle.GetWidth() - 13, idle.GetHeight() - 6); }
            }
            else if (currentAnimation == &moving) {
                if (GetInvertValue()) { collider->SetPos(position.x + 10, position.y + 4, moving.GetWidth() - 16, moving.GetHeight() - 4); }
                else { collider->SetPos(position.x + 6, position.y + 4, moving.GetWidth() - 16, moving.GetHeight() - 4); }
            }
            else if (currentAnimation == &shoot) {
                if(GetInvertValue()){ collider->SetPos(position.x + 8, position.y + 9, shoot.GetWidth() - 16, shoot.GetHeight() - 9); }
                else{ collider->SetPos(position.x + 8, position.y + 9, shoot.GetWidth() - 16, shoot.GetHeight() - 9); }
            }
            else if (currentAnimation == &climbing) {
                collider->SetPos(position.x + 8, position.y + 6, shoot.GetWidth() - 14, shoot.GetHeight() - 6);
            }
        }

        //Debug
        if (game->GetModuleInput()->GetCursorState() == 1) {
            if (game->GetModuleInput()->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
                ballTypeDebug = 0; //CHUNGUS
            }
            if (game->GetModuleInput()->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
                ballTypeDebug = 1; //NOT THAT MEH
            }
            if (game->GetModuleInput()->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
                ballTypeDebug = 2; //MEH
            }
            if (game->GetModuleInput()->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
                ballTypeDebug = 3; //SMOL
            }
        }
        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
            onceSetNumShots = true;
            switch (shotType) {
            case SHOT_TYPES::NORMAL: {
                shotType = SHOT_TYPES::DWIRE;
                break;
            }
            case SHOT_TYPES::DWIRE: {
                shotType = SHOT_TYPES::POWER;
                break;
            }
            case SHOT_TYPES::POWER: {
                shotType = SHOT_TYPES::VULCAN;
                break;
            }
            case SHOT_TYPES::VULCAN: {
                shotType = SHOT_TYPES::NORMAL;
                break;
            }
            }
        }
        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) { godMode = !godMode; }
        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) { destroyed = !destroyed; }
        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
            //!puppy
            switch (game->GetModuleWinScreen()->GetCurrentLevel()) {
            case 1: {
                game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelOne(), (Module*)game->GetModuleProjectSheet(), 90);
                break;
            }
            case 2: {
                game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelTwo(), (Module*)game->GetModuleProjectSheet(), 90);
                break;
            }
            case 3: {
                game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelThree(), (Module*)game->GetModuleProjectSheet(), 90);
                break;
            }
            case 4: {
                game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFour(), (Module*)game->GetModuleProjectSheet(), 90);
            }
            case 5: {
                game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFive(), (Module*)game->GetModuleProjectSheet(), 90);
            }
            case 6: {
                game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelSix(), (Module*)game->GetModuleProjectSheet(), 90);
            }
            default: {
                break;
            }
            }
        }
        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
            switch (game->GetModuleWinScreen()->GetCurrentLevel()) {
            case 1: {
                game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelOne(), (Module*)game->GetModuleWinScreen(), 4);
                break;
            }
            case 2: {
                game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelTwo(), (Module*)game->GetModuleWinScreen(), 4);
                break;
            }
            case 3: {
                game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelThree(), (Module*)game->GetModuleWinScreen(), 4);
                break;
            }
            case 4: {
                game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFour(), (Module*)game->GetModuleWinScreen(), 4);
                break;
            }
            case 5: {
                game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFive(), (Module*)game->GetModuleWinScreen(), 4);
                break;
            }
            case 6: {
                game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelSix(), (Module*)game->GetModuleWinScreen(), 4);
                break;
            }
            default: {
                break;
            }
            }
        }
        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) { timer = 2; }

        physics.UpdatePhysics(position.x, position.y, mruaSpeed.x, mruaSpeed.y);

        // This is where the movement collisions go
        iPoint tile = { position.x / TILE_SIZE, position.y / TILE_SIZE };
        float collisionSpeedM = -mruaSpeed.x * 2;
        float collisionSpeedm = -mruaSpeed.x / 2;
//        LOG("Tile = %d, %d", tile.x, tile.y);
        if (tile.x < 0) { tile.x = 0; }
        if (tile.y < 0) { tile.y = 0; }
            if (destroyed) {
                for (int i = 1; i < 5; i++) { // BOTTOM FLOOR
                    if (!godMode && game->GetModuleTileset()->GetLevelTile(tile.y + 4, tile.x + 2).id == ModuleTileset::TileType::WALL
                        && (game->GetModuleTileset()->GetLevelTile(tile.y + 4, tile.x + 1).id == ModuleTileset::TileType::WALL
                        || game->GetModuleTileset()->GetLevelTile(tile.y + 4, tile.x + 3).id == ModuleTileset::TileType::WALL)
                        && game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::AIR) {
                        if (deathJumps == 1) {
                            godMode = true;
                            mruaSpeed.y = -163.0f; // -145
                        }
                        else {
                            deathJumps++;
                            mruaSpeed.y = -257.0f;
                        }
                        break;
                    }
                }

                for (int i = 0; i < 4; i++) { // LEFT WALL
                    if (!godMode && game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::WALL) {
                        position.x = 2 * TILE_SIZE * tile.x - position.x;
                        mruaSpeed.x = 100.0f;
                        ChangeInvert();
                        godMode = true;
                        break;
                    }
                }

                for (int i = 0; i < 4; i++) { // RIGHT WALL
                    if (!godMode && game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 5).id == ModuleTileset::TileType::WALL) {
                        position.x = 2 * TILE_SIZE * (tile.x + 5) - currentAnimation->GetWidth() * 2 - position.x;
                        mruaSpeed.x = -50.0f;
                        ChangeInvert();
                        godMode = true;
                        break;
                    }
                }
            }
            else { // alive
                // BOTTOM FLOOR
                if (game->GetModuleTileset()->GetLevelTile((position.y + currentAnimation->GetHeight()) / TILE_SIZE, position.x / TILE_SIZE).id == ModuleTileset::TileType::AIR
                    && game->GetModuleTileset()->GetLevelTile((position.y + currentAnimation->GetHeight()) / TILE_SIZE, position.x / TILE_SIZE + 1).id == ModuleTileset::TileType::AIR
                    && game->GetModuleTileset()->GetLevelTile((position.y + currentAnimation->GetHeight()) / TILE_SIZE, position.x / TILE_SIZE + 2).id == ModuleTileset::TileType::AIR
                    && game->GetModuleTileset()->GetLevelTile((position.y + currentAnimation->GetHeight()) / TILE_SIZE, position.x / TILE_SIZE + 3).id == ModuleTileset::TileType::AIR) {
                    mruaSpeed.y = 4.0f;
                    position.y += mruaSpeed.y;
                }
                else if (game->GetModuleTileset()->GetLevelTile((position.y + currentAnimation->GetHeight()) / TILE_SIZE - 1, position.x / TILE_SIZE).id != ModuleTileset::TileType::STAIRS
                    && game->GetModuleTileset()->GetLevelTile((position.y + currentAnimation->GetHeight()) / TILE_SIZE - 1, position.x / TILE_SIZE + 1).id != ModuleTileset::TileType::STAIRS
                    && game->GetModuleTileset()->GetLevelTile((position.y + currentAnimation->GetHeight()) / TILE_SIZE - 1, position.x / TILE_SIZE + 2).id != ModuleTileset::TileType::STAIRS
                    && game->GetModuleTileset()->GetLevelTile((position.y + currentAnimation->GetHeight()) / TILE_SIZE - 1, position.x / TILE_SIZE + 3).id != ModuleTileset::TileType::STAIRS) {
                    mruaSpeed.y = 0;
                    position.y = 25 * TILE_SIZE - currentAnimation->GetHeight();
                }


                for (int i = 1; i < 4; i++) { // LEFT WALL
                    if (speed != 0 &&
                        (game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::WALL ||
                            game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                            game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                            game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                        position.x = 2 * TILE_SIZE * (tile.x + 1) - position.x;
                        break;
                    }
                }

                for (int i = 1; i < 4; i++) { // RIGHT WALL
                    if (speed != 0 &&
                        (game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 3).id == ModuleTileset::TileType::WALL ||
                            game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 3).id == ModuleTileset::TileType::DESTRUCTIBLE1 ||
                            game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 3).id == ModuleTileset::TileType::DESTRUCTIBLE2 ||
                            game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 3).id == ModuleTileset::TileType::NOT_DESTRUCTIBLE)) {
                        position.x = 2 * TILE_SIZE * (tile.x + 3) - idle.GetWidth() * 2 - position.x;
                        break;
                    }
                }
            }


        //Game Overs
        if (isTimeOver) {
			playerLifes--;
            if (once) {
                once = false;
                game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/noMusic.ogg");
                game->GetModuleAudio()->PlayFx(dedSoundIndex);
            }
            if (playerLifes < 0) {
                if (onceMusic) {
                    game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/gameOver.ogg");
                    onceMusic = false;
                }
                if (game->GetModuleAudio()->DetectIfEnd() == false) { 
                    if (game->GetModuleLevelOne()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelOne(), (Module*)game->GetModuleTitleScreen(), 4); }
                    else if (game->GetModuleLevelTwo()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelTwo(), (Module*)game->GetModuleTitleScreen(), 4); }
                    else if (game->GetModuleLevelThree()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelThree(), (Module*)game->GetModuleTitleScreen(), 4); }
                    else if (game->GetModuleLevelFour()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFour(), (Module*)game->GetModuleTitleScreen(), 4); }
                    else if (game->GetModuleLevelFive()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFive(), (Module*)game->GetModuleTitleScreen(), 4); }
                    else if (game->GetModuleLevelSix()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelSix(), (Module*)game->GetModuleTitleScreen(), 4); }
                }
            }
            else if (playerLifes >= 0 && onceTimeIsOver == 125) {
                if (onceDeathSpaguett) {
                    onceDeathSpaguett = false;
                    playerLifes--;
                }
                if (game->GetModuleLevelOne()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelOne(), (Module*)game->GetModuleLevelOne(), 4); }
                else if (game->GetModuleLevelTwo()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelTwo(), (Module*)game->GetModuleLevelTwo(), 4); }
                else if (game->GetModuleLevelThree()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelThree(), (Module*)game->GetModuleLevelThree(), 4); }
                else if (game->GetModuleLevelFour()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFour(), (Module*)game->GetModuleLevelFour(), 4); }
                else if (game->GetModuleLevelFive()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFive(), (Module*)game->GetModuleLevelFive(), 4); }
                else if (game->GetModuleLevelSix()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelSix(), (Module*)game->GetModuleLevelSix(), 4); }
            }
        }

        if (destroyed) {
            keyPressed = true;
            if (once) {
				playerLifes--; //Life system seems to work well with these adjustments
                once = false;
                SDL_Delay(500); //THIS SHIT HAS TO LEAVE AS SOON AS WE CAN. IT IS NO LONGER ALLOWED TO EXIST. THIS IS A NO SDL_DELAY ZONE. EVEN A FUCKING SECOND GIVES ME INSANE ANXIETY. AND ALSO AIDS. NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE. Oh hi Marc, fancy seeing you here :^3 - Luce <3
                game->GetModuleAudio()->PlayFx(dedSoundIndex);
                game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->hitScreen, 0, 0);
                mruaSpeed.x = 100;
                physics.SetAxis(true, true);
            }

            currentAnimation = &ded;
            if (position.y >= SCREEN_HEIGHT + currentAnimation->GetHeight() && playerLifes < 0) {
                if (onceMusic) {
                    game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/gameOver.ogg");
                    onceMusic = false;
                }
                if (game->GetModuleAudio()->DetectIfEnd() == false) {
                    if (game->GetModuleLevelOne()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelOne(), (Module*)game->GetModuleTitleScreen(), 4); }
                    else if (game->GetModuleLevelTwo()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelTwo(), (Module*)game->GetModuleTitleScreen(), 4); }
                    else if (game->GetModuleLevelThree()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelThree(), (Module*)game->GetModuleTitleScreen(), 4); }
                    else if (game->GetModuleLevelFour()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFour(), (Module*)game->GetModuleTitleScreen(), 4); }
                    else if (game->GetModuleLevelFive()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFive(), (Module*)game->GetModuleTitleScreen(), 4); }
                    else if (game->GetModuleLevelSix()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelSix(), (Module*)game->GetModuleTitleScreen(), 4); }
                }
            }
            else if (position.y >= SCREEN_HEIGHT + currentAnimation->GetHeight() && playerLifes >= 0) {
     //           if (onceDeathSpaguett) {
     //               onceDeathSpaguett = false;
					////playerLifes--;  
     //           }
				//What the fuck is onceDeathSpaguett and why does it exist and why is is Spaguett and not Spaghett?????????
                if (game->GetModuleLevelOne()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelOne(), (Module*)game->GetModuleLevelOne(), 4); }
                else if (game->GetModuleLevelTwo()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelTwo(), (Module*)game->GetModuleLevelTwo(), 4); }
                else if (game->GetModuleLevelThree()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelThree(), (Module*)game->GetModuleLevelThree(), 4); }
                else if (game->GetModuleLevelFour()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFour(), (Module*)game->GetModuleLevelFour(), 4); }
                else if (game->GetModuleLevelFive()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFive(), (Module*)game->GetModuleLevelFive(), 4); }
                else if (game->GetModuleLevelSix()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelSix(), (Module*)game->GetModuleLevelSix(), 4); }
            }
        }

        if (keyPressed) { currentAnimation->Update(); }

        //Win condition
        if (game->GetModuleEnemies()->CheckForBalloons()) {
            if (winCountdown == 75) {
                if (game->GetModuleLevelOne()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelOne(), (Module*)game->GetModuleWinScreen(), 4); }
                else if (game->GetModuleLevelTwo()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelTwo(), (Module*)game->GetModuleWinScreen(), 4); }
                else if (game->GetModuleLevelThree()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelThree(), (Module*)game->GetModuleWinScreen(), 4); }
                else if (game->GetModuleLevelFour()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFour(), (Module*)game->GetModuleWinScreen(), 4); }
                else if (game->GetModuleLevelFive()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelFive(), (Module*)game->GetModuleWinScreen(), 4); }
                else if (game->GetModuleLevelSix()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelSix(), (Module*)game->GetModuleWinScreen(), 4); }
            }
            else { ++winCountdown; }
        }
    }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModulePlayer::PostUpdate() {
    if (game->GetModuleInput()->GetCursorState() == 1) {
        sprintf_s(cursorDebugText, 25, "0:ch,1:ntm,2:meh,3:sm");
        game->GetModuleFonts()->BlitText(0, 0, normalFont, cursorDebugText);
    }
    sprintf_s(scoreText, 10, "%5d", score);
    game->GetModuleFonts()->BlitText(TILE_SIZE * 8, game->GetModuleLevelOne()->GetBackgroundAdapter().h + TILE_SIZE, normalFont, scoreText);
    sprintf_s(playerText, 10, "player-1");
    game->GetModuleFonts()->BlitText(TILE_SIZE * 2, game->GetModuleLevelOne()->GetBackgroundAdapter().h, normalFont, playerText);

    for (int i = playerLifes; i > 0; i--) {
        SDL_Rect lifeAdapter = { i * (TILE_SIZE * 2),game->GetModuleLevelOne()->GetBackgroundAdapter().h + (TILE_SIZE * 2),TILE_SIZE * 2,TILE_SIZE * 2 };
        game->GetModuleRender()->Blit(texture, 0, 0, false, &life, &lifeAdapter);
    }

    switch (shotType) {
    case SHOT_TYPES::DWIRE: {
        game->GetModuleRender()->Blit(powerUpIcons, TILE_SIZE * 11, game->GetModuleLevelOne()->GetBackgroundAdapter().h + (TILE_SIZE * 2), false, &iconP);
        break;
    }
    case SHOT_TYPES::POWER: {
        game->GetModuleRender()->Blit(powerUpIcons, TILE_SIZE * 11, game->GetModuleLevelOne()->GetBackgroundAdapter().h + (TILE_SIZE * 2), false, &iconD);
        break;
    }
    case SHOT_TYPES::VULCAN: {
        game->GetModuleRender()->Blit(powerUpIcons, TILE_SIZE * 11, game->GetModuleLevelOne()->GetBackgroundAdapter().h + (TILE_SIZE * 2), false, &iconV);
        break;
    }
    }

    game->GetModuleRender()->Blit(texture, position.x, position.y, GetInvertValue(), &currentAnimation->GetCurrentFrame());

    if (isTimeOver) {
        SDL_Rect timeOverAdapter = { (game->GetModuleLevelOne()->GetBackgroundAdapter().w / 2) - 75,game->GetModuleLevelOne()->GetBackgroundAdapter().h / 2 - (TILE_SIZE),150,17 };
        game->GetModuleRender()->Blit(blueText, 0, 0, false, &timeOver, &timeOverAdapter);
        onceTimeIsOver++;
    }
    else if (playerLifes < 0 && position.y >= SCREEN_HEIGHT + currentAnimation->GetHeight()) {
        SDL_Rect gameOverAdapter = { (game->GetModuleLevelOne()->GetBackgroundAdapter().w / 2) - 75,(game->GetModuleLevelOne()->GetBackgroundAdapter().h / 2) - (TILE_SIZE),150,17 };
        game->GetModuleRender()->Blit(blueText, 0, 0, false, &gameOver, &gameOverAdapter);
    }

    //Timer
    sprintf_s(timerText, 10, "time:%03d", timer);
    game->GetModuleFonts()->BlitText(game->GetModuleLevelOne()->GetBackgroundAdapter().w - (TILE_SIZE * 16) - 1, TILE_SIZE + 1, timerFont, timerText);

     if (!destroyed && !isTimeOver) {
         if (time <= 1) { time += deltaTime; }
         else {
             time = deltaTime;
             --timer;
         }

         if (timer <= 0) { isTimeOver = true; }

         if (timer == 20 && onceHurry2 == true) {
             onceHurry1 = false;
             onceHurry2 = false;
             game->GetModuleAudio()->PlayMusic("Resources/BGM/hurryUpLvl2.ogg");
         }

         if (timer <= 50 && onceHurry1 == true) {
             if (onceHurry0) {
                 onceHurry0 = false;
                 game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/introHurryUpLvl1.ogg");
             }
             game->GetModuleAudio()->ChangeAtEnd("Resources/BGM/hurryUpLvl1.ogg");
         }
     }

    return UPDATE_STATUS::UPDATE_CONTINUE; 
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2) {
    if (!godMode && c1 == collider && !destroyed && !isTimeOver && c2->GetType() == Collider::TYPE::BALLOON) {
        destroyed = true;
        if (position.x < c2->GetRect().x) {
            mruaSpeed.x = -100 / 2;
            ChangeInvert();
        }
    }
}

int ModulePlayer::GetSpeed() const { return speed; }
void ModulePlayer::SetSpeed(int _speed) { speed = _speed; }
iPoint ModulePlayer::GetPosition() const { return position; }
void ModulePlayer::SetPosition(iPoint _position) { position = _position; }
SDL_Texture* ModulePlayer::GetTexture() const { return texture; }
SDL_Texture* ModulePlayer::GetBlueTextTexture() const { return blueText; }
SDL_Texture* ModulePlayer::GetPowerIconTexture() const { return powerUpIcons; }
Animation* ModulePlayer::GetCurrentAnimation() const { return currentAnimation; }
SHOT_TYPES ModulePlayer::GetCurrentShotType() const { return shotType; }
uint ModulePlayer::GetNormalShotSoundIndex() const { return normalShotSoundIndex; }
void ModulePlayer::SetNormalShotSoundIndex(uint _normalShotSoundIndex) { normalShotSoundIndex = _normalShotSoundIndex; }
uint ModulePlayer::GetVulcanShotSoundIndex() const { return vulcanShotSoundIndex; }
void ModulePlayer::SetVulcanShotSoundIndex(uint _vulcanShotSoundIndex) { vulcanShotSoundIndex = _vulcanShotSoundIndex; }
uint ModulePlayer::GetDedSoundIndex() const { return dedSoundIndex; }
void ModulePlayer::SetDedSoundIndex(uint _dedSoundIndex) { dedSoundIndex = _dedSoundIndex; }
int ModulePlayer::GetFontIndex() const { return normalFont; }
int ModulePlayer::GetTimerFontIndex() const { return timerFont; }
void ModulePlayer::IncreaseShoot() { 
    if (shot < 0) { shot = 0; }
    else if (shot < maxShots) { shot++; }
    else { shot--; }
}
void ModulePlayer::DecreaseShoot() { shot -= 1; }
void ModulePlayer::SetShotValue(int _shot) { shot = _shot; };
int ModulePlayer::GetShotsValue() { return shot; }
bool ModulePlayer::CheckIfGodMode() const { return godMode; };
bool ModulePlayer::CheckIfDestroyed() const { return (destroyed || isTimeOver); };
void ModulePlayer::AddScore(int _score) { score += _score; }
int ModulePlayer::GetTimer() const { return timer; }
void ModulePlayer::SetPlayerLives(int lives) { playerLifes = lives; }
int ModulePlayer::GetPlayerLives() const { return playerLifes; }
void ModulePlayer::SetScore(int _score) { score = _score; };
int ModulePlayer::GetBallTypeDebug() const { return ballTypeDebug; };