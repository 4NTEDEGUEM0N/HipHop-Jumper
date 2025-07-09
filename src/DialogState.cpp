#include "../include/DialogState.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/InputManager.hpp"

DialogState::DialogState(vector<string> dialogs, string npcSprite, string npcName) {
    this->dialogs = dialogs;
    this->npcSprite = npcSprite;
    this->npcName = npcName;
    currrentDialog = 0;

    GameObject* dialogBox = new GameObject();
    SpriteRenderer* dialogSprite = new SpriteRenderer(*dialogBox, "../Recursos/img/pixel.png");
    dialogSprite->SetCameraFollower(true);
    dialogBox->AddComponent(dialogSprite);
    AddObject(dialogBox);
    dialogBox->box.X = 0;
    dialogBox->box.Y = 0;
    dialogBox->box.W = 1280;
    dialogBox->box.H = 180;

    GameObject* npcObj = new GameObject();
    SpriteRenderer* npcSpriteRenderer = new SpriteRenderer(*npcObj, npcSprite);
    npcSpriteRenderer->SetCameraFollower(true);
    npcObj->AddComponent(npcSpriteRenderer);
    AddObject(npcObj);
    npcObj->box.X = 0;
    npcObj->box.Y = npcObj->box.H/2;

    GameObject* nameTextObj = new GameObject();
    Text* nameText = new Text(*nameTextObj, "../Recursos/font/KGPerfectPenmanship.ttf", 20, Text::SOLID, npcName, {255, 255, 255, 255}, true);
    nameTextObj->AddComponent(nameText);
    AddObject(nameTextObj);
    nameTextObj->box.X = 5;
    nameTextObj->box.Y = 5;

    GameObject* dialogTextObj = new GameObject();
    dialogText = new Text(*dialogTextObj, "../Recursos/font/neodgm.ttf", 30, Text::SOLID, "", {255, 255, 255, 255}, true);
    dialogTextObj->AddComponent(dialogText);
    AddObject(dialogTextObj);
    dialogTextObj->box.X = npcObj->box.W + 10;
    dialogTextObj->box.Y = dialogBox->box.H/2 - dialogTextObj->box.H/2;

    GameObject* nextTextObj = new GameObject();
    Text* nextText = new Text(*nextTextObj, "../Recursos/font/KGPerfectPenmanship.ttf", 20, Text::SOLID, "Press ENTER -> ", {255, 255, 255, 255}, true);
    nextTextObj->AddComponent(nextText);
    AddObject(nextTextObj);
    nextTextObj->box.X = dialogBox->box.W - nextTextObj->box.W;
    nextTextObj->box.Y = dialogBox->box.H - nextTextObj->box.H;
}

DialogState::~DialogState() = default;

void DialogState::Start() {
    StartArray();
}

void DialogState::Update(float dt) {
    UpdateArray(dt);

    InputManager& input = InputManager::GetInstance();
    if (input.KeyPress(SDLK_ESCAPE)) {
        popRequested = true;
    }
    quitRequested = input.QuitRequested();


    dialogText->SetText(dialogs[currrentDialog]);
    if (input.KeyPress(SDLK_RETURN)) {
        currrentDialog++;
        if (currrentDialog >= dialogs.size()) {
            popRequested = true;
        }
    }
}

void DialogState::Render() {
    RenderArray();
}


void DialogState::Pause() {

}

void DialogState::Resume() {

}

void DialogState::LoadAssets() {

}



