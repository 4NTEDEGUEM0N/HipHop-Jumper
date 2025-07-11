#include "../include/PauseState.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Game.hpp"
#include "../include/InputManager.hpp"
#include "../include/Button.hpp"
#include "../include/OptionsState.hpp"
#include "../include/Text.hpp"

PauseState::PauseState() {
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

    GameObject* optionsObj = new GameObject();
    Button* optionsButton = new Button(*optionsObj);
    optionsObj->AddComponent(optionsButton);
    optionsText = new Text(*optionsObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Options", {255, 255, 255, 255}, true);
    optionsObj->AddComponent(optionsText);
    optionsObj->box.X = bgObject->box.X + bgObject->box.W/2 - optionsObj->box.W/2;
    optionsObj->box.Y = bgObject->box.Y + bgObject->box.H/2 - optionsObj->box.H/2;
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

    GameObject* resumeObj = new GameObject();
    Button* resumeButton = new Button(*resumeObj);
    resumeObj->AddComponent(resumeButton);
    resumeText = new Text(*resumeObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Resume", {255, 255, 255, 255}, true);
    resumeObj->AddComponent(resumeText);
    resumeObj->box.X = optionsObj->box.X;
    resumeObj->box.Y = optionsObj->box.Y - resumeObj->box.H - 10;
    AddObject(resumeObj);
        
    resumeButton->SetClickFunction([this]() {
        popRequested = true;
    });
    resumeButton->SetHoverFunction([this]() {
        resumeText->SetColor({26, 160, 251, 255});
    });
    resumeButton->SetNoneFunction([this]() {
        resumeText->SetColor({255, 255, 255, 255});
    });

    GameObject* menuObj = new GameObject();
    Button* menuButton = new Button(*menuObj);
    menuObj->AddComponent(menuButton);
    menuText = new Text(*menuObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Menu", {255, 255, 255, 255}, true);
    menuObj->AddComponent(menuText);
    menuObj->box.X = optionsObj->box.X;
    menuObj->box.Y = optionsObj->box.Y + menuObj->box.H + 10;
    AddObject(menuObj);
    menuButton->SetClickFunction([this]() {
        quitRequested = true;
    });
    menuButton->SetHoverFunction([this]() {
        menuText->SetColor({26, 160, 251, 255});
    });
    menuButton->SetNoneFunction([this]() {
        menuText->SetColor({255, 255, 255, 255});
    });

}

void PauseState::Update(float dt) {
    UpdateArray(dt);

    InputManager& input = InputManager::GetInstance();
    if (input.KeyPress(SDLK_ESCAPE)) {
//        Sound escSound = Sound("../Recursos/audio/UX/MENU CLICK 3.wav");
//        escSound.Play();
        popRequested = true;
    }
    //quitRequested = input.QuitRequested();
}

void PauseState::Render() {
    RenderArray();
}

void PauseState::Start() {
    StartArray();
}
void PauseState::Pause() {}
void PauseState::Resume() {}
void PauseState::LoadAssets() {}
PauseState::~PauseState() = default;

