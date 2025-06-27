#include "../include/StageState.hpp"
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
#include <algorithm>

#include "../include/AIController.hpp"
#include "../include/EndState.hpp"
#include "../include/TitleState.hpp"
#include "../include/WaveSpawner.hpp"
#include "../include/Game.hpp"
#include "../include/GameData.hpp"
#include "../include/HUD.hpp"
#include "../include/Item.hpp"
#include "../include/NotebookState.hpp"


StageState::StageState() {
    GameObject* bgObject = new GameObject();
    AddObject(bgObject);
    SpriteRenderer* bg = new SpriteRenderer(*bgObject, "../Recursos/img/Background.png");
    bg->SetCameraFollower(true);
    bgObject->AddComponent(bg);

    float scaleX = Game::VirtualScreenWidth  / 1200.0f;
    float scaleY = Game::VirtualScreenHeight / 900.0f;
    bg->SetScale(scaleX, scaleY);
    bgObject->box.X = 0;
    bgObject->box.Y = 0;

    int teste = 3;

    if (teste == 0) {
        tileMapObject = new GameObject();
        AddObject(tileMapObject);
        tileMapObject->box.X = 0;
        tileMapObject->box.Y = 0;
        set <int> solidIDs = {3,4,10,11,14,15,16,17,18,21,22,23,28,29,30};
        tileSet = new TileSet("../Recursos/img/Tileset.png", 64, 64);
        //TileMap* tileMap = new TileMap(*tileMapObject, "../Recursos/map/map_vertical.txt", tileSet, solidIDs);
        //tileMapObject->AddComponent(tileMap);

        GameObject* playerObject = new GameObject(false);
        playerObject->box.X = 198;
        playerObject->box.Y = 3690;
        AddObject(playerObject);
        Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Player.png");
        Camera::Follow(playerObject);
        playerObject->AddComponent(playerCharacter);
        PlayerController* playerController = new PlayerController(*playerObject);
        playerObject->AddComponent(playerController);

        ItemData minikitData = ItemData::Minikit();
        
        GameObject* minikitObject = new GameObject(false);
        minikitObject->box.X = 832;
        minikitObject->box.Y = 2816;
        Item* minikit = new Item(*minikitObject, minikitData, "../Recursos/img/minikit.png");
        minikitObject->AddComponent(minikit);
        AddObject(minikitObject);

        GameObject* minikitObject2 = new GameObject(false);
        minikitObject2->box.X = 640;
        minikitObject2->box.Y = 256;
        Item* minikit2 = new Item(*minikitObject2, minikitData, "../Recursos/img/minikit.png");
        minikitObject2->AddComponent(minikit2);
        AddObject(minikitObject2);

        GameObject* minikitObject3 = new GameObject(false);
        minikitObject3->box.X = 640;
        minikitObject3->box.Y = 2176;
        Item* minikit3 = new Item(*minikitObject3, minikitData, "../Recursos/img/minikit.png");
        minikitObject3->AddComponent(minikit3);
        AddObject(minikitObject3);

        GameObject* minikitObject4 = new GameObject(false);
        minikitObject4->box.X = 512;
        minikitObject4->box.Y = 1664;
        Item* minikit4 = new Item(*minikitObject4, minikitData, "../Recursos/img/minikit.png");
        minikitObject4->AddComponent(minikit4);
        AddObject(minikitObject4);

        GameObject* minikitObject5 = new GameObject(false);
        minikitObject5->box.X = 256;
        minikitObject5->box.Y = 2752;
        Item* minikit5 = new Item(*minikitObject5, minikitData, "../Recursos/img/minikit.png");
        minikitObject5->AddComponent(minikit5);
        AddObject(minikitObject5);



    } else if (teste == 1) {
        tileMapObject = new GameObject();
        AddObject(tileMapObject);
        tileMapObject->box.X = 0;
        tileMapObject->box.Y = 0;
        set<int> solidIDs = {7};
        tileSet = new TileSet("../Recursos/img/Tileset_teste.png", 64, 64);
        //TileMap* tileMap = new TileMap(*tileMapObject, "../Recursos/map/map_test_tileset.txt", tileSet, solidIDs);
        //tileMapObject->AddComponent(tileMap);

        GameObject* playerObject = new GameObject(false);
        AddObject(playerObject);
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Player.png");
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/spray run test.png");
        Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Spritesheet 2.png");
        playerObject->box.X = 1216;
        playerObject->box.Y = 960 - playerObject->box.H;
        Camera::Follow(playerObject);
        playerObject->AddComponent(playerCharacter);
        PlayerController* playerController = new PlayerController(*playerObject);
        playerObject->AddComponent(playerController);
        
        ItemData minikitData = ItemData::Minikit();

        GameObject* minikitObject = new GameObject(false);
        minikitObject->box.X = 1024;
        minikitObject->box.Y = 1024;
        Item* minikit = new Item(*minikitObject, minikitData, "../Recursos/img/minikit.png");
        minikitObject->AddComponent(minikit);
        AddObject(minikitObject);

        GameObject* zombieObject = new GameObject(false);
        AddObject(zombieObject);
        Zombie* zmb = new Zombie(*zombieObject, 100);
        zombieObject->box.X = 1024;
        zombieObject->box.Y = 1024 - zombieObject->box.H;
        zombieObject->AddComponent(zmb);

        GameObject* minikitObject2 = new GameObject(false);
        minikitObject2->box.X = 1536;
        minikitObject2->box.Y = 1152;
        Item* minikit2 = new Item(*minikitObject2, minikitData, "../Recursos/img/minikit.png");
        minikitObject2->AddComponent(minikit2);
        AddObject(minikitObject2);

        GameObject* zombieObject2 = new GameObject(false);
        AddObject(zombieObject2);
        Zombie* zmb2 = new Zombie(*zombieObject2, 100);
        zombieObject2->box.X = 1536;
        zombieObject2->box.Y = 1152 - zombieObject2->box.H;
        zombieObject2->AddComponent(zmb2);

        GameObject* minikitObject3 = new GameObject(false);
        minikitObject3->box.X = 1536;
        minikitObject3->box.Y = 896;
        Item* minikit3 = new Item(*minikitObject3, minikitData, "../Recursos/img/minikit.png");
        minikitObject3->AddComponent(minikit3);
        AddObject(minikitObject3);

        GameObject* minikitObject4 = new GameObject(false);
        minikitObject4->box.X = 960;
        minikitObject4->box.Y = 768;
        Item* minikit4 = new Item(*minikitObject4, minikitData, "../Recursos/img/minikit.png");
        minikitObject4->AddComponent(minikit4);
        AddObject(minikitObject4);

        GameObject* minikitObject5 = new GameObject(false);
        minikitObject5->box.X = 1216;
        minikitObject5->box.Y = 448;
        Item* minikit5 = new Item(*minikitObject5, minikitData, "../Recursos/img/minikit.png");
        minikitObject5->AddComponent(minikit5);
        AddObject(minikitObject5);

    } else if (teste == 2) {
        tileMapObject = new GameObject();
        AddObject(tileMapObject);
        tileMapObject->box.X = 0;
        tileMapObject->box.Y = 0;
        set<int> solidIDs = {0,1,2,3,4,5,6,7};
        tileSet = new TileSet("../Recursos/img/tileset_1fase.png", 64, 64);
        //TileMap* tileMap = new TileMap(*tileMapObject, "../Recursos/map/1fase.txt", tileSet, solidIDs);
        //tileMapObject->AddComponent(tileMap);

        GameObject* playerObject = new GameObject(false);
        AddObject(playerObject);
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Player.png");
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/spray run test.png");
        Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Spritesheet 2.png");
        playerObject->box.X = 1024;
        playerObject->box.Y = 3392 - playerObject->box.H;
        Camera::Follow(playerObject);
        playerObject->AddComponent(playerCharacter);
        PlayerController* playerController = new PlayerController(*playerObject);
        playerObject->AddComponent(playerController);
    } else if (teste == 3) {
        TileSet* frenteTileSet = new TileSet("../Recursos/img/tileset_1fase2_frente.png", 64, 64);
        TileSet* collisionTileSet = new TileSet("../Recursos/img/tileset_1fase2.png", 64, 64);
        TileSet* backgroundTileSet = new TileSet("../Recursos/img/tileset_1fase2.png", 64, 64);
        TileSet* metroTileSet = new TileSet("../Recursos/img/tileset_1fase2_tras.png", 64, 64);

        std::vector<TileSet*> TileSets;
        TileSets.push_back(frenteTileSet);
        TileSets.push_back(collisionTileSet);
        TileSets.push_back(backgroundTileSet);
        TileSets.push_back(metroTileSet);

        tileMapObject = new GameObject();
        AddObject(tileMapObject);
        tileMapObject->box.X = 0;
        tileMapObject->box.Y = 0;
        set<int> solidIDs = {0,1,2,3,4,5,6,7,8,9,10};
        tileSet = collisionTileSet;
        TileMap* tileMap = new TileMap(*tileMapObject, "../Recursos/map/1fase2.txt", TileSets, solidIDs, 1);
        tileMapObject->AddComponent(tileMap);

        GameObject* playerObject = new GameObject(false);
        AddObject(playerObject);
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Player.png");
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/spray run test.png");
        Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Spritesheet 2.png");
        playerObject->box.X = 1024;
        playerObject->box.Y = 3392 - playerObject->box.H;
        Camera::Follow(playerObject);
        playerObject->AddComponent(playerCharacter);
        PlayerController* playerController = new PlayerController(*playerObject);
        playerObject->AddComponent(playerController);
    }

    //backgroundMusic.Open("../Recursos/audio/bgm1.mp3");
    //backgroundMusic.Play();

    //GameObject* waveObject = new GameObject();
    //AddObject(waveObject);
    //WaveSpawner* waveSpawner = new WaveSpawner(*waveObject);
    //waveObject->AddComponent(waveSpawner);

    //GameObject* aiObject = new GameObject();
    //aiObject->box.X = 576 + rand()%(1984 - 576 + 1);
    //aiObject->box.Y = 448 + rand()%(2112 - 448 + 1);
    //AddObject(aiObject);
    //Character* aiCharacter = new Character(*aiObject, "../Recursos/img/NPC.png");
    //aiObject->AddComponent(aiCharacter);
    //AIController* aiController = new AIController(*aiObject);
    //aiObject->AddComponent(aiController);

    hud = new HUD();

}

StageState::~StageState() = default;


void StageState::LoadAssets() {}

void StageState::Update(float dtt) {
    float dt = 0.0156;
    UpdateArray(dt);

    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY)) {
        Game& game = Game::GetInstance();
        TitleState* titleState = new TitleState();
        game.Push(titleState);
        popRequested = true;
    } else {
        quitRequested = InputManager::GetInstance().QuitRequested();
    }

    if (InputManager::GetInstance().KeyPress(SDLK_SPACE)) {
        GameObject* zombieObject = new GameObject(false);
        AddObject(zombieObject);
        Zombie* zmb = new Zombie(*zombieObject, 100);
        zombieObject->box.X = InputManager::GetInstance().GetMouseX() + Camera::pos.GetX();
        zombieObject->box.Y = InputManager::GetInstance().GetMouseY() + Camera::pos.GetY();
        zombieObject->AddComponent(zmb);
    }

    if (InputManager::GetInstance().KeyPress(SDLK_c)) {
        Collider::showCollision = !Collider::showCollision;
    }

    if (InputManager::GetInstance().KeyPress(SDLK_g)) {
        Game& game = Game::GetInstance();
        NotebookState* cadernoState = new NotebookState();
        game.Push(cadernoState);
    }

    if (GameData::ended) {
        Game& game = Game::GetInstance();
        EndState* endState = new EndState();
        game.Push(endState);
        popRequested = true;
    }

    Camera::Update(dt);
    hud->Update(dt);
}


void StageState::Render() {
    RenderArray();
    hud->Render();
}

void StageState::Start() {
    StartArray();
}

void StageState::Pause() {}
void StageState::Resume() {}

GameObject* StageState::GetTileMapObject() {
    return tileMapObject;
}
