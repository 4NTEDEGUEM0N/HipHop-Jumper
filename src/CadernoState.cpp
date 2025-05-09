#include "../include/CadernoState.hpp"

#include <iostream>

#include "../include/Button.hpp"
#include "../include/Camera.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/InputManager.hpp"
#include "../include/Game.hpp"
#include "../include/Text.hpp"

CadernoState::CadernoState() {
    cadernoObj = new GameObject();
    AddObject(cadernoObj);
    SpriteRenderer* caderno = new SpriteRenderer(*cadernoObj, "../Recursos/img/Caderno.png");
    caderno->SetCameraFollower(true);
    caderno->SetScale(2,2);
    Vec2 screenCenter = Vec2(1200/2.0f, 900/2.0f);
    cadernoObj->box.X = screenCenter.GetX() - cadernoObj->box.W / 2;
    cadernoObj->box.Y = screenCenter.GetY() - cadernoObj->box.H / 2;
    cadernoObj->AddComponent(caderno);

    width = cadernoObj->box.W;
    height = cadernoObj->box.H;
    auto renderer = Game::GetInstance().GetRenderer();
    canvasTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetTextureBlendMode(canvasTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, canvasTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);

    currentColor = {255, 0, 0, 255};
    brushSize = 5;

    GameObject* buttonObj = new GameObject();
    Button* button = new Button(*buttonObj);
    buttonObj->AddComponent(button);
    SpriteRenderer* buttonSprite = new SpriteRenderer(*buttonObj, "../Recursos/img/pixel.png");
    buttonSprite->SetCameraFollower(true);
    buttonObj->AddComponent(buttonSprite);
    Text* text = new Text(*buttonObj, "../Recursos/font/neodgm.ttf", 30, Text::SOLID, "Vermelho", {255, 0, 0, 255}, true);
    buttonObj->AddComponent(text);
    //buttonSprite->SetScale(200,50);
    buttonObj->box.X = cadernoObj->box.X + cadernoObj->box.W + 5;
    buttonObj->box.Y = cadernoObj->box.Y;
    AddObject(buttonObj);

    button->SetClickFunction([this]() {
        currentColor = {255, 0, 0, 255};
    });

    GameObject* buttonObj2 = new GameObject();
    Button* button2 = new Button(*buttonObj2);
    buttonObj2->AddComponent(button2);
    SpriteRenderer* buttonSprite2 = new SpriteRenderer(*buttonObj2, "../Recursos/img/pixel.png");
    buttonSprite2->SetCameraFollower(true);
    buttonObj2->AddComponent(buttonSprite2);
    //buttonSprite->SetScale(200,50);
    Text* text2 = new Text(*buttonObj2, "../Recursos/font/neodgm.ttf", 30, Text::SOLID, "Amarelo", {255, 255, 0, 255}, true);
    buttonObj2->AddComponent(text2);
    buttonObj2->box.X = cadernoObj->box.X + cadernoObj->box.W + 5;
    buttonObj2->box.Y = cadernoObj->box.Y + buttonObj2->box.H + 5;
    AddObject(buttonObj2);

    //GameObject* textObj = new GameObject();

    button2->SetClickFunction([this]() {
        currentColor = {255, 255, 0, 255};
    });
}

void DrawBrush(SDL_Renderer* renderer, int x, int y, int size, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int dx = -size/2; dx <= size/2; ++dx) {
        for (int dy = -size/2; dy <= size/2; ++dy) {
            SDL_RenderDrawPoint(renderer, x + dx, y + dy);
        }
    }
}

void CadernoState::Update(float dt) {
    UpdateArray(dt);

    if (InputManager::GetInstance().KeyPress(SDLK_ESCAPE)) {
        popRequested = true;
    }

    InputManager& input = InputManager::GetInstance();

    int x = input.GetMouseX();
    int y = input.GetMouseY();
    int canvasX = x - cadernoObj->box.X;
    int canvasY = y - cadernoObj->box.Y;

    if (input.MousePress(SDL_BUTTON_LEFT)) {
        drawing = true;
        lastX = canvasX;
        lastY = canvasY;
    } else if (input.MouseRelease(SDL_BUTTON_LEFT)) {
        drawing = false;
        lastX = lastY = -1;
    }

    if (drawing && input.IsMouseDown(SDL_BUTTON_LEFT)) {
        auto renderer = Game::GetInstance().GetRenderer();
        SDL_SetRenderTarget(renderer, canvasTexture);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        if (lastX != -1 && lastY != -1) {
            // Para linhas grossas: interpola pontos ao longo da linha
            int steps = max(abs(canvasX - lastX), abs(canvasY - lastY));
            for (int i = 0; i <= steps; ++i) {
                float t = i / (float)steps;
                int ix = lastX + t * (canvasX - lastX);
                int iy = lastY + t * (canvasY - lastY);
                DrawBrush(renderer, ix, iy, brushSize, currentColor);
            }
        } else {
            DrawBrush(renderer, canvasX, canvasY, brushSize, currentColor);
        }

        SDL_SetRenderTarget(renderer, nullptr);
        lastX = canvasX;
        lastY = canvasY;
    }
}

void CadernoState::Render() {
    RenderArray();

    SDL_Rect canvasRect = {(int)cadernoObj->box.X, (int)cadernoObj->box.Y, (int)cadernoObj->box.W, (int)cadernoObj->box.H};
    SDL_RenderCopy(Game::GetInstance().GetRenderer(), canvasTexture, nullptr, &canvasRect);
}

void CadernoState::LoadAssets() {}
void CadernoState::Start() {
    StartArray();
}
void CadernoState::Pause() {}
void CadernoState::Resume() {}
CadernoState::~CadernoState() {
    SDL_DestroyTexture(canvasTexture);
}
