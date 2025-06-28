#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "include/Game.hpp"
#include <iostream>
#include "include/EndState.hpp"
#include "include/StageState.hpp"
#include "include/TitleState.hpp"
#include "include/PauseState.hpp"

int main (int argc, char** argv) {
    Game& game = Game::GetInstance();
    TitleState* tileState = new TitleState();
    game.Push(tileState);
    game.Run();
    Game::Quit();

    return 0;
}
