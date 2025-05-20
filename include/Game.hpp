#ifndef GAME_HPP
#define GAME_HPP

#include <stack>
#include <SDL.h>
#include "State.hpp"
#include <string>

using namespace std;

class Game {
public:
    Game(string title, int width, int height);
    ~Game();
    static void Quit();

    static Game& GetInstance();
    SDL_Renderer* GetRenderer();
    State& GetState();

    void Push(State* state);

    void Run();

    float GetDeltaTime();

    static int ScreenWidth;
    static int ScreenHeight;

private:
    void CalculateDeltaTime();

    int frameStart = 0;
    float dt = 0;

    static Game* instance;

    State* storedState = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    stack<unique_ptr<State>> stateStack;

};

#endif //GAME_HPP
