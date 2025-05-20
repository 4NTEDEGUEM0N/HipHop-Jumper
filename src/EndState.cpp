#include "../include/EndState.hpp"
#include "../include/GameData.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Text.hpp"
#include "../include/InputManager.hpp"
#include "../include/Game.hpp"
#include "../include/TitleState.hpp"
#include "../include/Camera.hpp"

EndState::EndState() {
    backgroudImage = new GameObject();
    State::AddObject(backgroudImage);
    if (GameData::playerVictory) {
        SpriteRenderer* titleBg = new SpriteRenderer(*backgroudImage, "../Recursos/img/Win.png");
        //titleBg->SetCameraFollower(true);
        backgroudImage->AddComponent(titleBg);
        backgroundMusic.Open("../Recursos/audio/endStateWin.ogg");
        float scaleX = Game::VirtualScreenWidth  / 1200.0f;
        float scaleY = Game::VirtualScreenHeight / 900.0f;
        titleBg->SetScale(scaleX, scaleY);

        backgroundMusic.Play();
    } else {
        SpriteRenderer* titleBg = new SpriteRenderer(*backgroudImage, "../Recursos/img/Lose.png");
        //titleBg->SetCameraFollower(true);
        backgroudImage->AddComponent(titleBg);
        float scaleX = Game::VirtualScreenWidth  / 1200.0f;
        float scaleY = Game::VirtualScreenHeight / 900.0f;
        titleBg->SetScale(scaleX, scaleY);

        backgroundMusic.Open("../Recursos/audio/endStateLose.ogg");
    }

    GameObject* playInstructionsObject = new GameObject();
    this->playInstructionsObject = AddObject(playInstructionsObject);
    playInstructionsObject->box.X = 400;
    playInstructionsObject->box.Y = 420;
    Text* playInstructionsText = new Text(*playInstructionsObject, "../Recursos/font/neodgm.ttf", 30, Text::SHADED, "PRESS THE SPACE BAR TO RESTART", {255, 255, 255, 255});
    playInstructionsObject->AddComponent(playInstructionsText);

    GameObject* quitInstructionsObject = new GameObject();
    this->quitInstructionsObject = AddObject(quitInstructionsObject);
    quitInstructionsObject->box.X = 475;
    quitInstructionsObject->box.Y = 470;
    Text* quitInstructionsText = new Text(*quitInstructionsObject, "../Recursos/font/neodgm.ttf", 30, Text::SHADED, "PRESS ESC TO QUIT", {255, 255, 255, 255});
    quitInstructionsObject->AddComponent(quitInstructionsText);

    up = false;
    counter = 0;
}

void EndState::Update(float dt) {
    UpdateArray(dt);

    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY)) {
        quitRequested = true;
    } else {
        quitRequested = InputManager::GetInstance().QuitRequested();
    }

    if (InputManager::GetInstance().KeyPress(SDLK_SPACE)) {
        Game& game = Game::GetInstance();
        TitleState* titleState = new TitleState();
        game.Push(titleState);
        popRequested = true;
    }

    counter++;
    if (counter == 50) {
        counter = 1;
        up = !up;
    }
    Text* quitTextCpt = dynamic_cast<Text*>(quitInstructionsObject.lock()->GetComponent("Text"));
    Text* playTextCpt = dynamic_cast<Text*>(playInstructionsObject.lock()->GetComponent("Text"));
    if (up) {
        Uint8 alpha = counter*5;
        quitTextCpt->SetColor({255, 255, 255, alpha});
        playTextCpt->SetColor({255, 255, 255, alpha});
    } else {
        Uint8 alpha = 255 - counter*5;
        quitTextCpt->SetColor({255, 255, 255, alpha});
        playTextCpt->SetColor({255, 255, 255, alpha});
    }

    Camera::Update(dt);
}

void EndState::Render() {
    RenderArray();
}
void EndState::LoadAssets() {}
void EndState::Start() {
    Camera::Follow(this->backgroudImage);
    backgroundMusic.Play();
    StartArray();

}
void EndState::Pause() {}
void EndState::Resume() {}
EndState::~EndState() = default;
