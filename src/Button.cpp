#include "../include/Button.hpp"

#include <SDL_mouse.h>

#include "../include/GameObject.hpp"
#include "../include/InputManager.hpp"

Button::Button(GameObject &associated) : Component(associated),
    clickSound("../Recursos/audio/UX/MENU CLICK 1.wav"),
    hoverSound("../Recursos/audio/UX/MENU CLICK 3.wav")
{
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
                clickSound.Play(1);
        } else {
            if (Hover)
                Hover();
                    if (!hoverSoundPlayed) {
                        // desativado por enquanto pois muitos botoes fazendo barulho
                        //hoverSound.Play(1);
                        hoverSoundPlayed = true;
                    }
        }
    } else {
        if (None)
            None();
            hoverSoundPlayed = false;
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

