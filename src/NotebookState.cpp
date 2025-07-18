#include "../include/Animator.hpp"
#include "../include/NotebookState.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Button.hpp"
#include "../include/InputManager.hpp"
#include "../include/DrawState.hpp"

#include <iostream>

#include "../include/Character.hpp"
#include "../include/LevelSelectorState.hpp"

NotebookState::NotebookState() {
    currentPage = 1;
    SDL_Color preto = {56,53,48,255};

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

    cadernoObj = new GameObject();
    AddObject(cadernoObj);
    SpriteRenderer* caderno = new SpriteRenderer(*cadernoObj, "../Recursos/img/CADERNO BRABO.png");
    caderno->SetCameraFollower(true);
    Vec2 screenCenter = Vec2(Game::VirtualScreenWidth/2.0f, Game::VirtualScreenHeight/2.0f);
    cadernoObj->box.X = screenCenter.GetX() - cadernoObj->box.W / 2;
    cadernoObj->box.Y = screenCenter.GetY() - cadernoObj->box.H / 2;
    cadernoObj->AddComponent(caderno);

    GameObject* currentPageObj = new GameObject();
    SpriteRenderer* currentPageButtonSprite = new SpriteRenderer(*currentPageObj, "../Recursos/img/Caderno_Botão_Fundo.png");
    currentPageButtonSprite->SetCameraFollower(true);
    currentPageObj->AddComponent(currentPageButtonSprite);
    currentPageObj->box.W = 92;
    currentPageObj->box.H = 30;
    currentPageObj->box.X = cadernoObj->box.X + cadernoObj->box.W/2 - currentPageObj->box.W/2;
    currentPageObj->box.Y = cadernoObj->box.Y - currentPageObj->box.H;
    AddObject(currentPageObj);

    GameObject* currentPageTextObj = new GameObject();
    currentPageText = new Text(*currentPageTextObj, "../Recursos/font/KGPerfectPenmanship.ttf", 21, Text::SOLID, "Página " + to_string(currentPage), preto, true);
    currentPageTextObj->AddComponent(currentPageText);
    currentPageTextObj->box.X = cadernoObj->box.X + cadernoObj->box.W/2 - currentPageTextObj->box.W/2;
    currentPageTextObj->box.Y = cadernoObj->box.Y - currentPageTextObj->box.H;
    AddObject(currentPageTextObj);

    GameObject* previousPageObj = new GameObject();
    Button* previousPageButton = new Button(*previousPageObj);
    previousPageObj->AddComponent(previousPageButton);
    SpriteRenderer* previousPageButtonSprite = new SpriteRenderer(*previousPageObj, "../Recursos/img/Caderno_previous.png");
    previousPageButtonSprite->SetCameraFollower(true);
    previousPageObj->AddComponent(previousPageButtonSprite);
    previousPageObj->box.W = 30;
    previousPageObj->box.H = 30;
    previousPageObj->box.X = currentPageObj->box.X - previousPageObj->box.W - 5;
    previousPageObj->box.Y = currentPageObj->box.Y;
    AddObject(previousPageObj);

    previousPageButton->SetClickFunction([this]() {
        if (currentPage > 1) {
            currentPage--;
            currentPageText->SetText("Página " + to_string(currentPage));
            UpdatePageContent();
        }
    });

    GameObject* nextPageObj = new GameObject();
    Button* nextPageButton = new Button(*nextPageObj);
    nextPageObj->AddComponent(nextPageButton);
    SpriteRenderer* nextPageButtonSprite = new SpriteRenderer(*nextPageObj, "../Recursos/img/Caderno_next.png");
    nextPageButtonSprite->SetCameraFollower(true);
    nextPageObj->AddComponent(nextPageButtonSprite);
    nextPageObj->box.W = 30;
    nextPageObj->box.H = 30;
    nextPageObj->box.X = currentPageObj->box.X + currentPageObj->box.W + 5;
    nextPageObj->box.Y = currentPageObj->box.Y;
    AddObject(nextPageObj);

    nextPageButton->SetClickFunction([this, currentPageObj]() {
        if (currentPage < 9) {
            currentPage++;
            currentPageText->SetText("Página " + to_string(currentPage));
            UpdatePageContent();
        }
    });

    GameObject* drawObj = new GameObject();
    Button* drawButton = new Button(*drawObj);
    drawObj->AddComponent(drawButton);
    SpriteRenderer* drawButtonSprite = new SpriteRenderer(*drawObj, "../Recursos/img/Caderno_desenhar.png");
    drawButtonSprite->SetCameraFollower(true);
    drawObj->AddComponent(drawButtonSprite);
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
    InputManager& inputManager = InputManager::GetInstance();

    if (contentNeedsUpdate) {
        UpdatePageContent();
        contentNeedsUpdate = false;
    }

    if (inputManager.KeyPress(SDLK_ESCAPE)) {
        popRequested = true;
    }
    quitRequested = inputManager.QuitRequested();
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

void NotebookState::RenderCollectables(int level) {
    
    if (level == 1) {
        int row = 0;
        int column = 0;
        int minikits = Character::player->minikits;
        for (int i = 0; i < 6; i++, column++) {
            if (i % 3 == 0 and i>0) {
                row++;
                column = 0;
            }
            GameObject* minikitObj = new GameObject();
            AddObject(minikitObj);
            pageItems.push_back(minikitObj);
            SpriteRenderer* minikit = new SpriteRenderer(*minikitObj, "../Recursos/img/minikitHud.png", 2, 1);
            minikit->SetCameraFollower(true);
            if (minikits > 0) {
                minikit->SetFrame(1);
                minikits--;
            } else {
                minikit->SetFrame(0);
            }
            minikitObj->AddComponent(minikit);
            minikitObj->box.X = (cadernoObj->box.X + (cadernoObj->box.W)/2) + minikitObj->box.W/2 + column*128.5;
            minikitObj->box.Y = 168 + row*100;
        }
        row++;
        column = 0;
        for (int i = 6 ;i < 9; i++, column++) {
            string file = "";
            if (i-6+1 <= Character::player->inventory.size()) {
                if (Character::player->inventory[i-6].name == "LATA") {
                    file = Character::player->inventory[i-6].iconPath;
                    
                    GameObject* canObj = new GameObject();
                    AddObject(canObj);
                    pageItems.push_back(canObj);
                    SpriteRenderer* can = new SpriteRenderer(*canObj, file, 9, 9);
                    can->SetCameraFollower(true);
                    canObj->AddComponent(can);
                    
                    Animator* canAnimator = new Animator(*canObj);
                    canAnimator->SetAnimation("spin");
                    canObj->AddComponent(canAnimator);
                    
                    canObj->box.Z = 2;
                    canObj->box.X = (cadernoObj->box.X + (cadernoObj->box.W)/2) + canObj->box.W/2 + column*128.5;
                    canObj->box.Y = 168 + row*100;
                }
            } else {
                file = "../Recursos/img/COLETAVEISSCALED/SILHUETALATA.png";
                
                GameObject* canObj = new GameObject();
                AddObject(canObj);
                pageItems.push_back(canObj);
                SpriteRenderer* can = new SpriteRenderer(*canObj, file);
                //can->SetScale(0.8, 0.8);
                can->SetCameraFollower(true);
                canObj->AddComponent(can);
                
                canObj->box.Z = 2;
                canObj->box.X = (cadernoObj->box.X + (cadernoObj->box.W)/2) + canObj->box.W/2 + column*128.5;
                canObj->box.Y = 168 + row*100;
                
            }
        }
    }
        
    if (level == 2) {
        int row = 0;
        int column = 0;
        int minikits = Character::player->minikits2;
        for (int i = 0; i < 6; i++, column++) {
            if (i % 3 == 0 and i>0) {
                row++;
                column = 0;
            }
            GameObject* minikitObj = new GameObject();
            AddObject(minikitObj);
            pageItems.push_back(minikitObj);
            SpriteRenderer* minikit = new SpriteRenderer(*minikitObj, "../Recursos/img/minikitHud.png", 2, 1);
            minikit->SetCameraFollower(true);
            if (minikits > 0) {
                minikit->SetFrame(1);
                minikits--;
            } else {
                minikit->SetFrame(0);
            }
            minikitObj->AddComponent(minikit);
            minikitObj->box.X = (cadernoObj->box.X + (cadernoObj->box.W)/2) + minikitObj->box.W/2 + column*128.5;
            minikitObj->box.Y = 168 + row*100;
        }
        row++;
        column = 0;
        for (int i = 6 ;i < 9; i++, column++) {
            string file = "";
            if (i-6+1 <= Character::player->inventory.size()) {
                if (Character::player->inventory[i-6].name == "MARCADOR") {
                    file = Character::player->inventory[i-6].iconPath;
                    
                    GameObject* canObj = new GameObject();
                    AddObject(canObj);
                    pageItems.push_back(canObj);
                    SpriteRenderer* can = new SpriteRenderer(*canObj, file, 9, 9);
                    can->SetCameraFollower(true);
                    canObj->AddComponent(can);
                    
                    Animator* canAnimator = new Animator(*canObj);
                    canAnimator->SetAnimation("spin");
                    canObj->AddComponent(canAnimator);
                    
                    canObj->box.Z = 2;
                    canObj->box.X = (cadernoObj->box.X + (cadernoObj->box.W)/2) + canObj->box.W/2 + column*128.5;
                    canObj->box.Y = 168 + row*100;
                }
            } else {
                file = "../Recursos/img/COLETAVEISSCALED/SILHUETAMARCADOR.png";
                
                GameObject* canObj = new GameObject();
                AddObject(canObj);
                pageItems.push_back(canObj);
                SpriteRenderer* can = new SpriteRenderer(*canObj, file);
                //can->SetScale(0.08, 0.08);
                can->SetCameraFollower(true);
                canObj->AddComponent(can);
                
                canObj->box.Z = 2;
                canObj->box.X = (cadernoObj->box.X + (cadernoObj->box.W)/2) + canObj->box.W/2 + column*128.5;
                canObj->box.Y = 168 + row*100;
                
            }
        }
    }
            
    if (level == 3) {
        int row = 0;
        int column = 0;
        int minikits = Character::player->minikits3;
        for (int i = 0; i < 6; i++, column++) {
            if (i % 3 == 0 and i>0) {
                row++;
                column = 0;
            }
            GameObject* minikitObj = new GameObject();
            AddObject(minikitObj);
            pageItems.push_back(minikitObj);
            SpriteRenderer* minikit = new SpriteRenderer(*minikitObj, "../Recursos/img/minikitHud.png", 2, 1);
            minikit->SetCameraFollower(true);
            if (minikits > 0) {
                minikit->SetFrame(1);
                minikits--;
            } else {
                minikit->SetFrame(0);
            }
            minikitObj->AddComponent(minikit);
            minikitObj->box.X = (cadernoObj->box.X + (cadernoObj->box.W)/2) + minikitObj->box.W/2 + column*128.5;
            minikitObj->box.Y = 168 + row*100;
        }
        row++;
        column = 0;
        for (int i = 6 ;i < 15; i++, column++) {
            string file = "";
            if (i-6+1 <= Character::player->inventory.size()) {
                if (Character::player->inventory[i-6].name == "BALDE") {
                    file = Character::player->inventory[i-6].iconPath;
                    
                    GameObject* canObj = new GameObject();
                    AddObject(canObj);
                    pageItems.push_back(canObj);
                    SpriteRenderer* can = new SpriteRenderer(*canObj, file, 9, 9);
                    can->SetCameraFollower(true);
                    canObj->AddComponent(can);
                    
                    Animator* canAnimator = new Animator(*canObj);
                    canAnimator->SetAnimation("spin");
                    canObj->AddComponent(canAnimator);
                    
                    canObj->box.Z = 2;
                    canObj->box.X = (cadernoObj->box.X + (cadernoObj->box.W)/2) + canObj->box.W/2 + column*128.5;
                    canObj->box.Y = 168 + row*100;
                }
            } else {
                file = "../Recursos/img/COLETAVEISSCALED/SILHUETABALDE.png";
                
                GameObject* canObj = new GameObject();
                AddObject(canObj);
                pageItems.push_back(canObj);
                SpriteRenderer* can = new SpriteRenderer(*canObj, file);
                //can->SetScale(0.08, 0.08);
                can->SetCameraFollower(true);
                canObj->AddComponent(can);
                
                canObj->box.Z = 2;
                canObj->box.X = (cadernoObj->box.X + (cadernoObj->box.W)/2) + canObj->box.W/2 + column*128.5;
                canObj->box.Y = 168 + row*100;
            }
        }
    }
}

void NotebookState::UpdatePageContent() {
    for (auto& item : pageItems) {
        item->RequestDelete();
    }
    pageItems.clear();
    
    if (currentPage == 1) {
        GameObject* logoObj = new GameObject();
        AddObject(logoObj);
        pageItems.push_back(logoObj);
        SpriteRenderer* logo = new SpriteRenderer(*logoObj, "../Recursos/img/hiphopjumper_logo_caderno.png");
        logo->SetCameraFollower(true);
        logoObj->AddComponent(logo);
        //logo->SetScale(0.5f, 0.5f);
        logoObj->box.X = (cadernoObj->box.X + (cadernoObj->box.W)/4) - logoObj->box.W/2;
        logoObj->box.Y = cadernoObj->box.Y + 20;


        string text1;
        if (LevelSelectorState::currentLevel == 0) {
            text1 = "No caderno você pode ver algumas \n"
        "informações importantes e também\n"
        "pode libertar sua criatividade.\n\n"
        "Na primeira página temos sua cole-\n"
        "ção de coletáveis para você ficar\n"
        "atento e não perder nenhum deles\n"
        "ao longo da fase.\n\n"
        "Você também pode desenhar seu\n"
        "próprio graffiti! Eles ficarão salvos\n"
        "nas próximas páginas.";
        } else {
            text1 = "LEVEL 1: O Nascimento de Gaia\n\n"
        "Explorar os obstáculos de um\n"
        "cenário urbano em busca do\n"
        "espaço perfeito para a arte\n"
        "de rua. Se expressar através\n"
        "do grafite e do HipHop enquanto\n"
        "dribla as forças de vigilância.\n";
        }


        GameObject* textObj1 = new GameObject();
        AddObject(textObj1);
        pageItems.push_back(textObj1);
        Text* descriptionText = new Text(*textObj1, "../Recursos/font/KGPerfectPenmanship.ttf", 24, Text::SOLID, text1, {0, 0, 0, 255}, true);
        textObj1->AddComponent(descriptionText);
        textObj1->box.X = cadernoObj->box.X + 30;
        textObj1->box.Y = logoObj->box.Y + logoObj->box.H + 20;

        GameObject* collectObj = new GameObject();
        AddObject(collectObj);
        pageItems.push_back(collectObj);
        Text* collectText = new Text(*collectObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 70, Text::BLENDED, "Coletáveis", {0, 0, 0, 255}, true);
        collectObj->AddComponent(collectText);
        collectObj->box.X = (cadernoObj->box.X + (3*cadernoObj->box.W)/4) - collectObj->box.W/2 - 20;
        collectObj->box.Y = cadernoObj->box.Y + 20;

        RenderCollectables(1);
    }
    
    if (currentPage == 2 && LevelSelectorState::currentLevel != 0) {
        GameObject* logoObj = new GameObject();
        AddObject(logoObj);
        pageItems.push_back(logoObj);
        SpriteRenderer* logo = new SpriteRenderer(*logoObj, "../Recursos/img/hiphopjumper.png");
        logo->SetCameraFollower(true);
        logoObj->AddComponent(logo);
        logo->SetScale(0.5f, 0.5f);
        logoObj->box.X = (cadernoObj->box.X + (cadernoObj->box.W)/4) - logoObj->box.W/2;
        logoObj->box.Y = cadernoObj->box.Y + 20;

        string text1;
        text1 = "LEVEL 2: O Mito de Sísifo\n\n"
        "Subir pelas veias da metrópole,\n"
        "encarar becos, muros e viadutos\n"
        "num esforço constante e desafiador.\n"
        "Como Sísifo empurrando sua arte\n"
        "contra a corrente, cada traço é\n"
        "resistência no coração urbano.\n";

        GameObject* textObj1 = new GameObject();
        AddObject(textObj1);
        pageItems.push_back(textObj1);
        Text* descriptionText = new Text(*textObj1, "../Recursos/font/KGPerfectPenmanship.ttf", 24, Text::SOLID, text1, {0, 0, 0, 255}, true);
        textObj1->AddComponent(descriptionText);
        textObj1->box.X = cadernoObj->box.X + 30;
        textObj1->box.Y = logoObj->box.Y + logoObj->box.H + 20;

        GameObject* collectObj = new GameObject();
        AddObject(collectObj);
        pageItems.push_back(collectObj);
        Text* collectText = new Text(*collectObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 70, Text::BLENDED, "Coletáveis", {0, 0, 0, 255}, true);
        collectObj->AddComponent(collectText);
        collectObj->box.X = (cadernoObj->box.X + (3*cadernoObj->box.W)/4) - collectObj->box.W/2 - 20;
        collectObj->box.Y = cadernoObj->box.Y + 20;

        RenderCollectables(2);
    }
    
    if (currentPage == 3 && LevelSelectorState::currentLevel != 0) {
        GameObject* logoObj = new GameObject();
        AddObject(logoObj);
        pageItems.push_back(logoObj);
        SpriteRenderer* logo = new SpriteRenderer(*logoObj, "../Recursos/img/hiphopjumper.png");
        logo->SetCameraFollower(true);
        logoObj->AddComponent(logo);
        logo->SetScale(0.5f, 0.5f);
        logoObj->box.X = (cadernoObj->box.X + (cadernoObj->box.W)/4) - logoObj->box.W/2;
        logoObj->box.Y = cadernoObj->box.Y + 20;

        string text1;
        text1 = "LEVEL 3: O Vôo de Ícaro\n\n"
        "Alcançar os céus da cidade,\n"
        "os topos esquecidos e as alturas\n"
        "proibidas. Ali, onde o vento bate\n"
        "mais forte, o grafite encontra sua\n"
        "glória, e seu risco. Voar alto é\n"
        "liberdade, mas também tentação.\n";

        GameObject* textObj1 = new GameObject();
        AddObject(textObj1);
        pageItems.push_back(textObj1);
        Text* descriptionText = new Text(*textObj1, "../Recursos/font/KGPerfectPenmanship.ttf", 24, Text::SOLID, text1, {0, 0, 0, 255}, true);
        textObj1->AddComponent(descriptionText);
        textObj1->box.X = cadernoObj->box.X + 30;
        textObj1->box.Y = logoObj->box.Y + logoObj->box.H + 20;

        GameObject* collectObj = new GameObject();
        AddObject(collectObj);
        pageItems.push_back(collectObj);
        Text* collectText = new Text(*collectObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 70, Text::BLENDED, "Coletáveis", {0, 0, 0, 255}, true);
        collectObj->AddComponent(collectText);
        collectObj->box.X = (cadernoObj->box.X + (3*cadernoObj->box.W)/4) - collectObj->box.W/2 - 20;
        collectObj->box.Y = cadernoObj->box.Y + 20;

        RenderCollectables(3);
    }

    if (currentPage > 3 || (LevelSelectorState::currentLevel == 0 && currentPage > 1)) {
        int magicNumber = 4;
        if (LevelSelectorState::currentLevel == 0) {
            magicNumber = 2;
        }
        if (Character::graffitiArray.empty()) return;

        int startIndex = (currentPage - magicNumber) * 4;
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
                default: break;
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
}
