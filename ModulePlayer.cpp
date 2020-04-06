#include "Game.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePlayer.h"

#include "SDL/include/SDL_scancode.h"

ModulePlayer::ModulePlayer() {
    position.x = 177;
    position.y = 186;

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
bool ModulePlayer::GetInvertValue() const { return playerInvert; };
void ModulePlayer::ChangeInvert() {
    if (playerInvert == true) { playerInvert = false; }
    else if (playerInvert == false) { playerInvert = true; }
};

bool ModulePlayer::Start()
{
    LOG("Loading player textures");

    texture = game->textures->Load("Resources/Sprites/player.png"); // arcade version

    return true;
}

UPDATE_STATUS ModulePlayer::Update()
{
    //Reset the currentAnimation back to idle before updating the logic
    if (returnToIdle == 0) { currentAnimation = &idle; }
    else { --returnToIdle; }

    if (game->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
        currentAnimation = &moving;
        if (GetInvertValue() == true) { ChangeInvert(); }
        position.x += speed;
    }
    if (game->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
        currentAnimation = &moving;
        if (GetInvertValue() == false) { ChangeInvert(); }
        position.x -= speed;
    }
    if (game->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
        currentAnimation = &shoot; //Needs fix & Add sounds
        returnToIdle = 5;
    }

    currentAnimation->Update();

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModulePlayer::PostUpdate() {
    SDL_Rect rect = currentAnimation->GetCurrentFrame();
    game->render->Blit(texture, position.x, position.y - rect.h, GetInvertValue(), &rect);

    return UPDATE_STATUS::UPDATE_CONTINUE;
}