#include "ModuleCollisions.h"
#include "Game.h"

#include "ModuleRender.h"
#include "ModuleInput.h"
#include "SDL/include/SDL_Scancode.h"

ModuleCollisions::ModuleCollisions() {
    for (uint i = 0; i < MAX_COLLIDERS; ++i) { colliders[i] = nullptr; }

    matrix[Collider::TYPE::WALL][Collider::TYPE::WALL] = false;
    matrix[Collider::TYPE::WALL][Collider::TYPE::PLAYER] = true;
    matrix[Collider::TYPE::WALL][Collider::TYPE::BALLOON] = true;
    matrix[Collider::TYPE::WALL][Collider::TYPE::ANIMAL] = false;
    matrix[Collider::TYPE::WALL][Collider::TYPE::PLAYER_SHOT] = true;

    matrix[Collider::TYPE::PLAYER][Collider::TYPE::WALL] = true;
    matrix[Collider::TYPE::PLAYER][Collider::TYPE::PLAYER] = false;
    matrix[Collider::TYPE::PLAYER][Collider::TYPE::BALLOON] = true;
    matrix[Collider::TYPE::PLAYER][Collider::TYPE::ANIMAL] = true;
    matrix[Collider::TYPE::PLAYER][Collider::TYPE::PLAYER_SHOT] = false;

    matrix[Collider::TYPE::BALLOON][Collider::TYPE::WALL] = true;
    matrix[Collider::TYPE::BALLOON][Collider::TYPE::PLAYER] = true;
    matrix[Collider::TYPE::BALLOON][Collider::TYPE::BALLOON] = true;
    matrix[Collider::TYPE::BALLOON][Collider::TYPE::ANIMAL] = false;
    matrix[Collider::TYPE::BALLOON][Collider::TYPE::PLAYER_SHOT] = true;

    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::WALL] = false;
    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::PLAYER] = true;
    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::BALLOON] = false;
    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::ANIMAL] = false;
    matrix[Collider::TYPE::ANIMAL][Collider::TYPE::PLAYER_SHOT] = true;

    matrix[Collider::TYPE::PLAYER_SHOT][Collider::TYPE::WALL] = true;
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
        if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true) {
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

            if (c1->Intersects(c2->rect) && matrix[c1->type][c2->type]) {
                if (c1->listener) { c1->listener->OnCollision(c1, c2); }
                if (c2->listener) { c2->listener->OnCollision(c2, c1); }
            }
        }
    }

    return UPDATE_STATUS::UPDATE_CONTINUE;
}

UPDATE_STATUS ModuleCollisions::Update() {
    if (game->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) { debug = !debug; }
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

        switch (colliders[i]->type) {
        case Collider::TYPE::NONE: // white
            game->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
            break;
        case Collider::TYPE::WALL: // magenta
            game->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
            break;
        case Collider::TYPE::PLAYER: // green
            game->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
            break;
        case Collider::TYPE::BALLOON: // red
            game->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
            break;
        case Collider::TYPE::ANIMAL: // yellow
            game->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
            break;
        case Collider::TYPE::PLAYER_SHOT: // blue
            game->render->DrawQuad(colliders[i]->rect, 0, 0, 255 , alpha);
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