#include "../include/KeyBindingManager.hpp"
#include "../include/InputManager.hpp"
#include <SDL_events.h>

#include "../include/OptionsState.hpp"

KeyBindingManager& KeyBindingManager::GetInstance() {
    static KeyBindingManager instance;
    return instance;
}

KeyBindingManager::KeyBindingManager() {
    LoadDefaultBindings();
}

void KeyBindingManager::LoadDefaultBindings() {
    keyActionMap[GameAction::JUMP] = SDLK_w;
    keyActionMap[GameAction::MOVE_LEFT] = SDLK_a;
    keyActionMap[GameAction::MOVE_RIGHT] = SDLK_d;
    keyActionMap[GameAction::DASH] = SDLK_LSHIFT;
    keyActionMap[GameAction::NOTEBOOK] = SDLK_g;
    keyActionMap[GameAction::GRAFFITI] = SDLK_e;
    keyActionMap[GameAction::SHOOT] = SDLK_f;
}

void KeyBindingManager::SetKeyForAction(GameAction action, SDL_Keycode key) {
    keyActionMap[action] = key;
}

void KeyBindingManager::SetButtonForAction(MouseAction action, int button) {
    mouseActionMap[action] = button;
}

bool KeyBindingManager::IsActionPressed(GameAction action) {
    SDL_Keycode key = keyActionMap[action];
    return InputManager::GetInstance().KeyPress(key);
}

bool KeyBindingManager::IsActionReleased(GameAction action) {
    SDL_Keycode key = keyActionMap[action];
    return InputManager::GetInstance().KeyRelease(key);
}

bool KeyBindingManager::IsActionDown(GameAction action) {
    SDL_Keycode key = keyActionMap[action];
    return InputManager::GetInstance().IsKeyDown(key);
}

bool KeyBindingManager::IsMouseActionPressed(MouseAction action) {
    int button = mouseActionMap[action];
    return InputManager::GetInstance().MousePress(button);
}

bool KeyBindingManager::IsMouseActionReleased(MouseAction action) {
    int button = mouseActionMap[action];
    return InputManager::GetInstance().MouseRelease(button);
}

bool KeyBindingManager::IsMouseActionDown(MouseAction action) {
    int button = mouseActionMap[action];
    return InputManager::GetInstance().IsMouseDown(button);
}

SDL_Keycode KeyBindingManager::GetKeyForAction(GameAction action) {
    return keyActionMap[action];
}

SDL_Keycode KeyBindingManager::GetButtonForAction(MouseAction action) {
    return mouseActionMap[action];
}

SDL_Keycode KeyBindingManager::GetNextKeyPressed() {
    SDL_Event event;
    while (true) {
        if (SDL_WaitEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    return GetKeyForAction(OptionsState::actionToRebind);
                }
                return event.key.keysym.sym;
            }
            if (event.type == SDL_QUIT) {
                return GetKeyForAction(OptionsState::actionToRebind);
            }
        }
    }
}
