#include "ModuleCollisions.h"
#include "Game.h"

#include "ModuleRender.h"
#include "ModuleInput.h"
#include "SDL/include/SDL_Scancode.h"

ModuleCollisions::ModuleCollisions() {
    for (uint i = 0; i < MAX_COLLIDERS; ++i) { colliders[i] = nullptr; }

    matrix[Collider::Type::WALL][Collider::Type::WALL] = false;
    matrix[Collider::Type::WALL][Collider::Type::PLAYER] = true;
    matrix[Collider::Type::WALL][Collider::Type::BALLOON] = true;
    matrix[Collider::Type::WALL][Collider::Type::ANIMAL] = false;
    matrix[Collider::Type::WALL][Collider::Type::PLAYER_SHOT] = true;

    matrix[Collider::Type::PLAYER][Collider::Type::WALL] = true;
    matrix[Collider::Type::PLAYER][Collider::Type::PLAYER] = false;
    matrix[Collider::Type::PLAYER][Collider::Type::BALLOON] = true;
    matrix[Collider::Type::PLAYER][Collider::Type::ANIMAL] = true;
    matrix[Collider::Type::PLAYER][Collider::Type::PLAYER_SHOT] = false;

    matrix[Collider::Type::BALLOON][Collider::Type::WALL] = true;
    matrix[Collider::Type::BALLOON][Collider::Type::PLAYER] = true;
    matrix[Collider::Type::BALLOON][Collider::Type::BALLOON] = true;
    matrix[Collider::Type::BALLOON][Collider::Type::ANIMAL] = false;
    matrix[Collider::Type::BALLOON][Collider::Type::PLAYER_SHOT] = true;

    matrix[Collider::Type::ANIMAL][Collider::Type::WALL] = false;
    matrix[Collider::Type::ANIMAL][Collider::Type::PLAYER] = true;
    matrix[Collider::Type::ANIMAL][Collider::Type::BALLOON] = false;
    matrix[Collider::Type::ANIMAL][Collider::Type::ANIMAL] = false;
    matrix[Collider::Type::ANIMAL][Collider::Type::PLAYER_SHOT] = true;

    matrix[Collider::Type::PLAYER_SHOT][Collider::Type::WALL] = true;
    matrix[Collider::Type::PLAYER_SHOT][Collider::Type::PLAYER] = false;
    matrix[Collider::Type::PLAYER_SHOT][Collider::Type::BALLOON] = true;
    matrix[Collider::Type::PLAYER_SHOT][Collider::Type::ANIMAL] = true;
    matrix[Collider::Type::PLAYER_SHOT][Collider::Type::PLAYER_SHOT] = false;
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
        case Collider::Type::NONE: // white
            game->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
            break;
        case Collider::Type::WALL: // blue
            game->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
            break;
        case Collider::Type::PLAYER: // green
            game->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
            break;
        case Collider::Type::BALLOON: // red
            game->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
            break;
        case Collider::Type::ANIMAL: // yellow
            game->render->DrawQuad(colliders[i]->rect, 0, 255, 255, alpha);
            break;
        case Collider::Type::PLAYER_SHOT: // magenta
            game->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
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

Collider* ModuleCollisions::AddCollider(SDL_Rect rect, Collider::Type type, Module* listener) {
    Collider* ret = nullptr;

    for (uint i = 0; i < MAX_COLLIDERS; ++i) {
        if (colliders[i] == nullptr) {
            ret = colliders[i] = new Collider(rect, type, listener);
            break;
        }
    }

    return ret;
}