#include "../include/PlayerController.hpp"

#include <iostream>

#include "../include/InputManager.hpp"
#include "../include/Character.hpp"
#include "../include/GameObject.hpp"
#include "../include/Camera.hpp"


void PlayerController::Update(float dt) {
    bool move = false;
    bool shot = false;
    Component* component = associated.GetComponent("Character");
    Character* character = dynamic_cast<Character*>(component);
    Vec2 direction = Vec2(0, 0);
    if (InputManager::GetInstance().KeyPress(SDLK_w)) {
        character->Issue(Character::Command(Character::Command::JUMP, 0, 0));
    }
    if (InputManager::GetInstance().IsKeyDown(SDLK_s)) {
        //direction = direction + Vec2(0, 1);
        //move = true;
    }
    if (InputManager::GetInstance().IsKeyDown(SDLK_a)) {
        direction = direction + Vec2(-1, 0);
        move = true;
    }
    if (InputManager::GetInstance().IsKeyDown(SDLK_d)) {
        direction = direction + Vec2(1, 0);
        move = true;
    }
    if (InputManager::GetInstance().MousePress(1)) {
        float mouseX = InputManager::GetInstance().GetMouseX() + Camera::pos.GetX();
        float mouseY = InputManager::GetInstance().GetMouseY() + Camera::pos.GetY();
        shot = true;
        character->Issue(Character::Command(Character::Command::SHOOT, mouseX, mouseY));
    }
    if (move)
        character->Issue(Character::Command(Character::Command::MOVE, direction.GetX(), direction.GetY()));
    else if (!shot)
        character->Issue(Character::Command(Character::Command::MOVE, 0, 0));
}

void PlayerController::Render() {
}

bool PlayerController::Is(string type) {
    return type == "PlayerController";
}

PlayerController::PlayerController(GameObject& associated) : Component(associated) {
}

void PlayerController::Start() {
}