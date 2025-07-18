#include "../include/EndState.hpp"
#include "../include/GameData.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Text.hpp"
#include "../include/InputManager.hpp"
#include "../include/Game.hpp"
#include "../include/TitleState.hpp"
#include "../include/Camera.hpp"
#include "../include/Button.hpp"
#include <sstream>
#include <iomanip>

#include "../include/LevelSelectorState.hpp"

EndState::EndState() {
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

    GameObject* completedObj = new GameObject();
    Text* completedText = new Text(*completedObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Level", {0, 255, 0, 255}, true);
    completedObj->AddComponent(completedText);
    AddObject(completedObj);
    completedObj->box.X = Game::VirtualScreenWidth/2 - completedObj->box.W/2;
    completedObj->box.Y = 20;

    GameObject* nextObj = new GameObject();
    Button* nextButton = new Button(*nextObj);
    nextObj->AddComponent(nextButton);
    nextText = new Text(*nextObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Next Level", {255, 0, 0, 255}, true);
    nextObj->AddComponent(nextText);
    nextObj->box.X = Game::VirtualScreenWidth/2 - nextObj->box.W/2;
    nextObj->box.Y = bgObject->box.Y + bgObject->box.H/2 - nextObj->box.H/2 + 200;

    nextButton->SetClickFunction([this]() {
        LevelSelectorState::NextLevel();
    });
    nextButton->SetHoverFunction([this]() {
        nextText->SetColor({26, 160, 251, 255});
    });
    nextButton->SetNoneFunction([this]() {
        nextText->SetColor({255, 255, 255, 255});
    });

    GameObject* menuObj = new GameObject();
    Button* menuButton = new Button(*menuObj);
    menuObj->AddComponent(menuButton);
    menuText = new Text(*menuObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Menu", {255, 255, 255, 255}, true);
    menuObj->AddComponent(menuText);
    menuObj->box.X = Game::VirtualScreenWidth/2 - menuObj->box.W/2;
    menuObj->box.Y = nextObj->box.Y + menuObj->box.H + 10;
    AddObject(menuObj);
    menuButton->SetClickFunction([this]() {
        Game& game = Game::GetInstance();
        TitleState* tileState = new TitleState();
        game.Push(tileState);
        game.SuperPop();
    });
    menuButton->SetHoverFunction([this]() {
        menuText->SetColor({26, 160, 251, 255});
    });
    menuButton->SetNoneFunction([this]() {
        menuText->SetColor({255, 255, 255, 255});
    });

    if (GameData::playerVictory) {
        //backgroundMusic.Open("../Recursos/audio/endStateWin.ogg");
        backgroundMusic.Open("../Recursos/audio/TRACKS/MAIN MENU TRACK LOOP.wav");
        backgroundMusic.SetIntro("../Recursos/audio/TRACKS/MAIN MENU TRACK START.wav");
        completedText->SetText("Level Completed!");
        completedText->SetColor({0, 255, 0, 255});
        completedObj->box.X = Game::VirtualScreenWidth/2 - completedObj->box.W/2;
        completedObj->box.Y = 20;
        AddObject(nextObj);

        GameObject* minikitTextdObj = new GameObject();
        Text* minikitText = new Text(*minikitTextdObj, "../Recursos/font/KGPerfectPenmanship.ttf", 50, Text::BLENDED, "Minikits: " + to_string(GameData::minikits) + "/6", {255, 255, 255, 255}, true);
        minikitTextdObj->AddComponent(minikitText);
        AddObject(minikitTextdObj);
        minikitTextdObj->box.X = Game::VirtualScreenWidth/2 - minikitTextdObj->box.W/2;
        minikitTextdObj->box.Y = (nextObj->box.Y - (completedObj->box.Y + completedObj->box.H))/2 + completedObj->box.Y + completedObj->box.H - minikitTextdObj->box.H/2;

        GameObject* minikitObj = new GameObject();
        AddObject(minikitObj);
        SpriteRenderer* minikit = new SpriteRenderer(*minikitObj, "../Recursos/img/minikitHud.png", 2, 1);
        minikit->SetCameraFollower(true);
        minikit->SetFrame(1);
        minikitObj->AddComponent(minikit);
        minikitObj->box.X = minikitTextdObj->box.X - minikitObj->box.W - 10;
        minikitObj->box.Y = minikitTextdObj->box.Y;

        GameObject* canTextObj = new GameObject();
        Text* canText = new Text(*canTextObj, "../Recursos/font/KGPerfectPenmanship.ttf", 50, Text::BLENDED, "Spray Cans: " + to_string(GameData::sprayCans) + "/3", {255, 255, 255, 255}, true);
        canTextObj->AddComponent(canText);
        AddObject(canTextObj);
        canTextObj->box.X = Game::VirtualScreenWidth/2 - canTextObj->box.W/2;
        canTextObj->box.Y = minikitTextdObj->box.Y + canTextObj->box.H + 10;

        GameObject* canObj = new GameObject();
        AddObject(canObj);
        SpriteRenderer* can = new SpriteRenderer(*canObj, "../Recursos/img/lata-cinza.png");
        can->SetCameraFollower(true);
        canObj->AddComponent(can);
        canObj->box.X = canTextObj->box.X - canObj->box.W - 10;
        canObj->box.Y = canTextObj->box.Y;

        ostringstream timer;
        timer << fixed << setprecision(4) << GameData::endTime;
        GameObject* timerTextObj = new GameObject();
        Text* timerText = new Text(*timerTextObj, "../Recursos/font/KGPerfectPenmanship.ttf", 50, Text::BLENDED, "Level Timer: " + timer.str() + "s", {255, 255, 255, 255}, true);
        timerTextObj->AddComponent(timerText);
        AddObject(timerTextObj);
        timerTextObj->box.X = Game::VirtualScreenWidth/2 - timerTextObj->box.W/2;
        timerTextObj->box.Y = minikitTextdObj->box.Y - timerTextObj->box.H - 10;

        GameObject* timerObj = new GameObject();
        AddObject(timerObj);
        SpriteRenderer* chronometer = new SpriteRenderer(*timerObj, "../Recursos/img/cronometro.png");
        chronometer->SetCameraFollower(true);
        timerObj->AddComponent(chronometer);
        timerObj->box.X = timerTextObj->box.X - timerObj->box.W - 10;
        timerObj->box.Y = timerTextObj->box.Y;

    } else {
        backgroundMusic.Open("../Recursos/audio/endStateLose.ogg");
        completedText->SetText("Level Failed!");
        completedText->SetColor({255, 0, 0, 255});
        completedObj->box.X = Game::VirtualScreenWidth/2 - completedObj->box.W/2;
        completedObj->box.Y = 20;
        menuObj->box.Y = Game::VirtualScreenHeight/2 - menuObj->box.H/2;
    }
}

void EndState::Update(float dt) {
    UpdateArray(dt);

    InputManager& input = InputManager::GetInstance();
    quitRequested = input.QuitRequested();

}

void EndState::Render() {
    RenderArray();
}
void EndState::LoadAssets() {}
void EndState::Start() {
    backgroundMusic.Play();
    StartArray();

}
void EndState::Pause() {}
void EndState::Resume() {}
EndState::~EndState() = default;
