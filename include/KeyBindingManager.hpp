#ifndef KEYBINDINGMANAGER_HPP
#define KEYBINDINGMANAGER_HPP

#include <SDL_keycode.h>
#include <map>

class KeyBindingManager {
public:
    enum GameAction {
        JUMP,
        MOVE_LEFT,
        MOVE_RIGHT,
        DASH,
        NOTEBOOK,
        GRAFFITI
    };

    enum MouseAction {
        SHOOT
    };

    static KeyBindingManager& GetInstance();

    void SetKeyForAction(GameAction action, SDL_Keycode key);
    void SetButtonForAction(MouseAction action, int button);

    bool IsActionPressed(GameAction action);
    bool IsActionReleased(GameAction action);
    bool IsActionDown(GameAction action);

    bool IsMouseActionPressed(MouseAction action);
    bool IsMouseActionReleased(MouseAction action);
    bool IsMouseActionDown(MouseAction action);

    SDL_Keycode GetKeyForAction(GameAction action);
    SDL_Keycode GetButtonForAction(MouseAction action);

    SDL_Keycode GetNextKeyPressed();

    void LoadDefaultBindings();

private:
    KeyBindingManager();

    std::map<GameAction, SDL_Keycode> keyActionMap;
    std::map<MouseAction, int> mouseActionMap;
};

#endif //KEYBINDINGMANAGER_HPP
