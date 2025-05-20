#include "../include/Camera.hpp"
#include "../include/Game.hpp"

#include <iostream>

#include "../include/InputManager.hpp"

GameObject* Camera::focus = nullptr;
Vec2 Camera::pos = Vec2(0, 0);
Vec2 Camera::speed = Vec2(0, 0);
int Camera::speedMultiplier = 200;

void Camera::Follow(GameObject* newFocus) {
    focus = newFocus;
}

void Camera::Unfollow() {
    focus = nullptr;
}

void Camera::Update(float dt) {
    if (focus != nullptr) {
        pos = focus->box.center() - Vec2(Game::ScreenWidth / 2, Game::ScreenHeight / 2);
    } else {
        speed = Vec2(0, 0);
        if (InputManager::GetInstance().IsKeyDown(UP_ARROW_KEY) || InputManager::GetInstance().IsKeyDown(SDLK_w)) {
            speed = speed + Vec2(0, -1);
        }
        if (InputManager::GetInstance().IsKeyDown(DOWN_ARROW_KEY) || InputManager::GetInstance().IsKeyDown(SDLK_s)) {
            speed = speed + Vec2(0, 1);
        }
        if (InputManager::GetInstance().IsKeyDown(LEFT_ARROW_KEY) || InputManager::GetInstance().IsKeyDown(SDLK_a)) {
            speed = speed + Vec2(-1, 0);
        }
        if (InputManager::GetInstance().IsKeyDown(RIGHT_ARROW_KEY) || InputManager::GetInstance().IsKeyDown(SDLK_d)) {
            speed = speed + Vec2(1, 0);
        }

        Vec2 speed_normalize = speed.normalize();
        pos = pos + (speed_normalize * dt) * speedMultiplier;
    }
}