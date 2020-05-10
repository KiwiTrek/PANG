#include "ModuleCollisions.h"

#include "Game.h"

#include "ModuleRender.h"
#include "ModuleInput.h"

#include "SDL/include/SDL_Scancode.h"

ModuleCollisions::ModuleCollisions(bool startEnabled) : Module(startEnabled) {
    for (uint i = 0; i < MAX_COLLIDERS; ++i) { colliders[i] = nullptr; }

    matrix[Collider::TYPE::WALL][Collider::TYPE::NONE] = false;
    matrix[Collider::TYPE::WALL][Collider::TYPE::WALL] = false;
    matrix[Collider::TYPE::WALL][Collider::TYPE::FLOOR] = false;
    matrix[Collider::TYPE::WALL][Collider::TYPE::STAIRS] = false;
    matrix[Collider::TYPE::WALL][Collider::TYPE::PLAYER] = true;
    matrix[Collider::TYPE::WALL][Collider::TYPE::BALLOON] = true;
    matrix[Collider::TYPE::WALL][Collider::TYPE::ANIMAL] = false;
    matrix[Collider::TYPE::WALL][Collider::TYPE::PLAYER_SHOT] = true;

    matrix[Collider::TYPE::FLOOR][Collider::TYPE::NONE] = false;
    matrix[Collider::TYPE::FLOOR][Collider::TYPE::WALL] = false;
    matrix[Collider::TYPE::FLOOR][Collider::TYPE::FLOOR] = false;
    matrix[Collider::TYPE::FLOOR][Collider::TYPE::STAIRS] = false;
    matrix[Collider::TYPE::FLOOR][Collider::TYPE::PLAYER] = true;
    matrix[Collider::TYPE::FLOOR][Collider::TYPE::BALLOON] = true;
    matrix[Collider::TYPE::FLOOR][Collider::TYPE::ANIMAL] = false;
    matrix[Collider::TYPE::FLOOR][Collider::TYPE::PLAYER_SHOT] = true;

    matrix[Collider::TYPE::PLAYER][Collider::TYPE::NONE] = false;
    matrix[Collider::TYPE::PLAYER][Collider::TYPE::WALL] = true;
    matrix[Collider::TYPE::PLAYER][Collider::TYPE::FLOOR] = true;
    matrix[Collider::TYPE::PLAYER][Collider::TYPE::STAIRS] = true;
    matrix[Collider::TYPE::PLAYER][Collider::TYPE::PLAYER] = false;
    matrix[Collider::TYPE::PLAYER][Collider::TYPE::BALLOON] = true;
    matrix[Collider::TYPE::PLAYER][Collider::TYPE::ANIMAL] = true;
    matrix[Collider::TYPE::PLAYER][Collider::TYPE::PLAYER_SHOT] = false;

    matrix[Collider::TYPE::BALLOON][Collider::TYPE::NONE] = false;
    matrix[Collider::TYPE::BALLOON][Collider::TYPE::WALL] = true;
    matrix[Collider::TYPE::BALLOON][Collider::TYPE::FLOOR] = true;
    matrix[Collider::TYPE::BALLOON][Collider::TYPE::STAIRS] = false;
    matrix[Collider::TYPE::BALLOON][Collider::TYPE::PLAYER] = true;
    matrix[Collider::TYPE::BALLOON][Collider::TYPE::BALLOON] = true;
    matrix[Collider::TYPE::BALLOON][Collider::TYPE::ANIMAL] = false;
    matrix[Collider::TYPE::BALLOON][Collider::TYPE::PLAYER_SHOT] = true;

    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::NONE] = false;
    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::WALL] = false;
    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::FLOOR] = false;
    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::STAIRS] = false;
    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::PLAYER] = true;
    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::BALLOON] = false;
    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::ANIMAL] = false;
    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::PLAYER_SHOT] = true;

    matrix[Collider::TYPE::PLAYER_SHOT][Collider::TYPE::NONE] = false;
    matrix[Collider::TYPE::PLAYER_SHOT][Collider::TYPE::WALL] = true;
    matrix[Collider::TYPE::PLAYER_SHOT][Collider::TYPE::FLOOR] = true;
    matrix[Collider::TYPE::PLAYER_SHOT][Collider::TYPE::STAIRS] = false;
    matrix[Collider::TYPE::PLAYER_SHOT][Collider::TYPE::PLAYER] = false;
    matrix[Collider::TYPE::PLAYER_SHOT][Collider::TYPE::BALLOON] = true;
    matrix[Collider::TYPE::PLAYER_SHOT][Collider::TYPE::ANIMAL] = true;
    matrix[Collider::TYPE::PLAYER_SHOT][Collider::TYPE::PLAYER_SHOT] = false;
}

// Destructor
ModuleCollisions::~ModuleCollisions() {}

UPDATE_STATUS ModuleCollisions::PreUpdate() {
    // Remove all colliders scheduled for deletion
    for (uint i = 0; i < MAX_COLLIDERS; ++i) {
        if (colliders[i] != nullptr && colliders[i]->CheckPendingToDelete() == true) {
            delete colliders[i];
            colliders[i] = nullptr;
        }
    }

    Collider* c1;
    Collider* c2;

    for (uint i = 0; i < MAX_COLLIDERS; ++i) {
        // skip empty colliders
        if (colliders[i] == nullptr) { continue; }

        c1 = colliders[i];

        // avoid checking collisions already checked
        for (uint k = i + 1; k < MAX_COLLIDERS; ++k) {
            // skip empty colliders
            if (colliders[k] == nullptr) { continue; }

            c2 = colliders[k];

            if (c1->Intersects(c2->GetRect()) && matrix[c1->GetType()][c2->GetType()]) {
                if (c1->GetListener()) { c1->GetListener()->OnCollision(c1, c2); }
                if (c2->GetListener()) { c2->GetListener()->OnCollision(c2, c1); }
            }
        }
    }

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleCollisions::Update() {
    if (game->GetModuleInput()->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) { debug = !debug; }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleCollisions::PostUpdate() {
    if (debug) { DebugDraw(); }
    return UPDATE_STATUS::UPDATE_CONTINUE;
}

void ModuleCollisions::DebugDraw() {
    Uint8 alpha = 80;
    for (uint i = 0; i < MAX_COLLIDERS; ++i) {
        if (colliders[i] == nullptr) { continue; }

        switch (colliders[i]->GetType()) {
        case Collider::TYPE::WALL: // yellow
            game->GetModuleRender()->DrawQuad(colliders[i]->GetRect(), 255, 255, 0, alpha);
            break;
        case Collider::TYPE::FLOOR: // yellow
            game->GetModuleRender()->DrawQuad(colliders[i]->GetRect(), 255, 255, 0, alpha);
            break;
        case Collider::TYPE::STAIRS: // magenta
            game->GetModuleRender()->DrawQuad(colliders[i]->GetRect(), 255, 0, 255, alpha);
            break;
        case Collider::TYPE::PLAYER: // red
            game->GetModuleRender()->DrawQuad(colliders[i]->GetRect(), 255, 0, 0, alpha);
            break;
        case Collider::TYPE::BALLOON: // green
            game->GetModuleRender()->DrawQuad(colliders[i]->GetRect(), 0, 255, 0, alpha);
            break;
        case Collider::TYPE::ANIMAL: // blue
            game->GetModuleRender()->DrawQuad(colliders[i]->GetRect(), 0, 0, 255, alpha);
            break;
        case Collider::TYPE::PLAYER_SHOT: // cyan
            game->GetModuleRender()->DrawQuad(colliders[i]->GetRect(), 0, 255, 255 , alpha);
            break;
        }
    }
}

// Called before quitting
bool ModuleCollisions::CleanUp() {
    LOG("Freeing all colliders");

    for (uint i = 0; i < MAX_COLLIDERS; ++i) {
        if (colliders[i] != nullptr) {
            delete colliders[i];
            colliders[i] = nullptr;
        }
    }

    return true;
}

Collider* ModuleCollisions::AddCollider(SDL_Rect rect, Collider::TYPE type, Module* listener) {
    Collider* ret = nullptr;

    for (uint i = 0; i < MAX_COLLIDERS; ++i) {
        if (colliders[i] == nullptr) {
            ret = colliders[i] = new Collider(rect, type, listener);
            break;
        }
    }

    return ret;
}