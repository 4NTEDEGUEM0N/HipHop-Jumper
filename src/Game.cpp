#include "../include/Game.hpp"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>


#include "../include/Resources.hpp"
#include "../include/InputManager.hpp"

using namespace std;

Game* Game::instance = nullptr;
int Game::VirtualScreenWidth = 1200;
int Game::VirtualScreenHeight = 900;

int Game::RealScreenWidth = 960;
int Game::RealScreenHeight = 720;

Game& Game::GetInstance(){
    if(instance != nullptr){
        return *instance;
    }
    instance = new Game("Hip-Hop Jumper", RealScreenWidth, RealScreenHeight);
    return *instance;
}

Game::Game(string title, int width, int height){
    if(instance != nullptr){
        cerr << ("Erro: Instância já existe") << endl;
        exit(1);
    }
    instance = this;
    storedState = nullptr;

    int sdl_init = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    if (sdl_init != 0){
        cerr << "Erro - SDL_Init: " << SDL_GetError() << endl;
        exit(1);
    }

    //int img_init = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
    int img_init = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    if (img_init != (IMG_INIT_JPG | IMG_INIT_PNG)) {
        cerr << "Erro - IMG_Init: "<< SDL_GetError() << endl;
        exit(1);
    }

    int mix_init = Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
    if (mix_init != (MIX_INIT_MP3 | MIX_INIT_OGG)) {
        cerr << "Erro - Mix_Init: "<< SDL_GetError() << endl;
        exit(1);
    }

    int mix_openaudio = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    if (mix_openaudio != 0) {
        cerr << "Erro - Mix_OpenAudio: "<< SDL_GetError() << endl;
        exit(1);
    }
    Mix_AllocateChannels(32);

    int tff_init = TTF_Init();
    if (tff_init != 0) {
        cerr << "Erro - TTF_Init: "<< SDL_GetError() << endl;
        exit(1);
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        cerr << "Erro - SDL_CreateWindow: "<< SDL_GetError() << endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cerr << "Erro - SDL_CreateRenderer: "<< SDL_GetError() << endl;
        exit(1);
    }

    InputManager::GetInstance().SetRenderer(renderer);

    SDL_RenderSetLogicalSize(renderer, VirtualScreenWidth, VirtualScreenHeight);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    srand(time(NULL));
}

Game::~Game(){
    if (storedState != nullptr) {
        delete storedState;
        storedState = nullptr;
    }
    while (!stateStack.empty()) {
        stateStack.pop();
    }

    Resources::ClearImages();
    Resources::ClearMusics();
    Resources::ClearSounds();
    Resources::ClearFonts();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Renderer* Game::GetRenderer(){
    return renderer;
}

void Game::Run(){
    if (storedState != nullptr) {
        stateStack.push(unique_ptr<State>(storedState));
        stateStack.top()->Start();
        storedState = nullptr;
        while(!stateStack.empty() && !stateStack.top()->QuitRequested()){
            if (stateStack.top()->PopRequested()) {
                stateStack.pop();
                Resources::ClearImages();
                Resources::ClearMusics();
                Resources::ClearSounds();
                Resources::ClearFonts();
                if (!stateStack.empty())
                    stateStack.top()->Resume();
            }
            if (storedState != nullptr) {
                if (!stateStack.empty())
                    stateStack.top()->Pause();
                stateStack.push(unique_ptr<State>(storedState));
                stateStack.top()->Start();
                storedState = nullptr;
            }
            CalculateDeltaTime();
            InputManager::GetInstance().Update();
            stateStack.top()->Update(dt);
            stateStack.top()->Render();
            SDL_RenderPresent(renderer);
            SDL_Delay(15);
            //cerr << "FPS: " << (1.0 / GetInstance().GetDeltaTime()) << endl;
        }
    }
}

void Game::Quit() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

void Game::CalculateDeltaTime(){
    int newFrameStart = SDL_GetTicks();
    dt = (newFrameStart - frameStart) / 1000.0;
    frameStart = newFrameStart;
}

float Game::GetDeltaTime(){
    return dt;
}

State& Game::GetState() {
    if (stateStack.empty()) {
        throw runtime_error("stateStack vazia");
    }
    return *stateStack.top();
}

void Game::Push(State* state) {
    storedState = state;
}



