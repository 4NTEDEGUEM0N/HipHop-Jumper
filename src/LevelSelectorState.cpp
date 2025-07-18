#include "../include/LevelSelectorState.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Game.hpp"
#include "../include/InputManager.hpp"
#include "../include/Button.hpp"
#include "../include/StageState.hpp"
#include "../include/StageTutorialState.hpp"
#include "../include/Text.hpp"
#include "../include/TitleState.hpp"

int LevelSelectorState::currentLevel = 0;

LevelSelectorState::LevelSelectorState() {
    GameObject* bgObject = new GameObject();
    AddObject(bgObject);
    SpriteRenderer* bg = new SpriteRenderer(*bgObject,  "../Recursos/img/BG Caderno.png");
    bg->SetCameraFollower(true);
    bgObject->AddComponent(bg);
    float scaleX = Game::VirtualScreenWidth  / (bgObject->box.W);
    float scaleY = Game::VirtualScreenHeight / (bgObject->box.H);
    bg->SetScale(scaleX, scaleY);
    bgObject->box.X = 0;
    bgObject->box.Y = 0;

    GameObject* level1Obj = new GameObject();
    Button* level1Button = new Button(*level1Obj);
    level1Obj->AddComponent(level1Button);
    level1Text = new Text(*level1Obj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Level 1", {255, 255, 255, 255}, true);
    level1Obj->AddComponent(level1Text);
    level1Obj->box.X = Game::VirtualScreenWidth/2 - level1Obj->box.W/2;
    level1Obj->box.Y = bgObject->box.Y + bgObject->box.H/2 - level1Obj->box.H/2;
    AddObject(level1Obj);
    level1Button->SetClickFunction([this]() {
        currentLevel = 1;
        LoadLevel();
    });
    level1Button->SetHoverFunction([this]() {
        level1Text->SetColor({26, 160, 251, 255});
    });
    level1Button->SetNoneFunction([this]() {
        level1Text->SetColor({255, 255, 255, 255});
    });

    GameObject* tutorialObj = new GameObject();
    Button* tutorialButton = new Button(*tutorialObj);
    tutorialObj->AddComponent(tutorialButton);
    tutorialText = new Text(*tutorialObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Tutorial", {255, 255, 255, 255}, true);
    tutorialObj->AddComponent(tutorialText);
    tutorialObj->box.X = Game::VirtualScreenWidth/2 - tutorialObj->box.W/2;
    tutorialObj->box.Y = level1Obj->box.Y - tutorialObj->box.H - 10;
    AddObject(tutorialObj);

    tutorialButton->SetClickFunction([this]() {
        currentLevel = 0;
        LoadLevel();
    });
    tutorialButton->SetHoverFunction([this]() {
        tutorialText->SetColor({26, 160, 251, 255});
    });
    tutorialButton->SetNoneFunction([this]() {
        tutorialText->SetColor({255, 255, 255, 255});
    });

    GameObject* level2Obj = new GameObject();
    Button* level2Button = new Button(*level2Obj);
    level2Obj->AddComponent(level2Button);
    level2Text = new Text(*level2Obj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Level 2", {255, 255, 255, 255}, true);
    level2Obj->AddComponent(level2Text);
    level2Obj->box.X = Game::VirtualScreenWidth/2 - level2Obj->box.W/2;
    level2Obj->box.Y = level1Obj->box.Y + level2Obj->box.H + 10;
    AddObject(level2Obj);
    level2Button->SetClickFunction([this]() {
        currentLevel = 2;
        LoadLevel();
    });
    level2Button->SetHoverFunction([this]() {
        level2Text->SetColor({26, 160, 251, 255});
    });
    level2Button->SetNoneFunction([this]() {
        level2Text->SetColor({255, 255, 255, 255});
    });

    GameObject* level3Obj = new GameObject();
    Button* level3Button = new Button(*level3Obj);
    level3Obj->AddComponent(level3Button);
    level3Text = new Text(*level3Obj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Level 3", {255, 255, 255, 255}, true);
    level3Obj->AddComponent(level3Text);
    level3Obj->box.X = Game::VirtualScreenWidth/2 - level3Obj->box.W/2;
    level3Obj->box.Y = level2Obj->box.Y + level3Obj->box.H + 10;
    AddObject(level3Obj);
    level3Button->SetClickFunction([this]() {
        currentLevel = 3;
        LoadLevel();
    });
    level3Button->SetHoverFunction([this]() {
        level3Text->SetColor({26, 160, 251, 255});
    });
    level3Button->SetNoneFunction([this]() {
        level3Text->SetColor({255, 255, 255, 255});
    });

}

void LevelSelectorState::Update(float dt) {
    UpdateArray(dt);

    InputManager& input = InputManager::GetInstance();
    if (input.KeyPress(SDLK_ESCAPE)) {
//        Sound escSound = Sound("../Recursos/audio/UX/MENU CLICK 3.wav");
//        escSound.Play();
        popRequested = true;
    }
    quitRequested = input.QuitRequested();
}

void LevelSelectorState::Render() {
    RenderArray();
}

void LevelSelectorState::Start() {
    StartArray();
}
void LevelSelectorState::Pause() {}
void LevelSelectorState::Resume() {}
void LevelSelectorState::LoadAssets() {}
LevelSelectorState::~LevelSelectorState() = default;

void LevelSelectorState::LoadLevel() {
    Game& game = Game::GetInstance();
    if (currentLevel == 0) {
        StageTutorialState* tutorial = new StageTutorialState();
        game.Push(tutorial);
        game.SuperPop();
    } else if (currentLevel == 1) {
        StageState* stage1 = new StageState();
        game.Push(stage1);
        game.SuperPop();
    } else if (currentLevel == 2) {
        StageState* stage2 = new StageState();
        game.Push(stage2);
        game.SuperPop();
    } else if (currentLevel == 3) {
        StageState* stage3 = new StageState();
        game.Push(stage3);
        game.SuperPop();
    } else {
        TitleState* tileState = new TitleState();
        game.Push(tileState);
        game.SuperPop();
    }
}

