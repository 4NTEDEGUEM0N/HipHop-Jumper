#include "../include/Button.hpp"

#include <SDL_mouse.h>

#include "../include/GameObject.hpp"
#include "../include/InputManager.hpp"

Button::Button(GameObject &associated) : Component(associated) {
    clickable = true;
}

void Button::Update(float dt) {
    auto inputManager = InputManager::GetInstance();
    int mouseX = inputManager.GetMouseX();
    int mouseY = inputManager.GetMouseY();
    Vec2 mousePos = Vec2(mouseX, mouseY);

    if (associated.box.contains(mousePos)) {
        if (inputManager.MousePress(SDL_BUTTON_LEFT)) {
            if (Click)
                Click();
        } else {
            if (Hover)
                Hover();
        }
    } else {
        if (None)
            None();
    }
}

void Button::Render() {}

bool Button::Is(string type) {
    return type == "Button";
}

void Button::SetClickFunction(function<void()> func) {
    Click = func;
}

void Button::SetHoverFunction(function<void()> func) {
    Hover = func;
}

void Button::SetNoneFunction(function<void()> func) {
    None = func;
}

