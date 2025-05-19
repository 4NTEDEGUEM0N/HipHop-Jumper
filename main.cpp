#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "include/Game.hpp"
#include <iostream>
#include "include/EndState.hpp"
#include "include/StageState.hpp"
#include "include/TitleState.hpp"

int main (int argc, char** argv) {
    Game& game = Game::GetInstance();
    //StageState* stageState = new StageState();
    TitleState* tileState = new TitleState();
    //EndState* endState = new EndState();
    game.Push(tileState);
    game.Run();
    Game::Quit();

    return 0;
}
