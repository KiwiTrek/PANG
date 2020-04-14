#include "Game.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"
#include "ModuleParticles.h"
#include "Particle.h"
#include "ModuleCollisions.h"

#include "SDL/include/SDL_scancode.h"

ModulePlayer::ModulePlayer() {
    //Animation setter
    idle.PushBack({ 12,112,25,31 });
    idle.SetSpeed(0.0f);

    shoot.PushBack({ 44,115,27,28 });
    shoot.SetSpeed(0.0f);

    moving.PushBack({ 12,2,29,31 });
    moving.PushBack({ 46,3,30,30 });
    moving.PushBack({ 80,2,29,31 });
    moving.PushBack({ 114,3,27,30 });
    moving.PushBack({ 148,3,29,30 });
    moving.SetSpeed(0.2f);
}
ModulePlayer::~ModulePlayer() {}

int ModulePlayer::GetSpeed() const { return speed; }
void ModulePlayer::SetSpeed(int _speed) { speed = _speed; }
iPoint ModulePlayer::GetPosition() const { return position; }
void ModulePlayer::SetPosition(iPoint _position) { position = _position; }
SDL_Texture* ModulePlayer::GetTexture() const { return texture; }
Animation* ModulePlayer::GetCurrentAnimation() const { return currentAnimation; }
uint ModulePlayer::GetShotSoundIndex() const { return shotSoundIndex; }
void ModulePlayer::SetShotSoundIndex(uint _shotSoundIndex) { shotSoundIndex = _shotSoundIndex; }
void ModulePlayer::SetIfShot(bool _shot) { shot = _shot; }
bool ModulePlayer::CheckIfGodMode() const { return godMode; };


bool ModulePlayer::Start()
{
    LOG("Loading player textures");

    texture = game->GetModuleTextures()->Load("Resources/Sprites/Player.png"); // arcade version
    shotSoundIndex = game->GetModuleAudio()->LoadFx("Resources/SFX/shotClaw.wav");

    position.x = 177;
    position.y = 186 - idle.GetHeight();

    collider = game->GetModuleCollisions()->AddCollider({ position.x, position.y, idle.GetWidth(), idle.GetHeight() }, Collider::TYPE::PLAYER, this); // adds a collider to the player

    return true;
}

UPDATE_STATUS ModulePlayer::Update()
{
    //Reset the currentAnimation back to idle before updating the logic
    if (returnToIdle == 0) { currentAnimation = &idle; }
    else { --returnToIdle; }

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
 //       game->GetModuleAudio()->PlayFx(shotSoundIndex);
        currentAnimation = &shoot;
        
        if (GetInvertValue()) {
			game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->normalWire, position.x + (shoot.GetWidth() / 3), position.y - 1 , Collider::TYPE::PLAYER_SHOT);
        }
        if (!GetInvertValue()) {
			game->GetModuleParticles()->AddParticle(game->GetModuleParticles()->normalWire, position.x + (shoot.GetWidth() / 2), position.y - 1 , Collider::TYPE::PLAYER_SHOT);
        }

        returnToIdle = 5;
    }

    if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
        if (godMode == false) { godMode = true; }
        else { godMode = false; }
    }


    if (currentAnimation == &idle) {
        collider->SetPos(position.x, position.y, idle.GetWidth(),  idle.GetHeight());
    }
    else if (currentAnimation == &moving) {
        collider->SetPos(position.x, position.y, moving.GetWidth(), moving.GetHeight());
    }
    else if (currentAnimation == &shoot) {
        collider->SetPos(position.x, position.y, shoot.GetWidth(), shoot.GetHeight());
    }

    currentAnimation->Update();

    if (destroyed) { return UPDATE_STATUS::UPDATE_STOP; }

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModulePlayer::PostUpdate() {
    if (!destroyed) {
        SDL_Rect rect = currentAnimation->GetCurrentFrame();
        game->GetModuleRender()->Blit(texture, position.x, position.y, GetInvertValue(), &rect);
    }

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(Collider* c1, Collider* c2) {
    if (godMode == false) {
        if (c1 == collider && destroyed == false && c2->GetType() == Collider::TYPE::WALL) {
            if (position.x < (c2->GetRect().x + c2->GetRect().w) && position.x > c2->GetRect().x) {
                position.x = (c2->GetRect().x + c2->GetRect().w);
            }
            else if ((position.x + currentAnimation->GetWidth()) > c2->GetRect().x&& position.x < c2->GetRect().x) {
                position.x = (c2->GetRect().x - currentAnimation->GetWidth());
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
        else if (c1 == collider && destroyed == false && c2->GetType() == Collider::TYPE::BALLOON) {
            destroyed = true;
        }
        else if (c1 == collider && destroyed == false && c2->GetType() == Collider::TYPE::ANIMAL) {
            destroyed = true;
        }
    }
}