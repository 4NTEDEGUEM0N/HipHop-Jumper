#include "../include/TitleState.hpp"

#include <iostream>

#include "../include/InputManager.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/StageState.hpp"
#include "../include/Game.hpp"
#include "../include/Camera.hpp"
#include "../include/GameData.hpp"
#include "../include/Text.hpp"

TitleState::TitleState() {
    backgroudImage = new GameObject();
    AddObject(backgroudImage);
    SpriteRenderer* titleBg = new SpriteRenderer(*backgroudImage, "../Recursos/img/Title.png");
    backgroudImage->AddComponent(titleBg);

    float scaleX = Game::VirtualScreenWidth  / 1200.0f;
    float scaleY = Game::VirtualScreenHeight / 900.0f;
    titleBg->SetScale(scaleX, scaleY);

    GameObject* instructionsObject = new GameObject();
    this->instructionsObject = AddObject(instructionsObject);
    instructionsObject->box.X = 400;
    instructionsObject->box.Y = 450;
    Text* instructionsText = new Text(*instructionsObject, "../Recursos/font/neodgm.ttf", 30, Text::SHADED, "PRESS THE SPACE BAR TO START", {255, 255, 255, 255});
    instructionsObject->AddComponent(instructionsText);
    up = false;
    counter = 0;
}

void TitleState::Update(float dt) {
    UpdateArray(dt);

    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY)) {
        quitRequested = true;
    } else {
        quitRequested = InputManager::GetInstance().QuitRequested();
    }

    if (InputManager::GetInstance().KeyPress(SDLK_SPACE)) {
        Game& game = Game::GetInstance();
        StageState* stage = new StageState();
        game.Push(stage);
        GameData::ended = false;
        popRequested = true;
    }

    counter++;
    if (counter == 50) {
        counter = 1;
        up = !up;
    }
    Text* textCpt = dynamic_cast<Text*>(instructionsObject.lock()->GetComponent("Text"));
    if (up) {
        Uint8 alpha = counter*5;
        textCpt->SetColor({255, 255, 255, alpha});
    } else {
        Uint8 alpha = 255 - counter*5;
        textCpt->SetColor({255, 255, 255, alpha});
    }


    Camera::Update(dt);
}

void TitleState::Render() {
    RenderArray();
}
void TitleState::LoadAssets() {}
void TitleState::Start() {
    Camera::Follow(this->backgroudImage);
    StartArray();
}
void TitleState::Pause() {}
void TitleState::Resume() {}
TitleState::~TitleState() = default;


