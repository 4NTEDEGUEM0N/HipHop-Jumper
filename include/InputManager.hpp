#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#define LEFT_ARROW_KEY SDLK_LEFT
#define RIGHT_ARROW_KEY SDLK_RIGHT
#define UP_ARROW_KEY SDLK_UP
#define DOWN_ARROW_KEY SDLK_DOWN
#define ESCAPE_KEY SDLK_ESCAPE
#define LEFT_MOUSE_BUTTON SDL_BUTTON_LEFT

#include <SDL_keycode.h>
#include <SDL_render.h>
#include <unordered_map>

using namespace std;

class InputManager {
private:
    bool mouseState[6];
    int mouseUpdate[6];
    unordered_map<int, bool> keyState;
    unordered_map<int, int> keyUpdate;
    bool quitRequested;
    int updateCounter;
    int mouseX;
    int mouseY;
    SDL_Renderer* renderer;

public:
    void Update();
    bool KeyPress(int key);
    bool KeyRelease(int key);
    bool IsKeyDown(int key);
    bool MousePress(int button);
    bool MouseRelease(int button);
    bool IsMouseDown(int button);
    int GetMouseX();
    int GetMouseY();
    bool QuitRequested();
    static InputManager& GetInstance();
    InputManager();
    ~InputManager() = default;
    void SetRenderer(SDL_Renderer* renderer);
};


#endif //INPUTMANAGER_HPP
