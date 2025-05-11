#include "../include/CadernoState.hpp"

#include <iostream>

#include "../include/Button.hpp"
#include "../include/Camera.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/InputManager.hpp"
#include "../include/Game.hpp"
#include "../include/Text.hpp"

void CadernoState::CreateColorButton(string cor, SDL_Color color, int n) {
    GameObject* buttonObj = new GameObject();
    Button* button = new Button(*buttonObj);
    buttonObj->AddComponent(button);
    SpriteRenderer* buttonSprite = new SpriteRenderer(*buttonObj, "../Recursos/img/pixel.png");
    buttonSprite->SetCameraFollower(true);
    buttonObj->AddComponent(buttonSprite);
    Text* text = new Text(*buttonObj, "../Recursos/font/neodgm.ttf", 30, Text::SOLID, cor, color, true);
    buttonObj->AddComponent(text);
    buttonObj->box.X = cadernoObj->box.X + cadernoObj->box.W + 5;
    buttonObj->box.Y = cadernoObj->box.Y + (n-1)*buttonObj->box.H + (n-1)*5;
    AddObject(buttonObj);

    button->SetClickFunction([color, this]() {
        currentColor = color;
    });
    brush = true;
}

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

    currentColor = {0, 0, 0, 255};
    brushSize = 5;


    CreateColorButton("Preto", {0,0,0,255}, 1);
    CreateColorButton("Vermelho", {255,0,0,255}, 2);
    CreateColorButton("Amarelo", {255,255,0,255}, 3);
    CreateColorButton("Rosa", {254, 88, 224, 255}, 4);
    CreateColorButton("Azul", {26, 160, 251, 255}, 5);
    CreateColorButton("Borracha", {255, 255, 255, 0}, 6);

    int n = 7;
    GameObject* buttonObj = new GameObject();
    Button* button = new Button(*buttonObj);
    buttonObj->AddComponent(button);
    SpriteRenderer* buttonSprite = new SpriteRenderer(*buttonObj, "../Recursos/img/pixel.png");
    buttonSprite->SetCameraFollower(true);
    buttonObj->AddComponent(buttonSprite);
    Text* text = new Text(*buttonObj, "../Recursos/font/neodgm.ttf", 30, Text::SOLID, "APAGAR", {255, 255, 255, 255}, true);
    buttonObj->AddComponent(text);
    buttonObj->box.X = cadernoObj->box.X + cadernoObj->box.W + 5;
    buttonObj->box.Y = cadernoObj->box.Y + (n-1)*buttonObj->box.H + (n-1)*5;
    AddObject(buttonObj);

    button->SetClickFunction([this]() {
        ClearCanvas();
    });

    n++;
    GameObject* buttonObj2 = new GameObject();
    Button* button2 = new Button(*buttonObj2);
    buttonObj2->AddComponent(button2);
    SpriteRenderer* buttonSprite2 = new SpriteRenderer(*buttonObj2, "../Recursos/img/pixel.png");
    buttonSprite2->SetCameraFollower(true);
    buttonObj2->AddComponent(buttonSprite2);
    Text* text2 = new Text(*buttonObj2, "../Recursos/font/neodgm.ttf", 30, Text::SOLID, "Pincel", {255, 255, 255, 255}, true);
    buttonObj2->AddComponent(text2);
    buttonObj2->box.X = cadernoObj->box.X + cadernoObj->box.W + 5;
    buttonObj2->box.Y = cadernoObj->box.Y + (n-1)*buttonObj->box.H + (n-1)*5;
    AddObject(buttonObj2);

    button2->SetClickFunction([this]() {
        brush = true;
    });

    n++;
    GameObject* buttonObj3 = new GameObject();
    Button* button3 = new Button(*buttonObj3);
    buttonObj3->AddComponent(button3);
    SpriteRenderer* buttonSprite3 = new SpriteRenderer(*buttonObj3, "../Recursos/img/pixel.png");
    buttonSprite3->SetCameraFollower(true);
    buttonObj3->AddComponent(buttonSprite3);
    Text* text3 = new Text(*buttonObj3, "../Recursos/font/neodgm.ttf", 30, Text::SOLID, "Balde", {255, 255, 255, 255}, true);
    buttonObj3->AddComponent(text3);
    buttonObj3->box.X = cadernoObj->box.X + cadernoObj->box.W + 5;
    buttonObj3->box.Y = cadernoObj->box.Y + (n-1)*buttonObj->box.H + (n-1)*5;
    AddObject(buttonObj3);

    button3->SetClickFunction([this]() {
        brush = false;
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

    if (drawing && input.IsMouseDown(SDL_BUTTON_LEFT) && brush) {
        auto renderer = Game::GetInstance().GetRenderer();
        SDL_SetRenderTarget(renderer, canvasTexture);
        if (currentColor.a == 0) {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
        } else {
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        }

        if (lastX != -1 && lastY != -1 && (canvasX != lastX || canvasY != lastY)) {
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
    } else if (drawing && input.IsMouseDown(SDL_BUTTON_LEFT) && !brush) {
        FloodFill(canvasX, canvasY);
    }
}

void CadernoState::ClearCanvas() {
    auto renderer = Game::GetInstance().GetRenderer();
    SDL_SetRenderTarget(renderer, canvasTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
}


void CadernoState::RenderMouseBrush() {
    InputManager& input = InputManager::GetInstance();
    int x = input.GetMouseX();
    int y = input.GetMouseY();
    Vec2 mouse = Vec2(x,y);

    if (cadernoObj->box.contains(mouse)) {
        SDL_ShowCursor(SDL_DISABLE);
        auto renderer = Game::GetInstance().GetRenderer();
        //SDL_SetRenderTarget(renderer, canvasTexture);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, 255);
        for (int dx = -brushSize / 2; dx <= brushSize / 2; dx++) {
            for (int dy = -brushSize / 2; dy <= brushSize / 2; dy++) {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
        SDL_SetRenderTarget(renderer, nullptr);
    } else {
        SDL_ShowCursor(SDL_ENABLE);
    }
}

#include <queue>

void CadernoState::FloodFill(int startX, int startY) {
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();

    // Cria uma surface com os dados da canvasTexture
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
    SDL_Texture* tempTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);

    // Copia a canvasTexture para a temporária
    SDL_SetRenderTarget(renderer, tempTex);
    SDL_RenderCopy(renderer, canvasTexture, nullptr, nullptr);
    SDL_SetRenderTarget(renderer, nullptr);

    // Lê os pixels da textura para a surface
    SDL_SetRenderTarget(renderer, tempTex);
    SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, surface->pixels, surface->pitch);
    SDL_SetRenderTarget(renderer, nullptr);

    // Acesso direto ao buffer
    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / 4;

    // Cor original a ser substituída
    Uint32* pixelRef = &pixels[startY * pitch + startX];
    Uint32 targetColor = *pixelRef;

    Uint32 newColor = SDL_MapRGBA(surface->format, currentColor.r, currentColor.g, currentColor.b, currentColor.a);
    if (targetColor == newColor) return;

    std::queue<std::pair<int, int>> q;
    q.push({startX, startY});

    while (!q.empty()) {
        //auto [x, y] = q.front(); q.pop();
        std::pair<int, int> point = q.front(); q.pop();
        int x = point.first;
        int y = point.second;

        if (x < 0 || x >= width || y < 0 || y >= height) continue;
        if (pixels[y * pitch + x] != targetColor) continue;

        pixels[y * pitch + x] = newColor;

        q.push({x+1, y});
        q.push({x-1, y});
        q.push({x, y+1});
        q.push({x, y-1});
    }

    // Atualiza a canvasTexture com os pixels preenchidos
    SDL_UpdateTexture(canvasTexture, nullptr, surface->pixels, surface->pitch);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(tempTex);
}

void CadernoState::Render() {
    RenderArray();

    SDL_Rect canvasRect = {(int)cadernoObj->box.X, (int)cadernoObj->box.Y, (int)cadernoObj->box.W, (int)cadernoObj->box.H};
    SDL_RenderCopy(Game::GetInstance().GetRenderer(), canvasTexture, nullptr, &canvasRect);

    RenderMouseBrush();
}

void CadernoState::LoadAssets() {}
void CadernoState::Start() {
    StartArray();
}
void CadernoState::Pause() {}
void CadernoState::Resume() {}
CadernoState::~CadernoState() {
    SDL_DestroyTexture(canvasTexture);
    SDL_ShowCursor(SDL_ENABLE);
}
