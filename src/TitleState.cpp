#include "../include/TitleState.hpp"

#include <iostream>
#include <ostream>

#include "../include/SpriteRenderer.hpp"
#include "../include/Game.hpp"
#include "../include/InputManager.hpp"
#include "../include/Button.hpp"
#include "../include/OptionsState.hpp"
#include "../include/Text.hpp"
#include "../include/StageState.hpp"
#include "../include/GameData.hpp"
#include "../include/LevelSelectorState.hpp"

TitleState::TitleState() {
    GameObject* bgObject = new GameObject();
    AddObject(bgObject);
    SpriteRenderer* bg = new SpriteRenderer(*bgObject,  "../Recursos/img/Menu BG.png");
    bg->SetCameraFollower(true);
    bgObject->AddComponent(bg);
    float scaleX = Game::VirtualScreenWidth  / (bgObject->box.W);
    float scaleY = Game::VirtualScreenHeight / (bgObject->box.H);
    bg->SetScale(scaleX, scaleY);
    bgObject->box.X = 0;
    bgObject->box.Y = 0;

    backgroundMusic.Open("../Recursos/audio/TRACKS/MAIN MENU TRACK LOOP.wav");
    backgroundMusic.SetIntro("../Recursos/audio/TRACKS/MAIN MENU TRACK START.wav");

    /*
    GameObject* logoObj = new GameObject();
    AddObject(logoObj);
    SpriteRenderer* logo = new SpriteRenderer(*logoObj, "../Recursos/img/hiphopjumper.png");
    logo->SetCameraFollower(true);
    logoObj->AddComponent(logo);
    //logo->SetScale(0.5f, 0.5f);
    logoObj->box.X = Game::VirtualScreenWidth/2 - logoObj->box.W/2;
    logoObj->box.Y = 20;
    */
    SDL_Color buttonNone = {47,63,68,255};
    //SDL_Color buttonNone = {87, 46, 73, 255};
    //SDL_Color buttonNone = {148, 0, 92, 255};
    SDL_Color buttonHover = {158,191,198,255};


    GameObject* optionsButtonObj = new GameObject();
    SpriteRenderer* optionsBgButton = new SpriteRenderer(*optionsButtonObj,  "../Recursos/img/MENU-BOTÃO-PREENCHIDO.png",2,1);
    optionsBgButton->SetCameraFollower(true);
    optionsButtonObj->AddComponent(optionsBgButton);
    AddObject(optionsButtonObj);
    optionsButtonObj->box.X = Game::VirtualScreenWidth/2 - optionsButtonObj->box.W/2;
    optionsButtonObj->box.Y = Game::VirtualScreenHeight/2 - optionsButtonObj->box.H/2;

    GameObject* optionsButtonObj2 = new GameObject();
    SpriteRenderer* optionsBgButton2 = new SpriteRenderer(*optionsButtonObj2,  "../Recursos/img/MENU-BOTÃO-2.png");
    optionsBgButton2->SetCameraFollower(true);
    optionsButtonObj2->AddComponent(optionsBgButton2);
    AddObject(optionsButtonObj2);
    optionsButtonObj2->box.X = Game::VirtualScreenWidth/2 - optionsButtonObj2->box.W/2 + 7;
    optionsButtonObj2->box.Y = Game::VirtualScreenHeight/2 - optionsButtonObj2->box.H/2 + 5;

    GameObject* optionsObj = new GameObject();
    Button* optionsButton = new Button(*optionsButtonObj);
    optionsButtonObj->AddComponent(optionsButton);
    optionsText = new Text(*optionsObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Options", buttonNone, true);
    optionsObj->AddComponent(optionsText);
    optionsObj->box.Z = 1;
    optionsObj->box.X = optionsButtonObj->box.X + optionsButtonObj->box.W/2 - optionsObj->box.W/2;
    optionsObj->box.Y = optionsButtonObj->box.Y;
    AddObject(optionsObj);
    optionsButton->SetClickFunction([this]() {
        Game& game = Game::GetInstance();
        OptionsState* optionsState = new OptionsState();
        game.Push(optionsState);
    });
    optionsButton->SetHoverFunction([this, buttonHover, optionsBgButton]() {
        optionsText->SetColor(buttonHover);
        optionsBgButton->SetFrame(1);
    });
    optionsButton->SetNoneFunction([this, buttonNone, optionsBgButton]() {
        optionsText->SetColor(buttonNone);
        optionsBgButton->SetFrame(0);
    });


    GameObject* playButtonObj = new GameObject();
    SpriteRenderer* playBgButton = new SpriteRenderer(*playButtonObj,  "../Recursos/img/MENU-BOTÃO-PREENCHIDO.png",2,1);
    playBgButton->SetCameraFollower(true);
    playButtonObj->AddComponent(playBgButton);
    AddObject(playButtonObj);
    playButtonObj->box.X = optionsButtonObj->box.X;
    playButtonObj->box.Y = optionsButtonObj->box.Y - playButtonObj->box.H - 40;

    GameObject* playButtonObj2 = new GameObject();
    SpriteRenderer* playBgButton2 = new SpriteRenderer(*playButtonObj2,  "../Recursos/img/MENU-BOTÃO-1.png");
    playBgButton2->SetCameraFollower(true);
    playButtonObj2->AddComponent(playBgButton2);
    AddObject(playButtonObj2);
    playButtonObj2->box.X = Game::VirtualScreenWidth/2 - playButtonObj2->box.W/2 - 7;
    playButtonObj2->box.Y = Game::VirtualScreenHeight/2 - playButtonObj2->box.H/2 - playButtonObj2->box.H + 18;

    GameObject* playObj = new GameObject();
    Button* playButton = new Button(*playButtonObj);
    playButtonObj->AddComponent(playButton);
    playText = new Text(*playObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Play", buttonNone, true);
    playObj->AddComponent(playText);
    playObj->box.Z = 1;
    playObj->box.X = playButtonObj->box.X + playButtonObj->box.W/2 - playObj->box.W/2;
    playObj->box.Y = playButtonObj->box.Y;
    AddObject(playObj);

    playButton->SetClickFunction([this]() {
        Game& game = Game::GetInstance();
        LevelSelectorState* stage = new LevelSelectorState();
        game.Push(stage);
        GameData::ended = false;
        //popRequested = true;
    });
    playButton->SetHoverFunction([this, playBgButton, buttonHover]() {
        playText->SetColor(buttonHover);
        playBgButton->SetFrame(1);
    });
    playButton->SetNoneFunction([this, buttonNone, playBgButton]() {
        playText->SetColor(buttonNone);
        playBgButton->SetFrame(0);
    });

    GameObject* creditosButtonObj = new GameObject();
    SpriteRenderer* creditosBgButton = new SpriteRenderer(*creditosButtonObj,  "../Recursos/img/MENU-BOTÃO-PREENCHIDO.png",2,1);
    creditosBgButton->SetCameraFollower(true);
    creditosButtonObj->AddComponent(creditosBgButton);
    AddObject(creditosButtonObj);
    creditosButtonObj->box.X = optionsButtonObj->box.X;
    creditosButtonObj->box.Y = optionsButtonObj->box.Y + creditosButtonObj->box.H + 40;

    GameObject* creditosButtonObj2 = new GameObject();
    SpriteRenderer* creditosBgButton2 = new SpriteRenderer(*creditosButtonObj2,  "../Recursos/img/MENU-BOTÃO-1.png");
    creditosBgButton2->SetCameraFollower(true);
    creditosButtonObj2->AddComponent(creditosBgButton2);
    AddObject(creditosButtonObj2);
    creditosButtonObj2->box.X = Game::VirtualScreenWidth/2 - creditosButtonObj2->box.W/2 - 7;
    creditosButtonObj2->box.Y = Game::VirtualScreenHeight/2 - creditosButtonObj2->box.H/2 + creditosButtonObj2->box.H - 12;

    GameObject* creeditosObj = new GameObject();
    Button* creditosButton = new Button(*creditosButtonObj);
    creditosButtonObj->AddComponent(creditosButton);
    creditosText = new Text(*creeditosObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Credits", buttonNone, true);
    creeditosObj->AddComponent(creditosText);
    creeditosObj->box.Z = 1;
    creeditosObj->box.X = creditosButtonObj->box.X + creditosButtonObj->box.W/2 - creeditosObj->box.W/2;
    creeditosObj->box.Y = creditosButtonObj->box.Y;
    AddObject(creeditosObj);
    creditosButton->SetClickFunction([this]() {

    });
    creditosButton->SetHoverFunction([this, buttonHover, creditosBgButton]() {
        creditosText->SetColor(buttonHover);
        creditosBgButton->SetFrame(1);
    });
    creditosButton->SetNoneFunction([this, buttonNone, creditosBgButton]() {
        creditosText->SetColor(buttonNone);
        creditosBgButton->SetFrame(0);
    });

    GameObject* quitButtonObj = new GameObject();
    SpriteRenderer* quitBgButton = new SpriteRenderer(*quitButtonObj,  "../Recursos/img/MENU-BOTÃO-PREENCHIDO.png",2,1);
    quitBgButton->SetCameraFollower(true);
    quitButtonObj->AddComponent(quitBgButton);
    AddObject(quitButtonObj);
    quitButtonObj->box.X = optionsButtonObj->box.X;
    quitButtonObj->box.Y = creditosButtonObj->box.Y + quitButtonObj->box.H + 40;

    GameObject* quitButtonObj2 = new GameObject();
    SpriteRenderer* quitBgButton2 = new SpriteRenderer(*quitButtonObj2,  "../Recursos/img/MENU-BOTÃO-1.png");
    quitBgButton2->SetCameraFollower(true);
    quitButtonObj2->AddComponent(quitBgButton2);
    AddObject(quitButtonObj2);
    quitButtonObj2->box.X = Game::VirtualScreenWidth/2 - quitButtonObj2->box.W/2 - 7;
    quitButtonObj2->box.Y = Game::VirtualScreenHeight/2 - quitButtonObj2->box.H/2 + 2*quitButtonObj2->box.H - 27;

    GameObject* quitObj = new GameObject();
    Button* quitButton = new Button(*quitButtonObj);
    quitButtonObj->AddComponent(quitButton);
    quitText = new Text(*quitObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Quit", buttonNone, true);
    quitObj->AddComponent(quitText);
    quitObj->box.Z = 1;
    quitObj->box.X = quitButtonObj->box.X + quitButtonObj->box.W/2 - quitObj->box.W/2;
    quitObj->box.Y = quitButtonObj->box.Y;
    AddObject(quitObj);
    quitButton->SetClickFunction([this]() {
        quit = true;
    });
    quitButton->SetHoverFunction([this, buttonHover, quitBgButton]() {
        quitText->SetColor(buttonHover);
        quitBgButton->SetFrame(1);
    });
    quitButton->SetNoneFunction([this, buttonNone, quitBgButton]() {
        quitText->SetColor(buttonNone);
        quitBgButton->SetFrame(0);
    });

}

void TitleState::Update(float dt) {
    UpdateArray(dt);

    InputManager& input = InputManager::GetInstance();
    if (input.KeyPress(ESCAPE_KEY) or quit) {
        quitRequested = true;
    } else {
        quitRequested = input.QuitRequested();
    }
}

void TitleState::Render() {
    RenderArray();
}

void TitleState::Start() {
    StartArray();
    backgroundMusic.UpdateVolume();
    backgroundMusic.Play();
}
void TitleState::Pause() {
    if (dynamic_cast<LevelSelectorState*>(Game::GetInstance().GetStoredState()) == nullptr) {
        backgroundMusic.Pause();
    }
}
void TitleState::Resume() {
    backgroundMusic.UpdateVolume();
    backgroundMusic.Resume();
}
void TitleState::LoadAssets() {}
TitleState::~TitleState() = default;

