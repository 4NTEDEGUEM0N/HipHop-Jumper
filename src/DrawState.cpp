#include "../include/DrawState.hpp"
#include <iostream>
#include <queue>
#include "../include/Button.hpp"
#include "../include/Camera.hpp"
#include "../include/Character.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/InputManager.hpp"
#include "../include/Game.hpp"
#include "../include/Text.hpp"

void DrawState::CreateColorButton(string cor, SDL_Color color, int& n) {
    int row = n/2;
    int column = n%2;
    GameObject* buttonObj = new GameObject();
    /*
    SpriteRenderer* buttonSprite = new SpriteRenderer(*buttonObj, "../Recursos/img/pixel.png");
    buttonSprite->SetCameraFollower(true);
    buttonObj->AddComponent(buttonSprite);
    */
    Button* button = new Button(*buttonObj);
    buttonObj->AddComponent(button);
    buttonObj->box.W = 60;
    buttonObj->box.H = 60;
    buttonObj->box.X = cadernoObj->box.X + cadernoObj->box.W + 5 + 16 + 5*column + 60*column;
    buttonObj->box.Y = cadernoObj->box.Y + 18 + 65*row;
    AddObject(buttonObj);

    button->SetClickFunction([color, this]() {
        currentColor = color;
        currentColorText->SetColor(color);
        if (currentTool->GetText() == "Borracha") {
            currentTool->SetText("Pincel");
            ferramentaObj->box.X = infosObj->box.X + infosObj->box.W/2 - ferramentaObj->box.W/2;
            ferramentaObj->box.Y = infosObj->box.Y + 77/2 - ferramentaObj->box.H/2 + 15;
        }
    });
    n++;
}

DrawState::DrawState() {
    SDL_Color preto = {56,53,48,255};
    GameObject* bgObject = new GameObject();
    AddObject(bgObject);
    SpriteRenderer* bg = new SpriteRenderer(*bgObject, "../Recursos/img/BG Caderno.png");
    bg->SetCameraFollower(true);
    bgObject->AddComponent(bg);
    float scaleX = Game::VirtualScreenWidth  / (bgObject->box.W);
    float scaleY = Game::VirtualScreenHeight / (bgObject->box.H);
    bg->SetScale(scaleX, scaleY);
    bgObject->box.X = 0;
    bgObject->box.Y = 0;

    cadernoObj = new GameObject();
    AddObject(cadernoObj);
    SpriteRenderer* caderno = new SpriteRenderer(*cadernoObj, "../Recursos/img/CADERNO BRABO.png");
    caderno->SetCameraFollower(true);
    Vec2 screenCenter = Vec2(Game::VirtualScreenWidth/2.0f, Game::VirtualScreenHeight/2.0f);
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
    brush = true;
    brushSize = 5;
    drawing = false;

    GameObject* coresObj = new GameObject();
    SpriteRenderer* coresSprite = new SpriteRenderer(*coresObj, "../Recursos/img/Draw_Cores.png");
    coresSprite->SetCameraFollower(true);
    coresObj->AddComponent(coresSprite);
    coresObj->box.X = cadernoObj->box.X + cadernoObj->box.W + 10;
    coresObj->box.Y = cadernoObj->box.Y;
    AddObject(coresObj);

    infosObj = new GameObject();
    SpriteRenderer* infosSprite = new SpriteRenderer(*infosObj, "../Recursos/img/Draw_Infos.png");
    infosSprite->SetCameraFollower(true);
    infosObj->AddComponent(infosSprite);
    infosObj->box.X = cadernoObj->box.X - infosObj->box.W - 5;
    infosObj->box.Y = cadernoObj->box.Y;
    AddObject(infosObj);

    int n = 0;
    CreateColorButton("Azul Escuro", {18,0,170,255}, n);
    CreateColorButton("Azul Claro", {0,206,238,255}, n);
    CreateColorButton("Verde Escuro", {13,114,0,255}, n);
    CreateColorButton("Verde Claro", {32,201,0,255}, n);
    CreateColorButton("Laranja", {255, 186, 10, 255}, n);
    CreateColorButton("Amarelo", {253,254,0,255}, n);
    CreateColorButton("Vermelho", {224, 0, 0, 255}, n);
    CreateColorButton("Rosa", {223, 0, 255, 255}, n);
    CreateColorButton("Branco", {243, 228, 219, 255}, n);
    CreateColorButton("Preto", {0, 0, 0, 255}, n);


    ferramentaObj = new GameObject();
    currentTool = new Text(*ferramentaObj, "../Recursos/font/KGPerfectPenmanship.ttf", 30, Text::SOLID, "Pincel", preto, true);
    ferramentaObj->box.X = infosObj->box.X + infosObj->box.W/2 - ferramentaObj->box.W/2;
    ferramentaObj->box.Y = infosObj->box.Y + 77/2 - ferramentaObj->box.H/2 + 15;
    ferramentaObj->AddComponent(currentTool);
    AddObject(ferramentaObj);

    GameObject* colorObj = new GameObject();
    currentColorText = new Text(*colorObj, "../Recursos/font/neodgm.ttf", 300, Text::SOLID, "-", currentColor, true);
    colorObj->box.X = infosObj->box.X + infosObj->box.W/2 - colorObj->box.W/2 + 10;
    colorObj->box.Y = infosObj->box.Y + 77/2 - ferramentaObj->box.H/2 - 15;
    colorObj->AddComponent(currentColorText);
    AddObject(colorObj);

    GameObject* sizeObj = new GameObject();
    currentSizeText = new Text(*sizeObj, "../Recursos/font/neodgm.ttf", 30, Text::SOLID,  to_string(brushSize), preto, true);
    sizeObj->box.X = infosObj->box.X + infosObj->box.W/2 - sizeObj->box.W/2;
    sizeObj->box.Y = infosObj->box.Y + 264 - sizeObj->box.H - 10;
    sizeObj->AddComponent(currentSizeText);
    AddObject(sizeObj);

    GameObject* sizePlusObj = new GameObject();
    Button* sizePlusButton = new Button(*sizePlusObj);
    sizePlusObj->AddComponent(sizePlusButton);
    sizePlusObj->box.W = 78;
    sizePlusObj->box.H = 77;
    sizePlusObj->box.X = infosObj->box.X + infosObj->box.W - sizePlusObj->box.W;
    sizePlusObj->box.Y = infosObj->box.Y + infosObj->box.H - sizePlusObj->box.H;
    AddObject(sizePlusObj);

    sizePlusButton->SetClickFunction([this, sizeObj]() {
        if (brushSize < 20) {
            brushSize++;
            currentSizeText->SetText(to_string(brushSize));
            sizeObj->box.X = infosObj->box.X + infosObj->box.W/2 - sizeObj->box.W/2;
            sizeObj->box.Y = infosObj->box.Y + 264 - sizeObj->box.H - 10;
        }
    });

    GameObject* sizeMinusObj = new GameObject();
    Button* sizeMinusButton = new Button(*sizeMinusObj);
    sizeMinusObj->AddComponent(sizeMinusButton);
    sizeMinusObj->box.W = 78;
    sizeMinusObj->box.H = 77;
    sizeMinusObj->box.X = infosObj->box.X;
    sizeMinusObj->box.Y = infosObj->box.Y + infosObj->box.H - sizeMinusObj->box.H;
    AddObject(sizeMinusObj);

    sizeMinusButton->SetClickFunction([this, sizeObj]() {
        if (brushSize > 1) {
            brushSize--;
            currentSizeText->SetText(to_string(brushSize));
            sizeObj->box.X = infosObj->box.X + infosObj->box.W/2 - sizeObj->box.W/2;
            sizeObj->box.Y = infosObj->box.Y + 264 - sizeObj->box.H - 10;
        }
    });

    int column = 0;
    int row = 0;
    GameObject* borrachaObj = new GameObject();
    Button* borrachaButton = new Button(*borrachaObj);
    borrachaObj->AddComponent(borrachaButton);
    borrachaObj->box.W = 54;
    borrachaObj->box.H = 54;
    borrachaObj->box.X = cadernoObj->box.X + cadernoObj->box.W + 10 + 16 + 3*column + 60*column;
    borrachaObj->box.Y = cadernoObj->box.Y + 377 + 66*row;
    AddObject(borrachaObj);

    borrachaButton->SetClickFunction([this]() {
        brush = true;
        currentTool->SetText("Borracha");
        currentColor = {255, 255, 255, 0};
        currentColorText->SetColor({255, 255, 255, 0});
        ferramentaObj->box.X = infosObj->box.X + infosObj->box.W/2 - ferramentaObj->box.W/2;
        ferramentaObj->box.Y = infosObj->box.Y + 77/2 - ferramentaObj->box.H/2 + 15;
    });

    row = 0;
    column = 1;
    GameObject* pincelObj = new GameObject();
    Button* pincelButton = new Button(*pincelObj);
    pincelObj->AddComponent(pincelButton);
    pincelObj->box.W = 54;
    pincelObj->box.H = 54;
    pincelObj->box.X = cadernoObj->box.X + cadernoObj->box.W + 10 + 16 + 3*column + 60*column;
    pincelObj->box.Y = cadernoObj->box.Y + 377 + 66*row;
    AddObject(pincelObj);

    pincelButton->SetClickFunction([this]() {
        brush = true;
        currentTool->SetText("Pincel");
        ferramentaObj->box.X = infosObj->box.X + infosObj->box.W/2 - ferramentaObj->box.W/2;
        ferramentaObj->box.Y = infosObj->box.Y + 77/2 - ferramentaObj->box.H/2 + 15;
    });

    row = 1;
    column = 0;
    GameObject* baldeObj = new GameObject();
    Button* baldeButton = new Button(*baldeObj);
    baldeObj->AddComponent(baldeButton);
    baldeObj->box.W = 54;
    baldeObj->box.H = 54;
    baldeObj->box.X = cadernoObj->box.X + cadernoObj->box.W + 10 + 16 + 3*column + 60*column;
    baldeObj->box.Y = cadernoObj->box.Y + 377 + 66*row;
    AddObject(baldeObj);

    baldeButton->SetClickFunction([this]() {
        brush = false;
        currentTool->SetText("Balde");
        ferramentaObj->box.X = infosObj->box.X + infosObj->box.W/2 - ferramentaObj->box.W/2;
        ferramentaObj->box.Y = infosObj->box.Y + 77/2 - ferramentaObj->box.H/2 + 15;
    });

    row = 1;
    column = 1;
    GameObject* apagarObj = new GameObject();
    Button* apagarButton = new Button(*apagarObj);
    apagarObj->AddComponent(apagarButton);
    apagarObj->box.W = 54;
    apagarObj->box.H = 54;
    apagarObj->box.X = cadernoObj->box.X + cadernoObj->box.W + 10 + 16 + 3*column + 60*column;
    apagarObj->box.Y = cadernoObj->box.Y + 377 + 66*row;
    AddObject(apagarObj);

    apagarButton->SetClickFunction([this]() {
        ClearCanvas();
    });

    row = 2;
    column = 0;
    GameObject* exitObj = new GameObject();
    Button* exitButton = new Button(*exitObj);
    exitObj->AddComponent(exitButton);
    exitObj->box.W = 54;
    exitObj->box.H = 54;
    exitObj->box.X = cadernoObj->box.X + cadernoObj->box.W + 10 + 16 + 3*column + 60*column;
    exitObj->box.Y = cadernoObj->box.Y + 377 + 66*row;
    AddObject(exitObj);

    exitButton->SetClickFunction([this]() {
        popRequested = true;
    });

    row = 2;
    column = 1;
    GameObject* salvarObj = new GameObject();
    Button* salvarButton = new Button(*salvarObj);
    salvarObj->AddComponent(salvarButton);
    salvarObj->box.W = 54;
    salvarObj->box.H = 54;
    salvarObj->box.X = cadernoObj->box.X + cadernoObj->box.W + 10 + 16 + 3*column + 60*column;
    salvarObj->box.Y = cadernoObj->box.Y + 377 + 66*row;
    AddObject(salvarObj);

    salvarButton->SetClickFunction([this]() {
        Character::AddGraffiti(canvasTexture);
        popRequested = true;
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

void DrawState::Update(float dt) {
    UpdateArray(dt);

    InputManager& input = InputManager::GetInstance();
    if (input.KeyPress(SDLK_ESCAPE)) {
        popRequested = true;
    }
    quitRequested = input.QuitRequested();

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

void DrawState::ClearCanvas() {
    auto renderer = Game::GetInstance().GetRenderer();
    SDL_SetRenderTarget(renderer, canvasTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, nullptr);
}


void DrawState::RenderMouseBrush() {
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

void DrawState::FloodFill(int startX, int startY) {
    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();

    // Cria uma surface com os dados da canvasTexture
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);

    // Lê os pixels da textura para a surface
    SDL_SetRenderTarget(renderer, canvasTexture);
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
}

void DrawState::Render() {
    RenderArray();

    SDL_Rect canvasRect = {(int)cadernoObj->box.X, (int)cadernoObj->box.Y, (int)cadernoObj->box.W, (int)cadernoObj->box.H};
    SDL_RenderCopy(Game::GetInstance().GetRenderer(), canvasTexture, nullptr, &canvasRect);

    RenderMouseBrush();
}

void DrawState::LoadAssets() {}
void DrawState::Start() {
    StartArray();
}
void DrawState::Pause() {}
void DrawState::Resume() {}
DrawState::~DrawState() {
    SDL_DestroyTexture(canvasTexture);
    SDL_ShowCursor(SDL_ENABLE);
}
