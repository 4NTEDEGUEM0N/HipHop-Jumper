#include "../include/Stage3State.hpp"
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

GameObject* CreateMinikit3(Vec2 pos) {
    ItemData minikitData = ItemData::Minikit();

    GameObject* minikitObject = new GameObject(false);
    minikitObject->box.X = pos.X*64;
    minikitObject->box.Y = pos.Y*64;
    Item* minikit = new Item(*minikitObject, minikitData);
    minikitObject->AddComponent(minikit);
    return minikitObject;
}

GameObject* CreateVigilante2(Vec2 pos) {
    GameObject* zombieObject = new GameObject(false);
    Zombie* zmb = new Zombie(*zombieObject, 100);
    zombieObject->box.X = pos.X*64;
    zombieObject->box.Y = pos.Y*64 - zombieObject->box.H;
    zombieObject->AddComponent(zmb);
    return zombieObject;
}

Stage3State::Stage3State() {
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
        1252, 1253, 1254, 1255, 1256, 1257, 1258, 1259, 1260, 1261, 1262, 1263, 1264, 1265, 1266, 1267, 1268, 1269,17, 18, 19, 20, 21, 22,
        23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,60,70,71,72,75,111,112,113,116};
    tileSet = collisionTileSet;
    TileMap* tileMap = new TileMap(*tileMapObject, "../Recursos/map/2_3_fase.txt", TileSets, solidIDs, 1);
    tileMap->TriangleTopLeftIDs = {166, 205, 26, 32, 35, 113};
    tileMap->TriangleTopRightIDs = {134, 206, 215, 36, 112};
    tileMap->TriangleBottomLeftIDs = {246, 27, 33, 116};
    tileMap->TriangleBottomRightIDs = {125, 216, 247, 28, 34, 75};
    tileMapObject->AddComponent(tileMap);

    GameObject* playerObject = new GameObject(false);
    AddObject(playerObject);
    Character* playerCharacter = new Character(*playerObject, "../Recursos/img/sprite_spray_3_scaled.png");
    //53, 58
    playerObject->box.X = 53*64;
    playerObject->box.Y = 58*64 - playerObject->box.H;
    playerObject->box.Z = 1;
    Camera::Follow(playerObject);
    playerObject->AddComponent(playerCharacter);
    PlayerController* playerController = new PlayerController(*playerObject);
    playerObject->AddComponent(playerController);

    backgroundMusic.Open("../Recursos/audio/TRACKS/THEME 3 G-FUNK LOOP 2.wav");
    backgroundMusic.SetIntro("../Recursos/audio/TRACKS/THEME 3 G-FUNK LOOP 1.wav");

    GameObject* doidaNPCObj = new GameObject();
    AddObject(doidaNPCObj);
    NPC* doidaNPC = new NPC(*doidaNPCObj, "../Recursos/img/EsqueciOnomeDela.png", "Nine");
    doidaNPCObj->AddComponent(doidaNPC);
    doidaNPCObj->box.X = 56*64;
    doidaNPCObj->box.Y = 59*64 - doidaNPCObj->box.H;

    GameObject* endObj = new GameObject();
    endObj->box.X = 52*64;
    endObj->box.Y = 7*64;
    endObj->box.W = 64*3;
    endObj->box.H = 64*3;
    AddObject(endObj);
    DetectionZone* endDetectionZone = new DetectionZone(*endObj);
    endObj->AddComponent(endDetectionZone);
    endDetectionZone->SetDetectFunction([this,playerObject]() {
        End(playerObject);
    });

    GameObject* end2Obj = new GameObject();
    end2Obj->box.X = 93*64;
    end2Obj->box.Y = 54*64;
    end2Obj->box.W = 64*3;
    end2Obj->box.H = 64*3;
    //AddObject(end2Obj);
    DetectionZone* end2DetectionZone = new DetectionZone(*end2Obj);
    end2Obj->AddComponent(end2DetectionZone);
    end2DetectionZone->SetDetectFunction([this,playerObject]() {
        End(playerObject);
    });

    AddObject(CreateVigilante2({66,31}));
    AddObject(CreateVigilante2({65,15}));
    AddObject(CreateVigilante2({60,15}));
    AddObject(CreateVigilante2({72,29}));
    AddObject(CreateVigilante2({90,32}));

    AddObject(CreateMinikit3({106,49}));
    AddObject(CreateMinikit3({109,4}));
    AddObject(CreateMinikit3({87,44}));
    AddObject(CreateMinikit3({73,26}));
    AddObject(CreateMinikit3({62,11}));
    AddObject(CreateMinikit3({62,44}));

    ItemData sprayVermelhoData = ItemData::BucketColor_RED();
    ItemData sprayVerdeData = ItemData::BucketColor_GREEN();
    ItemData sprayAzulData = ItemData::BucketColor_BLUE();

    GameObject* redObject = new GameObject(false);
    redObject->box.X = 72*64;
    redObject->box.Y = 28*64;
    Item* red = new Item(*redObject, sprayVermelhoData);
    redObject->AddComponent(red);
    AddObject(redObject);

    GameObject* greenObject = new GameObject(false);
    greenObject->box.X = 90*64;
    greenObject->box.Y = 31*64;
    Item* green = new Item(*greenObject, sprayVerdeData);
    greenObject->AddComponent(green);
    AddObject(greenObject);

    GameObject* blueObject = new GameObject(false);
    blueObject->box.X = 102*64;
    blueObject->box.Y = 5*64;
    Item* blue = new Item(*blueObject, sprayAzulData);
    blueObject->AddComponent(blue);
    AddObject(blueObject);

    hud = new HUD();

}

Stage3State::~Stage3State() {
    AmbientManager::GetInstance().StopAll();
}


void Stage3State::LoadAssets() {}

void Stage3State::Update(float dtt) {
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

bool YZ_Sort3(GameObject* a, GameObject* b) {
    if (a->box.Z == b->box.Z)
        return a->box.Y < b->box.Y;
    return a->box.Z < b->box.Z;
}

void Stage3State::Render() {
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
    sort(objectsToRender.begin(), objectsToRender.end(), YZ_Sort3);
    for (int i = 0; i < objectsToRender.size(); i++) {
        objectsToRender[i]->Render();
    }

    tileMap->RenderLayer(0); // Camada

    hud->Render();
}

void Stage3State::Start() {
    StartArray();
    backgroundMusic.UpdateVolume();
    backgroundMusic.Play();
    GameData::playerVictory = false;
    GameData::ended = false;
}

void Stage3State::Pause() {
    backgroundMusic.Pause();
    AmbientManager::GetInstance().PauseAll();
}
void Stage3State::Resume() {
    backgroundMusic.UpdateVolume();
    backgroundMusic.Resume();
    AmbientManager::GetInstance().ResumeAll();
}

GameObject* Stage3State::GetTileMapObject() {
    return tileMapObject;
}

void Stage3State::End(GameObject* playerObject) {
    playerObject->box.X = 53*64;
    playerObject->box.Y = 10*64 - playerObject->box.H;
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
        file = "../Recursos/img/Graffiti final level 2_4_scaled.png";
    }
    else if (hasRed && hasGreen) {
        file = "../Recursos/img/Graffiti final level 2_3_scaled.png";
    }
    else if (hasRed && hasBlue) {
        file = "../Recursos/img/Graffiti final level 2_3_scaled.png";
    }
    else if (hasGreen && hasBlue) {
        file = "../Recursos/img/Graffiti final level 2_3_scaled.png";
    }
    else if (hasRed) {
        file = "../Recursos/img/Graffiti final level 2_2_scaled.png";
    }
    else if (hasGreen) {
        file = "../Recursos/img/Graffiti final level 2_2_scaled.png";
    }
    else if (hasBlue) {
        file = "../Recursos/img/Graffiti final level 2_2_scaled.png";
    }
    else {
        file = "../Recursos/img/Graffiti final level 2_1_scaled.png";
    }

    GameObject* graffitiObj = new GameObject(true);
    SpriteRenderer* graffiti = new SpriteRenderer(*graffitiObj, file);
    graffitiObj->AddComponent(graffiti);
    //graffiti->SetScale(0.3f, 0.3f);
    graffitiObj->box.Z = 0;
    graffitiObj->box.X = playerObject->box.X + playerObject->box.W/2 - graffitiObj->box.W/2;
    graffitiObj->box.Y = playerObject->box.Y - graffitiObj->box.H + 64;
    AddObject(graffitiObj);
}
