#include "../include/PlayerController.hpp"

#include <iostream>

#include "../include/InputManager.hpp"
#include "../include/Character.hpp"
#include "../include/GameObject.hpp"
#include "../include/Camera.hpp"
#include "../include/KeyBindingManager.hpp"


void PlayerController::Update(float dt) {
    bool move = false;
    bool shot = false;
    Component* component = associated.GetComponent("Character");
    Character* character = dynamic_cast<Character*>(component);
    InputManager& inputManager = InputManager::GetInstance();
    KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
    Vec2 direction = Vec2(0, 0);
    if (keybinder.IsActionPressed(KeyBindingManager::GameAction::JUMP)) {
        character->Issue(Character::Command(Character::Command::JUMP, 0, 0));
    }
    if (keybinder.IsActionDown(KeyBindingManager::GameAction::MOVE_LEFT)) {
        direction = direction + Vec2(-1, 0);
        move = true;
    }
    if (keybinder.IsActionDown(KeyBindingManager::GameAction::MOVE_RIGHT)) {
        direction = direction + Vec2(1, 0);
        move = true;
    }
    if (keybinder.IsMouseActionPressed(KeyBindingManager::MouseAction::SHOOT)) {
        float mouseX = inputManager.GetMouseX() + Camera::pos.GetX();
        float mouseY = inputManager.GetMouseY() + Camera::pos.GetY();
        shot = true;
        character->Issue(Character::Command(Character::Command::SHOOT, mouseX, mouseY));
    }
    if (keybinder.IsActionPressed(KeyBindingManager::GameAction::DASH)) {
        character->Issue(Character::Command(Character::Command::DASH, direction.X, 0));
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