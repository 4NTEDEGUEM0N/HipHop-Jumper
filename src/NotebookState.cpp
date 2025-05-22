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

    if (InputManager::GetInstance().KeyPress(SDLK_ESCAPE)) {
        popRequested = true;
    }
}

void NotebookState::Render() {
    RenderArray();
    RenderGraffitis();
}

void NotebookState::LoadAssets() {}
void NotebookState::Start() {
    StartArray();
}
void NotebookState::Pause() {}
void NotebookState::Resume() {}
NotebookState::~NotebookState() = default;

void NotebookState::RenderGraffitis() {
    if (Character::graffitiArray.empty()) return;

    int startIndex = (currentPage - 1) * 4;
    int endIndex = startIndex + 4;

    int i = startIndex;
    auto it = Character::graffitiArray.begin();
    advance(it, startIndex);
    for (; it != Character::graffitiArray.end() and i < endIndex; it++, i++) {
        SDL_Texture* texture = it->second;
        GameObject* graffitiObj = new GameObject(true);
        SpriteRenderer* graffiti = new SpriteRenderer(*graffitiObj, texture, it->first);
        graffiti->SetCameraFollower(true);
        graffiti->SetScale(0.4f, 0.4f);
        graffitiObj->AddComponent(graffiti);

        switch (i-startIndex) {
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

        graffiti->Render();
        delete graffitiObj;
    }
}