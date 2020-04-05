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
}
ModulePlayer::~ModulePlayer() {}

int ModulePlayer::GetSpeed() const { return speed; }
void ModulePlayer::SetSpeed(int _speed) { speed = _speed; }
iPoint ModulePlayer::GetPosition() const { return position; }
void ModulePlayer::SetPosition(iPoint _position) { position = _position; }
SDL_Texture* ModulePlayer::GetTexture() const { return texture; }

bool ModulePlayer::Start()
{
    LOG("Loading player textures");

    texture = game->textures->Load("Resources/Sprites/player.png"); // arcade version

    return true;
}

UPDATE_STATUS ModulePlayer::Update()
{
    //Reset the currentAnimation back to idle before updating the logic
    //currentAnimation = &idleAnim;

    if (game->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
    {
        //currentAnimation = &forwardAnim;
        position.x += speed;
    }
    if (game->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
    {
        //currentAnimation = &backwardAnim;
        position.x -= speed;

    }

    //currentAnimation->Update();

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModulePlayer::PostUpdate() {
    //    SDL_Rect rect = currentAnimation->GetCurrentFrame();
    SDL_Rect rect = { 12,2,29,31 };
    game->render->Blit(texture, position.x, position.y - rect.h, &rect);

    return UPDATE_STATUS::UPDATE_CONTINUE;
}