#include "../include/Stage2State.hpp"
#include <iostream>

#include "../include/SpriteRenderer.hpp"
#include "../include/Zombie.hpp"
#include "../include/TileMap.hpp"
#include "../include/TileSet.hpp"
#include "../include/InputManager.hpp"
#include "../include/Camera.hpp"
#include "../include/Character.hpp"
#include "../include/PlayerController.hpp"
#include "../include/Collider.hpp"
#include "../include/Collision.hpp"
#include "../include/Animator.hpp"
#include "../include/MusicManager.hpp"
#include "../include/AmbientManager.hpp"
#include <algorithm>

#include "../include/DetectionZone.hpp"
#include "../include/DialogState.hpp"
#include "../include/EndState.hpp"
#include "../include/TitleState.hpp"
#include "../include/Game.hpp"
#include "../include/GameData.hpp"
#include "../include/HUD.hpp"
#include "../include/Item.hpp"
#include "../include/NotebookState.hpp"
#include "../include/PauseState.hpp"
#include "../include/KeyBindingManager.hpp"
#include "../include/NPC.hpp"

GameObject* CreateMinikit2(Vec2 pos) {
    ItemData minikitData = ItemData::Minikit();

    GameObject* minikitObject = new GameObject(false);
    minikitObject->box.X = pos.X*64;
    minikitObject->box.Y = pos.Y*64;
    Item* minikit = new Item(*minikitObject, minikitData);
    minikitObject->AddComponent(minikit);
    return minikitObject;
}

Stage2State::Stage2State() {
    endTimer = new Timer();
    GameObject* bgObject = new GameObject();
    //AddObject(bgObject);
    SpriteRenderer* bg = new SpriteRenderer(*bgObject, "../Recursos/img/Background.png");
    bg->SetCameraFollower(true);
    bgObject->AddComponent(bg);

    float scaleX = Game::VirtualScreenWidth  / 1200.0f;
    float scaleY = Game::VirtualScreenHeight / 900.0f;
    bg->SetScale(scaleX, scaleY);
    bgObject->box.X = 0;
    bgObject->box.Y = 0;


    TileSet* cenario2frenteTileSet = new TileSet("../Recursos/img/2_3fase/area 2 parallax.png", 64, 64);
    TileSet* collisionTileSet = new TileSet("../Recursos/img/2_3fase/assets juntos primeira fase.png", 64, 64);
    TileSet* detalhes1TileSet = new TileSet("../Recursos/img/2_3fase/assets juntos primeira fase.png", 64, 64);
    TileSet* detalhes2TileSet = new TileSet("../Recursos/img/2_3fase/Sprite sheets detalhes.png", 64, 64);
    TileSet* cenario2TrasTileSet = new TileSet("../Recursos/img/2_3fase/area 2 trás.png", 64, 64);
    TileSet* luzesTileSet = new TileSet("../Recursos/img/2_3fase/assets juntos primeira fase.png", 64, 64);
    TileSet* cenario2FundoTileSet = new TileSet("../Recursos/img/2_3fase/assets juntos primeira fase.png", 64, 64);
    TileSet* cenario3TileSet = new TileSet("../Recursos/img/2_3fase/cenário 3.png", 64, 64);
    TileSet* cenario2FundoParalaxTileSet = new TileSet("../Recursos/img/2_3fase/fundo segundo cenário.png", 64, 64);

    std::vector<TileSet*> TileSets;
    TileSets.push_back(cenario2frenteTileSet);
    TileSets.push_back(collisionTileSet);
    TileSets.push_back(detalhes1TileSet);
    TileSets.push_back(detalhes2TileSet);
    TileSets.push_back(cenario2TrasTileSet);
    TileSets.push_back(luzesTileSet);
    TileSets.push_back(cenario2FundoTileSet);
    TileSets.push_back(cenario3TileSet);
    TileSets.push_back(cenario2FundoParalaxTileSet);

    tileMapObject = new GameObject();
    AddObject(tileMapObject);
    tileMapObject->box.X = 0;
    tileMapObject->box.Y = 0;
    set<int> solidIDs = {0,1,2,41,42,43,82,83,84,123,124,125,164,165,166,205,206,207,246,247,250,251,252,254,257,134,175,176,214,215,216,
        1230, 1231, 1232, 1233, 1234, 1235, 1236, 1237, 1238, 1239, 1240, 1241, 1242, 1243, 1244, 1245, 1246, 1247, 1248, 1249, 1250, 1251,
        1252, 1253, 1254, 1255, 1256, 1257, 1258, 1259, 1260, 1261, 1262, 1263, 1264, 1265, 1266, 1267, 1268, 1269};
    tileSet = collisionTileSet;
    TileMap* tileMap = new TileMap(*tileMapObject, "../Recursos/map/2_3_fase.txt", TileSets, solidIDs, 1);
    tileMapObject->AddComponent(tileMap);

    GameObject* playerObject = new GameObject(false);
    AddObject(playerObject);
    Character* playerCharacter = new Character(*playerObject, "../Recursos/img/sprite_spray_3_scaled.png");
    playerObject->box.X = 8*64;
    playerObject->box.Y = 164*64 - playerObject->box.H;
    playerObject->box.Z = 1;
    Camera::Follow(playerObject);
    playerObject->AddComponent(playerCharacter);
    PlayerController* playerController = new PlayerController(*playerObject);
    playerObject->AddComponent(playerController);

    backgroundMusic.Open("../Recursos/audio/TRACKS/THEME 2 LOOP 3.wav");
    backgroundMusic.SetIntro("../Recursos/audio/TRACKS/THEME 2 LOOP 1.wav");

    GameObject* nineNPCObj = new GameObject();
    AddObject(nineNPCObj);
    NPC* nineNPC = new NPC(*nineNPCObj, "../Recursos/img/nine.png", "Nine");
    nineNPCObj->AddComponent(nineNPC);
    nineNPCObj->box.X = 16*64;
    nineNPCObj->box.Y = 167*64 - nineNPCObj->box.H;
    Collider* npcCollider = new Collider(*nineNPCObj, {2,2});
    nineNPCObj->AddComponent(npcCollider);

    nineNPC->dialogs.emplace_back("Eae, é o nine! Ratorious me falou de você. Dahora!");
    nineNPC->dialogs.emplace_back("Mas se quiser realmente entrar pra crew vai ter que fazer algo mais desafiador\nque subir de um bueiro.");
    nineNPC->dialogs.emplace_back("Se o Ratorious te falou alguma coisa de ser o melhor graffiteiro da cidade,\nele tava mentindo, porque sou eu!");
    nineNPC->dialogs.emplace_back("Por isso o meu desafio é maior que o dele.");
    nineNPC->dialogs.emplace_back("Em cima desse prédio tem um spot de tirar onda.\nSe fizer um bomb lá você vai ser respeitado.");
    nineNPC->dialogs.emplace_back("Mas cuidado, a área é protegida por uns vigilantes aí.");
    nineNPC->dialogs.emplace_back("Mas não liga pra eles não. Nós somos tudo maloqueiro e eles são tudo cuzão.");
    nineNPC->dialogs.emplace_back("E se lembra, a única regra aqui é não seguir regras!!");

    hud = new HUD();

}

Stage2State::~Stage2State() {
    AmbientManager::GetInstance().StopAll();
}


void Stage2State::LoadAssets() {}

void Stage2State::Update(float dtt) {
    if (GameData::ended and endTimer->Get() > 5 or Character::player == nullptr) {
        Game& game = Game::GetInstance();
        EndState* endState = new EndState();
        game.Push(endState);
        popRequested = true;
    }
    endTimer->Update(dtt);
    InputManager& inputManager = InputManager::GetInstance();
    KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
    float dt = 0.0156;
    UpdateArray(dt);

    if (inputManager.KeyPress(ESCAPE_KEY)) {
        Game& game = Game::GetInstance();
        PauseState* pauseState = new PauseState();
        game.Push(pauseState);
    }
    quitRequested = inputManager.QuitRequested();

    if (inputManager.KeyPress(SDLK_l)) {
        GameObject* zombieObject = new GameObject(false);
        AddObject(zombieObject);
        Zombie* zmb = new Zombie(*zombieObject, 100);
        zombieObject->box.X = inputManager.GetMouseX() + Camera::pos.GetX();
        zombieObject->box.Y = inputManager.GetMouseY() + Camera::pos.GetY();
        zombieObject->AddComponent(zmb);
    }

    if (inputManager.KeyPress(SDLK_c)) {
        Collider::showCollision = !Collider::showCollision;
    }

    if (keybinder.IsActionPressed(KeyBindingManager::GameAction::NOTEBOOK)) {
        Game& game = Game::GetInstance();
        NotebookState* cadernoState = new NotebookState();
        game.Push(cadernoState);
    }

    Camera::Update(dt);
    if (Character::player != nullptr) {
        hud->Update(dt);
        AmbientManager::GetInstance().Update(Character::player->GetPosition());
    }
    //MusicManager::GetInstance().Update(dt);

}

bool YZ_Sort2(GameObject* a, GameObject* b) {
    if (a->box.Z == b->box.Z)
        return a->box.Y < b->box.Y;
    return a->box.Z < b->box.Z;
}

void Stage2State::Render() {
    TileMap* tileMap = (TileMap*)tileMapObject->GetComponent("TileMap");
    tileMap->RenderLayer(8); // Camada
    tileMap->RenderLayer(7); // Camada
    tileMap->RenderLayer(6); // Camada
    tileMap->RenderLayer(5); // Camada
    tileMap->RenderLayer(4); // Camada
    tileMap->RenderLayer(3); // Camada
    tileMap->RenderLayer(2); // Camada
    tileMap->RenderLayer(1);  // Camada colisão

    vector<GameObject*> objectsToRender;
    for (const auto& objPtr : objectArray) {
        if (objPtr.get() != tileMapObject) {
            objectsToRender.push_back(objPtr.get());
        }
    }
    sort(objectsToRender.begin(), objectsToRender.end(), YZ_Sort2);
    for (int i = 0; i < objectsToRender.size(); i++) {
        objectsToRender[i]->Render();
    }

    tileMap->RenderLayer(0); // Camada

    hud->Render();
}

void Stage2State::Start() {
    StartArray();
    backgroundMusic.UpdateVolume();
    backgroundMusic.Play();
    GameData::playerVictory = false;
    GameData::ended = false;
}

void Stage2State::Pause() {
    backgroundMusic.Pause();
    AmbientManager::GetInstance().PauseAll();
}
void Stage2State::Resume() {
    backgroundMusic.UpdateVolume();
    backgroundMusic.Resume();
    AmbientManager::GetInstance().ResumeAll();
}

GameObject* Stage2State::GetTileMapObject() {
    return tileMapObject;
}

void Stage2State::End(GameObject* playerObject) {
    playerObject->box.X = 7232;
    playerObject->box.Y = 2368 - playerObject->box.H;
    GameData::ended = true;
    GameData::playerVictory = true;
    GameData::endTime = hud->GetLevelTimer();
    GameData::minikits = Character::player->minikits;
    GameData::sprayCans = Character::player->inventory.size();
    endTimer->Restart();

    Sound* graffitiSound = new Sound("../Recursos/audio/GRAFFITI/SPRAY 1.wav");
    graffitiSound->Play(1);

    bool hasRed = false;
    bool hasGreen = false;
    bool hasBlue = false;
    for (int i=0; i < Character::player->inventory.size(); i++) {
        switch (Character::player->inventory[i].type) {
            case ItemData::Type::SprayColor_RED:
                hasRed = true;
            break;
            case ItemData::Type::SprayColor_GREEN:
                hasGreen = true;
            break;
            case ItemData::Type::SprayColor_BLUE:
                hasBlue = true;
            break;
            default:
                break;
        }
    }

    string file = "";
    if (hasRed && hasGreen && hasBlue) {
        file = "../Recursos/img/Graffiti final level 1_3_scaled.png";
    }
    else if (hasRed && hasGreen) {
        file = "../Recursos/img/Graffiti final level 1_2_scaled.png";
    }
    else if (hasRed && hasBlue) {
        file = "../Recursos/img/Graffiti final level 1_2_scaled.png";
    }
    else if (hasGreen && hasBlue) {
        file = "../Recursos/img/Graffiti final level 1_2_scaled.png";
    }
    else if (hasRed) {
        file = "../Recursos/img/Graffiti final level 1_1_scaled.png";
    }
    else if (hasGreen) {
        file = "../Recursos/img/Graffiti final level 1_1_scaled.png";
    }
    else if (hasBlue) {
        file = "../Recursos/img/Graffiti final level 1_1_scaled.png";
    }
    else {
        file = "../Recursos/img/Graffiti final level 1_1_scaled.png";
    }

    GameObject* graffitiObj = new GameObject(true);
    SpriteRenderer* graffiti = new SpriteRenderer(*graffitiObj, file);
    graffitiObj->AddComponent(graffiti);
    //graffiti->SetScale(0.3f, 0.3f);
    cerr << graffitiObj->box.W << endl;
    cerr << graffitiObj->box.H << endl;
    graffitiObj->box.Z = 0;
    graffitiObj->box.X = playerObject->box.X + playerObject->box.W/2 - graffitiObj->box.W/2;
    graffitiObj->box.Y = playerObject->box.Y - graffitiObj->box.H;
    AddObject(graffitiObj);
}
