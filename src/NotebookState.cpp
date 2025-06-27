#include "../include/NotebookState.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Button.hpp"
#include "../include/InputManager.hpp"
#include "../include/DrawState.hpp"

#include <iostream>

#include "../include/Character.hpp"

NotebookState::NotebookState() {
    currentPage = 1;

    GameObject* bgObject = new GameObject();
    AddObject(bgObject);
    SpriteRenderer* bg = new SpriteRenderer(*bgObject,  "../Recursos/img/madeira.png");
    bg->SetCameraFollower(true);
    bgObject->AddComponent(bg);
    float scaleX = Game::VirtualScreenWidth  / 1200.0f;
    float scaleY = Game::VirtualScreenHeight / 900.0f;
    bg->SetScale(scaleX, scaleY);
    bgObject->box.X = 0;
    bgObject->box.Y = 0;

    cadernoObj = new GameObject();
    AddObject(cadernoObj);
    SpriteRenderer* caderno = new SpriteRenderer(*cadernoObj, "../Recursos/img/Caderno.png");
    caderno->SetCameraFollower(true);
    caderno->SetScale(2,2);
    Vec2 screenCenter = Vec2(Game::VirtualScreenWidth/2.0f, Game::VirtualScreenHeight/2.0f);
    cadernoObj->box.X = screenCenter.GetX() - cadernoObj->box.W / 2;
    cadernoObj->box.Y = screenCenter.GetY() - cadernoObj->box.H / 2;
    cadernoObj->AddComponent(caderno);

    GameObject* currentPageObj = new GameObject();
    SpriteRenderer* currentPageButtonSprite = new SpriteRenderer(*currentPageObj, "../Recursos/img/pixel.png");
    currentPageButtonSprite->SetCameraFollower(true);
    currentPageObj->AddComponent(currentPageButtonSprite);
    currentPageText = new Text(*currentPageObj, "../Recursos/font/neodgm.ttf", 30, Text::SOLID, "Pagina " + to_string(currentPage), {255, 255, 255, 255}, true);
    currentPageObj->AddComponent(currentPageText);
    currentPageObj->box.X = cadernoObj->box.X + cadernoObj->box.W/2 - currentPageObj->box.W/2;
    currentPageObj->box.Y = cadernoObj->box.Y - currentPageObj->box.H;
    AddObject(currentPageObj);

    GameObject* previousPageObj = new GameObject();
    Button* previousPageButton = new Button(*previousPageObj);
    previousPageObj->AddComponent(previousPageButton);
    SpriteRenderer* previousPageButtonSprite = new SpriteRenderer(*previousPageObj, "../Recursos/img/pixel.png");
    previousPageButtonSprite->SetCameraFollower(true);
    previousPageObj->AddComponent(previousPageButtonSprite);
    Text* previousPageText = new Text(*previousPageObj, "../Recursos/font/neodgm.ttf", 30, Text::SOLID, "<<", {255, 255, 255, 255}, true);
    previousPageObj->AddComponent(previousPageText);
    previousPageObj->box.X = currentPageObj->box.X - previousPageObj->box.W - 5;
    previousPageObj->box.Y = currentPageObj->box.Y;
    AddObject(previousPageObj);

    previousPageButton->SetClickFunction([this]() {
        if (currentPage > 1) {
            currentPage--;
            currentPageText->SetText("Pagina " + to_string(currentPage));
            UpdatePageContent();
        }
    });

    GameObject* nextPageObj = new GameObject();
    Button* nextPageButton = new Button(*nextPageObj);
    nextPageObj->AddComponent(nextPageButton);
    SpriteRenderer* nextPageButtonSprite = new SpriteRenderer(*nextPageObj, "../Recursos/img/pixel.png");
    nextPageButtonSprite->SetCameraFollower(true);
    nextPageObj->AddComponent(nextPageButtonSprite);
    Text* nextPageText = new Text(*nextPageObj, "../Recursos/font/neodgm.ttf", 30, Text::SOLID, ">>", {255, 255, 255, 255}, true);
    nextPageObj->AddComponent(nextPageText);
    nextPageObj->box.X = currentPageObj->box.X + currentPageObj->box.W + 5;
    nextPageObj->box.Y = currentPageObj->box.Y;
    AddObject(nextPageObj);

    nextPageButton->SetClickFunction([this]() {
        if (currentPage < 9) {
            currentPage++;
            currentPageText->SetText("Pagina " + to_string(currentPage));
            UpdatePageContent();
        }
    });

    GameObject* drawObj = new GameObject();
    Button* drawButton = new Button(*drawObj);
    drawObj->AddComponent(drawButton);
    SpriteRenderer* drawButtonSprite = new SpriteRenderer(*drawObj, "../Recursos/img/pixel.png");
    drawButtonSprite->SetCameraFollower(true);
    drawObj->AddComponent(drawButtonSprite);
    Text* drawText = new Text(*drawObj, "../Recursos/font/neodgm.ttf", 50, Text::SOLID, "Desenhar", {255, 255, 255, 255}, true);
    drawObj->AddComponent(drawText);
    drawObj->box.X = cadernoObj->box.X + cadernoObj->box.W/2 - drawObj->box.W/2;
    drawObj->box.Y = cadernoObj->box.Y + cadernoObj->box.H;
    AddObject(drawObj);
    drawButton->SetClickFunction([]() {
        Game& game = Game::GetInstance();
        DrawState* cadernoState = new DrawState();
        game.Push(cadernoState);
    });

}

void NotebookState::Update(float dt) {
    UpdateArray(dt);

    if (contentNeedsUpdate) {
        UpdatePageContent();
        contentNeedsUpdate = false;
    }

    if (InputManager::GetInstance().KeyPress(SDLK_ESCAPE)) {
        popRequested = true;
    }
}

void NotebookState::Render() {
    RenderArray();
}

void NotebookState::LoadAssets() {}
void NotebookState::Start() {
    StartArray();
    UpdatePageContent();
}
void NotebookState::Pause() {}
void NotebookState::Resume() {
    UpdatePageContent();
}

NotebookState::~NotebookState() {
    for (auto& item : pageItems) {
        item->RequestDelete();
    }
    pageItems.clear();
}

void NotebookState::UpdatePageContent() {
    for (auto& item : pageItems) {
        item->RequestDelete();
    }
    pageItems.clear();

    if (Character::graffitiArray.empty()) return;

    int startIndex = (currentPage - 1) * 4;
    int endIndex = startIndex + 4;

    for (int i = startIndex; i < Character::graffitiArray.size() && i < endIndex; i++) {
        SDL_Texture* texture = Character::graffitiArray[i];

        GameObject* graffitiObj = new GameObject();
        SpriteRenderer* graffiti = new SpriteRenderer(*graffitiObj, texture, to_string(i));
        graffiti->SetCameraFollower(true);
        graffiti->SetScale(0.4f, 0.4f);
        graffitiObj->AddComponent(graffiti);

        int itemIndexOnPage = i - startIndex;

        switch (itemIndexOnPage) {
            case 0: // Top Left
                graffitiObj->box.X = cadernoObj->box.X + 30;
            graffitiObj->box.Y = cadernoObj->box.Y + 20;
            break;
            case 1: // Bottom Left
                graffitiObj->box.X = cadernoObj->box.X + 30;
            graffitiObj->box.Y = cadernoObj->box.Y + cadernoObj->box.H - graffitiObj->box.H - 20;
            break;
            case 2: // Top Right
                graffitiObj->box.X = cadernoObj->box.X + cadernoObj->box.W - graffitiObj->box.W - 30;
            graffitiObj->box.Y = cadernoObj->box.Y + 20;
            break;
            case 3: // Bottom Right
                graffitiObj->box.X = cadernoObj->box.X + cadernoObj->box.W - graffitiObj->box.W - 30;
            graffitiObj->box.Y = cadernoObj->box.Y + cadernoObj->box.H - graffitiObj->box.H - 20;
            break;
        }

        AddObject(graffitiObj);
        pageItems.push_back(graffitiObj);

        GameObject* selectBtnObj = new GameObject();
        Button* selectBtn = new Button(*selectBtnObj);
        selectBtnObj->AddComponent(selectBtn);

        SpriteRenderer* buttonSprite = new SpriteRenderer(*selectBtnObj, "../Recursos/img/pixel.png");
        buttonSprite->SetCameraFollower(true);
        selectBtnObj->AddComponent(buttonSprite);
        Text* selectText = new Text(*selectBtnObj, "../Recursos/font/neodgm.ttf", 30, Text::SOLID, "Selecionar", {255, 255, 255, 255}, true);
        selectBtnObj->AddComponent(selectText);

        selectBtnObj->box.X = graffitiObj->box.X + (graffitiObj->box.W / 2) - (selectBtnObj->box.W / 2);
        selectBtnObj->box.Y = graffitiObj->box.Y + graffitiObj->box.H + 5;

        selectBtn->SetClickFunction([this,i]() {
            Character::SetCurrentGraffitiId(i);
            this->contentNeedsUpdate = true;
        });

        AddObject(selectBtnObj);
        pageItems.push_back(selectBtnObj);

        if (i == Character::currentGraffitiId) {
            GameObject* checkObj = new GameObject();
            SpriteRenderer* checkSprite = new SpriteRenderer(*checkObj, "../Recursos/img/check.png");
            checkSprite->SetCameraFollower(true);
            checkSprite->SetScale(0.5f, 0.5f);
            checkObj->AddComponent(checkSprite);

            checkObj->box.X = graffitiObj->box.X + graffitiObj->box.W - checkObj->box.W;
            checkObj->box.Y = graffitiObj->box.Y;

            AddObject(checkObj);
            pageItems.push_back(checkObj);
        }
    }
}
