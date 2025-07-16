#include "../include/TitleState.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Game.hpp"
#include "../include/InputManager.hpp"
#include "../include/Button.hpp"
#include "../include/OptionsState.hpp"
#include "../include/Text.hpp"
#include "../include/StageState.hpp"
#include "../include/GameData.hpp"

TitleState::TitleState() {
    GameObject* bgObject = new GameObject();
    AddObject(bgObject);
    SpriteRenderer* bg = new SpriteRenderer(*bgObject,  "../Recursos/img/pixel.png");
    bg->SetCameraFollower(true);
    bgObject->AddComponent(bg);
    float scaleX = Game::VirtualScreenWidth  / (bgObject->box.W);
    float scaleY = Game::VirtualScreenHeight / (bgObject->box.H);
    bg->SetScale(scaleX, scaleY);
    bgObject->box.X = 0;
    bgObject->box.Y = 0;

    backgroundMusic.Open("../Recursos/audio/TRACKS/MAIN MENU TRACK LOOP.wav");
    backgroundMusic.SetIntro("../Recursos/audio/TRACKS/MAIN MENU TRACK START.wav");

    GameObject* logoObj = new GameObject();
    AddObject(logoObj);
    SpriteRenderer* logo = new SpriteRenderer(*logoObj, "../Recursos/img/hiphopjumper.png");
    logo->SetCameraFollower(true);
    logoObj->AddComponent(logo);
    //logo->SetScale(0.5f, 0.5f);
    logoObj->box.X = Game::VirtualScreenWidth/2 - logoObj->box.W/2;
    logoObj->box.Y = 20;

    GameObject* optionsObj = new GameObject();
    Button* optionsButton = new Button(*optionsObj);
    optionsObj->AddComponent(optionsButton);
    optionsText = new Text(*optionsObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Options", {255, 255, 255, 255}, true);
    optionsObj->AddComponent(optionsText);
    optionsObj->box.X = Game::VirtualScreenWidth/2 - optionsObj->box.W/2;
    optionsObj->box.Y = bgObject->box.Y + bgObject->box.H/2 - optionsObj->box.H/2 + 50;
    AddObject(optionsObj);
    optionsButton->SetClickFunction([this]() {
        Game& game = Game::GetInstance();
        OptionsState* optionsState = new OptionsState();
        game.Push(optionsState);
    });
    optionsButton->SetHoverFunction([this]() {
        optionsText->SetColor({26, 160, 251, 255});
    });
    optionsButton->SetNoneFunction([this]() {
        optionsText->SetColor({255, 255, 255, 255});
    });

    GameObject* playObj = new GameObject();
    Button* playButton = new Button(*playObj);
    playObj->AddComponent(playButton);
    playText = new Text(*playObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Play", {255, 255, 255, 255}, true);
    playObj->AddComponent(playText);
    playObj->box.X = Game::VirtualScreenWidth/2 - playObj->box.W/2;
    playObj->box.Y = optionsObj->box.Y - playObj->box.H - 10;
    AddObject(playObj);

    playButton->SetClickFunction([this]() {
        Game& game = Game::GetInstance();
        StageState* stage = new StageState();
        game.Push(stage);
        GameData::ended = false;
        popRequested = true;
    });
    playButton->SetHoverFunction([this]() {
        playText->SetColor({26, 160, 251, 255});
    });
    playButton->SetNoneFunction([this]() {
        playText->SetColor({255, 255, 255, 255});
    });

    GameObject* quitObj = new GameObject();
    Button* quitButton = new Button(*quitObj);
    quitObj->AddComponent(quitButton);
    quitText = new Text(*quitObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Quit", {255, 255, 255, 255}, true);
    quitObj->AddComponent(quitText);
    quitObj->box.X = Game::VirtualScreenWidth/2 - quitObj->box.W/2;
    quitObj->box.Y = optionsObj->box.Y + quitObj->box.H + 10;
    AddObject(quitObj);
    quitButton->SetClickFunction([this]() {
        quit = true;
    });
    quitButton->SetHoverFunction([this]() {
        quitText->SetColor({26, 160, 251, 255});
    });
    quitButton->SetNoneFunction([this]() {
        quitText->SetColor({255, 255, 255, 255});
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
    backgroundMusic.Pause();
}
void TitleState::Resume() {
    backgroundMusic.UpdateVolume();
    backgroundMusic.Resume();
}
void TitleState::LoadAssets() {}
TitleState::~TitleState() = default;

