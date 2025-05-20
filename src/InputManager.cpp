#include "../include/InputManager.hpp"

#include <cstring>
#include <iostream>
#include <SDL_events.h>

InputManager& InputManager::GetInstance() {
    static InputManager instance;
    return instance;
}

InputManager::InputManager() {
    memset(mouseState, false, sizeof(mouseState));
    memset(mouseUpdate, 0, sizeof(mouseUpdate));
    quitRequested = false;
    updateCounter = 0;
    mouseX = 0;
    mouseY = 0;
}

void InputManager::Update() {
    quitRequested = false;
    updateCounter++;
    int realMouseX, realMouseY;
    SDL_GetMouseState(&realMouseX, &realMouseY);

    float logicalMouseX, logicalMouseY;
    SDL_RenderWindowToLogical(renderer, realMouseX, realMouseY, &logicalMouseX, &logicalMouseY);
    mouseX = (int)logicalMouseX;
    mouseY = (int)logicalMouseY;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quitRequested = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseState[event.button.button] = true;
                mouseUpdate[event.button.button] = updateCounter;
                break;
            case SDL_MOUSEBUTTONUP:
                mouseState[event.button.button] = false;
                mouseUpdate[event.button.button] = updateCounter;
                break;
            case SDL_KEYDOWN:
                if (event.key.repeat == 0) {
                    keyState[event.key.keysym.sym] = true;
                    keyUpdate[event.key.keysym.sym] = updateCounter;
                }
                break;
            case SDL_KEYUP:
                keyState[event.key.keysym.sym] = false;
                keyUpdate[event.key.keysym.sym] = updateCounter;
                break;
            default:
                break;
        }
    }
}

bool InputManager::KeyPress(int key) {
    return (keyUpdate.find(key) != keyUpdate.end()) && (keyUpdate[key] == updateCounter && keyState[key]);
}

bool InputManager::KeyRelease(int key) {
    return (keyUpdate.find(key) != keyUpdate.end()) && (keyUpdate[key] == updateCounter && !keyState[key]);
}

bool InputManager::IsKeyDown(int key) {
    return (keyState.find(key) != keyState.end()) && keyState[key];
}

bool InputManager::MousePress(int button) {
    return mouseUpdate[button] == updateCounter && mouseState[button];
}

bool InputManager::MouseRelease(int button) {
    return mouseUpdate[button] == updateCounter && !mouseState[button];
}

bool InputManager::IsMouseDown(int button) {
    return mouseState[button];
}

int InputManager::GetMouseX() {
    return mouseX;
}

int InputManager::GetMouseY() {
    return mouseY;
}

bool InputManager::QuitRequested() {
    return quitRequested;
}

void InputManager::SetRenderer(SDL_Renderer* renderer) {
    this->renderer = renderer;
}