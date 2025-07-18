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
    void SuperPop();
    void Run();

    float GetDeltaTime();
    State* GetStoredState();

    static int VirtualScreenWidth;
    static int VirtualScreenHeight;

    static int RealScreenWidth;
    static int RealScreenHeight;

private:
    void CalculateDeltaTime();

    int frameStart = 0;
    float dt = 0;

    static Game* instance;

    State* storedState = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    stack<unique_ptr<State>> stateStack;

    bool superPopRequested = false;

};

#endif //GAME_HPP
