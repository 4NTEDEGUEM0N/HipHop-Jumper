#include "../include/AIController.hpp"

#include "../include/Character.hpp"
#include "../include/GameObject.hpp"
#include <iostream>

using namespace std;

AIController::AIController(GameObject &associated): Component(associated) {
    npcCounter = 0;
    state = AIState::RESTING;
    restTimer.Restart();
    destination = Vec2(0,0);
}

void AIController::Update(float dt) {
    Component* component = associated.GetComponent("Character");
    Character* character = dynamic_cast<Character*>(component);
    if (Character::player == nullptr) {
        state = AIState::RESTING;
        return;
    }
    if (state == AIState::RESTING) {
        restTimer.Update(dt);
        character->Issue(Character::Command(Character::Command::MOVE, 0, 0));
        if (restTimer.Get() > 2.5) {
            state = AIState::MOVING;
            destination = Character::player->GetPosition();
            Vec2 direction = destination - associated.box.center();
            character->Issue(Character::Command(Character::Command::MOVE, direction.GetX(), direction.GetY()));
        }
    } else if (state == AIState::MOVING) {
        Vec2 direction = destination - associated.box.center();
        character->Issue(Character::Command(Character::Command::MOVE, direction.GetX(), direction.GetY()));
        if (abs(destination.GetX() - associated.box.center().GetX()) < 5 && abs(destination.GetY() - associated.box.center().GetY()) < 5) {
            destination = Character::player->GetPosition();
            character->Issue(Character::Command(Character::Command::SHOOT, destination.GetX(), destination.GetY()));
            state = AIState::RESTING;
            restTimer.Restart();
        }

    }
}

void AIController::Render() {}

bool AIController::Is(string type) {
    return type == "AIController";
}


