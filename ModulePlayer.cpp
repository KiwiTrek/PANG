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
    shot = false;
    winCountdown = 0;

    blueText = game->GetModuleTextures()->Load("Resources/Sprites/blueText.png");
    texture = game->GetModuleTextures()->Load("Resources/Sprites/player.png"); // arcade version
    shotSoundIndex = game->GetModuleAudio()->LoadFx("Resources/SFX/shotClaw.wav");
    dedSoundIndex = game->GetModuleAudio()->LoadFx("Resources/SFX/dead.wav");

    moving.Reset();
    ded.Reset();

    position.x = (TILE_SIZE * 20) + 2;
    position.y = game->GetModuleLevelOne()->GetBackgroundAdapter().h - TILE_SIZE - idle.GetHeight();
    mruaSpeed.x = 100;
    mruaSpeed.y = -197;
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

    if (returnToIdle == 0) { currentAnimation = &idle; }
    else { --returnToIdle; }

    if (game->GetModuleLevelOne()->CheckIfStarted() ||
        game->GetModuleLevelTwo()->CheckIfStarted() ||
        game->GetModuleLevelThree()->CheckIfStarted()) {
        //Reset the currentAnimation back to idle before updating the logic
        if (!destroyed && !isTimeOver) {
            if (((game->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && game->GetModuleInput()->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
                || (pad.right && !pad.left)
                || pad.l_x > 0)
                && (returnToIdle == 0)) {
                currentAnimation = &moving;
                if (GetInvertValue()) { ChangeInvert(); }
                position.x += speed;
            }
            if (((game->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && game->GetModuleInput()->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
                || (pad.left && !pad.right)
                || pad.l_x < 0)
                && (returnToIdle == 0)) {
                currentAnimation = &moving;
                if (!(GetInvertValue())) { ChangeInvert(); }
                position.x -= speed;
            }

            // we need to activate it only when the player is on the stairs (OnCollision)
            /*if (game->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT 
                && game->GetModuleInput()->GetKey(SDL_SCANCODE_S) != KEY_REPEAT
                && game->GetModuleInput()->GetGamePad().up
                && game->GetModuleInput()->GetGamePad().down
                && game->GetModuleInput()->GetGamePad().l_y < 0
                (returnToIdle == 0)) {
                currentAnimation = &idle;
                if (!(GetInvertValue())) { ChangeInvert(); }
                position.y -= speed;
            }
            if (game->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT
                && game->GetModuleInput()->GetKey(SDL_SCANCODE_W) != KEY_REPEAT
                && game->GetModuleInput()->GetGamePad().down
                && game->GetModuleInput()->GetGamePad().up
                && game->GetModuleInput()->GetGamePad().l_y > 0
                && (returnToIdle == 0)) {
                currentAnimation = &idle;
                if (!(GetInvertValue())) { ChangeInvert(); }
                position.y += speed;
            }*/

            if ((game->GetModuleInput()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || pad.x || pad.b || pad.r2 > 0)
                && shot == false) {
                shot = true;
                if (!godMode) { game->GetModuleAudio()->PlayFx(shotSoundIndex); }
                currentAnimation = &shoot;
                if (GetInvertValue()) {
                    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->normalWire, position.x + (shoot.GetWidth() / 3) - 2, position.y - 1, Collider::TYPE::PLAYER_SHOT);
                    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->muzzleFlash, position.x + 3, position.y - 10, Collider::TYPE::NONE);
                }
                else {
                    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->normalWire, position.x + (shoot.GetWidth() / 2) - 2, position.y - 1, Collider::TYPE::PLAYER_SHOT);
                    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->muzzleFlash, position.x + currentAnimation->GetWidth() / 2 - 6, position.y - 10, Collider::TYPE::NONE); //It works, it just works ~Todd Howard,from Skyrim
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
            default: {
                break;
            }
            }
        }
        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) { timer = 2; }

        // This is where the physics go
        physics.UpdatePhysics(position.x, position.y, mruaSpeed.x, mruaSpeed.y);

        // This is where the movement collisions go
        iPoint tile = { position.x / TILE_SIZE, position.y / TILE_SIZE };
        float collisionSpeedM = -mruaSpeed.x * 2;
        float collisionSpeedm = -mruaSpeed.x / 2;
        LOG("Tile = %d, %d", tile.x, tile.y);
        if (tile.x < 0) { tile.x = 0; }
        if (tile.y < 0) { tile.y = 0; }
        if (tile.y > 26) { tile.y = 26; }
            if (destroyed) {
                for (int i = 1; i < 5; i++) { // BOTTOM FLOOR
                    if (!godMode && game->GetModuleTileset()->GetLevelTile(tile.y + 4, tile.x + 2).id == ModuleTileset::TileType::WALL && (game->GetModuleTileset()->GetLevelTile(tile.y + 4, tile.x + 1).id == ModuleTileset::TileType::WALL || game->GetModuleTileset()->GetLevelTile(tile.y + 4, tile.x + 3).id == ModuleTileset::TileType::WALL) && game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::AIR) {
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
                        position.x = 2 * TILE_SIZE - position.x;
                        mruaSpeed.x = 100.0f;
                        ChangeInvert();
                        godMode = true;
                        break;
                    }
                }

                for (int i = 0; i < 4; i++) { // RIGHT WALL
                    if (!godMode && game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 5).id == ModuleTileset::TileType::WALL) {
                        position.x = 2 * TILE_SIZE * 47 - currentAnimation->GetWidth() * 2 - position.x;
                        mruaSpeed.x = -50.0f;
                        ChangeInvert();
                        godMode = true;
                        break;
                    }
                }
            }
            else { // alive
                for (int i = 1; i < 3; i++) { // BOTTOM FLOOR
                    //if (onceDeath && game->GetModuleTileset()->GetLevelTile(tile.y + 4, tile.x).id == ModuleTileset::TileType::WALL && (game->GetModuleTileset()->GetLevelTile(tile.y + 4, tile.x + 1).id == ModuleTileset::TileType::WALL || game->GetModuleTileset()->GetLevelTile(tile.y + 4, tile.x - 1).id == ModuleTileset::TileType::WALL) && game->GetModuleTileset()->GetLevelTile(tile.y, tile.x).id == ModuleTileset::TileType::AIR) {
                    if (game->GetModuleTileset()->GetLevelTile(tile.y + 4, tile.x + i).id == ModuleTileset::TileType::WALL) {
                        position.y = 2 * TILE_SIZE * 25 - idle.GetHeight() * 2 - position.y;
                        break;
                    }
                }

                for (int i = 0; i < 4; i++) { // LEFT WALL
                    //if (game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::WALL && game->GetModuleTileset()->GetLevelTile(tile.y + 4, tile.x + 1).id == ModuleTileset::TileType::AIR) {
                    if (game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x).id == ModuleTileset::TileType::WALL) {
                        position.x = 2 * TILE_SIZE - position.x;
                        break;
                    }
                }

                for (int i = 0; i < 4; i++) { // RIGHT WALL
                    //if (game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 4).id == ModuleTileset::TileType::WALL && game->GetModuleTileset()->GetLevelTile(tile.y + 4, tile.x).id == ModuleTileset::TileType::AIR) {
                    if (game->GetModuleTileset()->GetLevelTile(tile.y + i, tile.x + 3).id == ModuleTileset::TileType::WALL) {
                        position.x = 2 * TILE_SIZE * 47 - idle.GetWidth() * 2 - position.x;
                        break;
                    }
                }
            }

            //LEFT WALL
            //if (physics.CheckMovementCollision({ position.x, position.y,currentAnimation->GetWidth(), currentAnimation->GetHeight() }, { 0,0, TILE_SIZE, TILE_SIZE * 26 })) {
            //    position.x = 2 * TILE_SIZE - position.x;
            //    if (destroyed) {
            //        mruaSpeed.x = -mruaSpeed.x * 2;
            //        ChangeInvert();
            //    }
            //}

            //TOP FLOOR

            //RIGHT WALL
            //if (physics.CheckMovementCollision({ position.x, position.y,currentAnimation->GetWidth(), currentAnimation->GetHeight() }, { TILE_SIZE * 47,0, TILE_SIZE, TILE_SIZE * 26 })) {
            //    position.x = 2 * TILE_SIZE * 47 - currentAnimation->GetWidth() * 2 - position.x;
            //    if (destroyed) {
            //        mruaSpeed.x = -mruaSpeed.x / 2;
            //        ChangeInvert();
            //    }
            //}

            //BOTTOM FLOOR
            //if (physics.CheckMovementCollision({ position.x, position.y,currentAnimation->GetWidth(), currentAnimation->GetHeight() }, { 0,TILE_SIZE * 25, TILE_SIZE * 48, TILE_SIZE })) {
            //    if (destroyed) {
            //        if (onceDeath) {
            //            mruaSpeed.y = -145;
            //            onceDeath = false;
            //        }
            //    }
            //    else { position.y = 2 * TILE_SIZE * 25 - currentAnimation->GetHeight() * 2 - position.y; }
            //}

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
            }
        }

        if (destroyed) {
            if (once) {
				playerLifes--; //Life system seems to work well with these adjustments
                once = false;
                SDL_Delay(500); //THIS SHIT HAS TO LEAVE AS SOON AS WE CAN. IT IS NO LONGER ALLOWED TO EXIST. THIS IS A NO SDL_DELAY ZONE. EVEN A FUCKING SECOND GIVES ME INSANE ANXIETY. AND ALSO AIDS. NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE NIGHTMARE. Oh hi Marc, fancy seeing you here :^3 - Luce <3
                game->GetModuleAudio()->PlayFx(dedSoundIndex);
                game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->hitScreen, 0, 0);
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
            }
        }

        currentAnimation->Update();

        if (game->GetModuleEnemies()->CheckForBalloons()) {
            if (winCountdown == 75) {
                if (game->GetModuleLevelOne()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelOne(), (Module*)game->GetModuleWinScreen(), 4); }
                else if (game->GetModuleLevelTwo()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelTwo(), (Module*)game->GetModuleWinScreen(), 4); }
                else if (game->GetModuleLevelThree()->IsEnabled()) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelThree(), (Module*)game->GetModuleWinScreen(), 4); }
            }
            else { ++winCountdown; }
        }
    }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModulePlayer::PostUpdate() {
    sprintf_s(scoreText, 10, "%5d", score);
    game->GetModuleFonts()->BlitText(TILE_SIZE * 8, game->GetModuleLevelOne()->GetBackgroundAdapter().h + TILE_SIZE, normalFont, scoreText);
    sprintf_s(playerText, 10, "player-1");
    game->GetModuleFonts()->BlitText(TILE_SIZE * 2, game->GetModuleLevelOne()->GetBackgroundAdapter().h, normalFont, playerText);

    for (int i = playerLifes; i > 0; i--) {
        SDL_Rect lifeAdapter = { i * (TILE_SIZE * 2),game->GetModuleLevelOne()->GetBackgroundAdapter().h + (TILE_SIZE * 2),TILE_SIZE * 2,TILE_SIZE * 2 };
        game->GetModuleRender()->Blit(texture, 0, 0, false, &life, &lifeAdapter);
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
Animation* ModulePlayer::GetCurrentAnimation() const { return currentAnimation; }
uint ModulePlayer::GetShotSoundIndex() const { return shotSoundIndex; }
void ModulePlayer::SetShotSoundIndex(uint _shotSoundIndex) { shotSoundIndex = _shotSoundIndex; }
uint ModulePlayer::GetDedSoundIndex() const { return shotSoundIndex; }
void ModulePlayer::SetDedSoundIndex(uint _dedSoundIndex) { dedSoundIndex = _dedSoundIndex; }
int ModulePlayer::GetFontIndex() const { return normalFont; }
int ModulePlayer::GetTimerFontIndex() const { return timerFont; }
void ModulePlayer::SetIfShot(bool _shot) { shot = _shot; }
bool ModulePlayer::CheckIfGodMode() const { return godMode; };
bool ModulePlayer::CheckIfDestroyed() const { return (destroyed || isTimeOver); };
void ModulePlayer::AddScore(int _score) { score += _score; }
int ModulePlayer::GetTimer() const { return timer; }
void ModulePlayer::SetPlayerLives(int lives) { playerLifes = lives; }
int ModulePlayer::GetPlayerLives() const { return playerLifes; }
void ModulePlayer::SetScore(int _score) { score = _score; };