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
#include "ModuleLevelOne.h"
#include "ModuleFonts.h"

#include "Particle.h"

#include "SDL/include/SDL_timer.h"
#include "SDL/include/SDL_scancode.h"
#include <stdio.h>

ModulePlayer::ModulePlayer(bool startEnabled) : Module(startEnabled) {
    //Animation setter
    idle.PushBack({ 12,112,25,32 });
    idle.SetSpeed(0.0f);

    shoot.PushBack({ 44,112,27,32 });
    shoot.SetSpeed(0.0f);

    moving.PushBack({ 12,2,29,32 });
    moving.PushBack({ 46,2,30,32 });
    moving.PushBack({ 80,2,29,32 });
    moving.PushBack({ 114,2,27,32 });
    moving.PushBack({ 148,2,29,32 });
    moving.SetSpeed(0.2f);

    ded.PushBack({ 81,112,40,29 });
    ded.SetSpeed(0.0f);

    gameOver = { 5,5,532,58 };
    timeOver = { 5,69,532,58 };
    ready = { 5,132,203,67 };
    life = { 154,44,15,15 };

    mruaSpeed.x = 100;
    mruaSpeed.y = -197;
}
ModulePlayer::~ModulePlayer() {}

bool ModulePlayer::Start() {
    LOG("Loading player textures");
    destroyed = false;
    once = true;
    onceMusic = true;
    onceHurry1 = true;
    onceHurry2 = true;
    godMode = false;

    blueText = game->GetModuleTextures()->Load("Resources/Sprites/blueText.png");
    texture = game->GetModuleTextures()->Load("Resources/Sprites/player.png"); // arcade version
    shotSoundIndex = game->GetModuleAudio()->LoadFx("Resources/SFX/shotClaw.wav");
    dedSoundIndex = game->GetModuleAudio()->LoadFx("Resources/SFX/dead.wav");

    position.x = 177;
    position.y = 186 - idle.GetHeight();
    physics.SetAxis(false, false);

    collider = game->GetModuleCollisions()->AddCollider({ position.x, position.y, idle.GetWidth(), idle.GetHeight() }, Collider::TYPE::PLAYER, this); // adds a collider to the player

    normalFont = game->GetModuleFonts()->Load("Resources/Sprites/Font.png", "abcdefghijklmnopqrstuvwxyz0123456789.,!<-:�>        ", 4);
    timerFont = game->GetModuleFonts()->Load("Resources/Sprites/Timer.png", "time:0123456789 ", 2);
    timer[0] = 1;
    timer[1] = 0;
    timer[2] = 0;
    time = -31.0f / 60.0f;

    return true;
}

UPDATE_STATUS ModulePlayer::Update()
{
    if (returnToIdle == 0) { currentAnimation = &idle; }
    else { --returnToIdle; }

    if (game->GetModuleLevelOne()->CheckIfStarted()) {
        //Reset the currentAnimation back to idle before updating the logic
        if (!destroyed) {
            if (game->GetModuleInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && game->GetModuleInput()->GetKey(SDL_SCANCODE_A) != KEY_REPEAT && (returnToIdle == 0)) {
                currentAnimation = &moving;
                if (GetInvertValue()) { ChangeInvert(); }
                position.x += speed;
            }
            if (game->GetModuleInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && game->GetModuleInput()->GetKey(SDL_SCANCODE_D) != KEY_REPEAT && (returnToIdle == 0)) {
                currentAnimation = &moving;
                if (!(GetInvertValue())) { ChangeInvert(); }
                position.x -= speed;
            }
            // we need to activate it only when the player is on the stairs (OnCollision)
            //if (game->GetModuleInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && (returnToIdle == 0)) {
            //    currentAnimation = &idle;
            //    if (!(GetInvertValue())) { ChangeInvert(); }
            //    position.y -= speed;
            //}
            //if (game->GetModuleInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && (returnToIdle == 0)) {
            //    currentAnimation = &idle;
            //    if (!(GetInvertValue())) { ChangeInvert(); }
            //    position.y += speed;
            //}
            if (game->GetModuleInput()->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && shot == false) {
                shot = true;
                if (!godMode) { game->GetModuleAudio()->PlayFx(shotSoundIndex); }
                currentAnimation = &shoot;

                if (GetInvertValue()) {
                    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->normalWire, position.x + (shoot.GetWidth() / 3), position.y - 1, Collider::TYPE::PLAYER_SHOT);
                    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->muzzleFlash, position.x + 3, position.y - 10, Collider::TYPE::PLAYER_SHOT);
                }
                if (!GetInvertValue()) {
                    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->normalWire, position.x + (shoot.GetWidth() / 2), position.y - 1, Collider::TYPE::PLAYER_SHOT);
                    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->muzzleFlash, position.x + currentAnimation->GetWidth() / 2 - 6, position.y - 10, Collider::TYPE::PLAYER_SHOT); //It works, it just works ~Todd Howard,from Skyrim
                }

                returnToIdle = 20;
            }
            if (currentAnimation == &idle) { collider->SetPos(position.x, position.y, idle.GetWidth(), idle.GetHeight()); }
            else if (currentAnimation == &moving) { collider->SetPos(position.x, position.y, moving.GetWidth(), moving.GetHeight()); }
            else if (currentAnimation == &shoot) { collider->SetPos(position.x, position.y, shoot.GetWidth(), shoot.GetHeight()); }
        }

        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) { godMode = !godMode; }
        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) { destroyed = !destroyed; }
        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
            //!puppy
            game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelOne(), (Module*)game->GetModuleProjectSheet(), 90);
        }
        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelOne(), (Module*)game->GetModuleWinScreen(), 4); }
        if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
            timer[1] = 2;
            timer[2] = 5;
        }

        if (destroyed) {
            if (once) {
                once = false;
                if (timer[0] != 0 && timer[1] != 0 && timer[2] != 0) { SDL_Delay(1000); }
                game->GetModuleAudio()->PlayFx(dedSoundIndex);
                if (timer[0] != 0 && timer[1] != 0 && timer[2] != 0) {
                    game->GetModuleCollisions()->AddCollider({ position.x, position.y, ded.GetWidth(), ded.GetHeight() }, Collider::TYPE::PLAYER, this);
                    game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->hitScreen, 0, 0);
                    physics.SetAxis(true, true);
                }
            }
            if (timer[0] != 0 && timer[1] != 0 && timer[2] != 0) { currentAnimation = &ded; }
            --playerLifes;
            if (position.y >= SCREEN_HEIGHT + currentAnimation->GetHeight() && playerLifes < 0) {
                if (onceMusic) {

                    game->GetModuleAudio()->PlayMusicOnce("Resources/BGM/gameOver.ogg");
                    onceMusic = false;
                }
        
                if (game->GetModuleAudio()->DetectIfEnd() == false) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelOne(), (Module*)game->GetModuleTitleScreen(), 4); }
            }
            else if (position.y >= SCREEN_HEIGHT + currentAnimation->GetHeight() && playerLifes >= 0) {
                game->GetModuleTransition()->Transition(this, this, 4);
            }
            else if (timer[0] != 0 && timer[1] != 0 && timer[2] != 0) { physics.UpdatePhysics(position.x, position.y, mruaSpeed.x, mruaSpeed.y); }

            if (timer[0] != 0 && timer[1] != 0 && timer[2] != 0) { collider->SetPos(position.x, position.y, ded.GetWidth(), ded.GetHeight()); }
        }

        currentAnimation->Update();

        if (game->GetModuleEnemies()->CheckForBalloons()) {
            if (winCountdown == 100) { game->GetModuleTransition()->Transition((Module*)game->GetModuleLevelOne(), (Module*)game->GetModuleWinScreen(), 4); }
            else { ++winCountdown; }
        }
        
    }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModulePlayer::PostUpdate() {
    sprintf_s(scoreText, 10, "%6d", score);
    game->GetModuleFonts()->BlitText(100, 203, normalFont, scoreText);
    sprintf_s(playerText, 10, "player1");
    game->GetModuleFonts()->BlitText(20, 193, normalFont, playerText);

    for (int i = playerLifes; i > 0; i--) {
        SDL_Rect lifeAdapter = { i * 45,SCREEN_HEIGHT*3-45,15,15 };
        game->GetModuleRender()->Blit(texture, 0, 0, false, &life, &lifeAdapter);
    }

    game->GetModuleRender()->Blit(texture, position.x, position.y, GetInvertValue(), &currentAnimation->GetCurrentFrame());
    if (playerLifes < 0) {
        SDL_Rect backgroundAdapter = { 0, 0, 384, 193 };
        if (timer[0] == 0 && timer[1] == 0 && timer[2] == 0) {
            game->GetModuleAudio()->PlayMusic("Resources/BGM/noMusic.ogg");
            SDL_Rect timeOverAdapter = { (SCREEN_WIDTH / 2 + 150),backgroundAdapter.h + 75,150,17 };
            game->GetModuleRender()->Blit(blueText, 0, 0, false, &timeOver, &timeOverAdapter);
        }
        else if (position.y >= SCREEN_HEIGHT + currentAnimation->GetHeight()) {
            SDL_Rect gameOverAdapter = { (SCREEN_WIDTH / 2 + 150),backgroundAdapter.h + 75,150,17 };
            game->GetModuleRender()->Blit(blueText, 0, 0, false, &gameOver, &gameOverAdapter);
        }
    }

    //Timer
    sprintf_s(timerText, 10, "time:%d%d%d", timer[0], timer[1], timer[2]);
    game->GetModuleFonts()->BlitText(SCREEN_WIDTH - 130, 10, timerFont, timerText);
    
     if (!destroyed) {
         if (time <= 1) { time += deltaTime; }
         else {
             time = deltaTime;
             timer[2]--;
             if (timer[2] < 0) {
                 timer[2] = 9;
                 timer[1]--;
                 if (timer[1] < 0) {
                     timer[1] = 9;
                     timer[0]--;
                 }
             }
         }
         if (timer[0] == 0 && timer[1] == 5 && timer[2] == 0 && onceHurry1 == true) {
             game->GetModuleAudio()->PlayMusic("Resources/BGM/hurryUpLvl1.ogg");
             onceHurry1 = false;
         }
         if (timer[0] == 0 && timer[1] == 2 && timer[2] == 0 && onceHurry2 == true) {
             game->GetModuleAudio()->PlayMusic("Resources/BGM/hurryUpLvl2.ogg");
             onceHurry2 = false;
         }
         if (timer[0] == 0 && timer[1] == 0 && timer[2] == 0) { destroyed = true; }
     }
    
    

    return UPDATE_STATUS::UPDATE_CONTINUE; 
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2) {
    short int counter = 0;
    if (!godMode) {
        if (c1 == collider && destroyed == false && c2->GetType() == Collider::TYPE::WALL) {
            if (position.x < (c2->GetRect().x + c2->GetRect().w) && position.x > c2->GetRect().x) { 
                position.x = (c2->GetRect().x + c2->GetRect().w);
                if (destroyed) { ChangeInvert(); }
            }
            else if ((position.x + currentAnimation->GetWidth()) > c2->GetRect().x&& position.x < c2->GetRect().x) { 
                position.x = (c2->GetRect().x - currentAnimation->GetWidth()); 
                if (destroyed) { ChangeInvert(); }
            }
            //if (position.y < (c2->GetRect().y + c2->GetRect().h) && position.y > c2->GetRect().y) {
            //    position.y = (c2->GetRect().y + c2->GetRect().h);
            //}
            //else if ((position.y + currentAnimation->GetHeight()) > c2->GetRect().y&& position.y < c2->GetRect().y) {
            //    position.y = (c2->GetRect().y - currentAnimation->GetHeight());
            //}
        }
        else if (c1 == collider && destroyed == false && c2->GetType() == Collider::TYPE::STAIRS) {
            //if ((c2->GetRect().y + c2->GetRect().h) >= (position.y + currentAnimation->GetHeight())) { // (bottom of stairs) should be able to move both x and y

            //}
            //else if (c2->GetRect().y <= position.y) { // (top of stairs) should be able to move both x and y

            //}
            //else { // (middle of stairs) should only be able to move y

            //}
        }
        else if (c1 == collider && destroyed == false && c2->GetType() == Collider::TYPE::BALLOON) { destroyed = true; }
        else if (c1 == collider && destroyed == false && c2->GetType() == Collider::TYPE::ANIMAL) { /*destroyed = true;*/ }
        else if (destroyed) {
            if ((position.x + currentAnimation->GetWidth()) > c2->GetRect().x&& position.x < c2->GetRect().x) {
                mruaSpeed.x = -50;
            }
            if (c1 == collider && c2->GetType() == Collider::TYPE::FLOOR) {
               mruaSpeed.y = -145;
               godMode = true;
            }
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
void ModulePlayer::SetIfShot(bool _shot) { shot = _shot; }
bool ModulePlayer::CheckIfGodMode() const { return godMode; };
bool ModulePlayer::CheckIfDestroyed() const { return destroyed; };
void ModulePlayer::AddScore(int _score) { score += _score; }