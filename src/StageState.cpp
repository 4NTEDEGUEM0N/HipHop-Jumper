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


StageState::StageState() {
    GameObject* bgObject = new GameObject();
    AddObject(bgObject);
    SpriteRenderer* bg = new SpriteRenderer(*bgObject, "../Recursos/img/Background.png");
    bg->SetCameraFollower(true);
    bgObject->AddComponent(bg);

    bool teste = true;

    if (!teste) {
        tileMapObject = new GameObject();
        AddObject(tileMapObject);
        tileMapObject->box.X = 0;
        tileMapObject->box.Y = 0;
        set <int> solidIDs = {3,4,10,11,14,15,16,17,18,21,22,23,28,29,30};
        tileSet = new TileSet("../Recursos/img/Tileset.png", 64, 64);
        TileMap* tileMap = new TileMap(*tileMapObject, "../Recursos/map/map_vertical.txt", tileSet, solidIDs);
        tileMapObject->AddComponent(tileMap);

        GameObject* playerObject = new GameObject(false);
        playerObject->box.X = 198;
        playerObject->box.Y = 3690;
        AddObject(playerObject);
        Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Player.png");
        Camera::Follow(playerObject);
        playerObject->AddComponent(playerCharacter);
        PlayerController* playerController = new PlayerController(*playerObject);
        playerObject->AddComponent(playerController);

        GameObject* minikitObject = new GameObject(false);
        minikitObject->box.X = 832;
        minikitObject->box.Y = 2816;
        Item* minikit = new Item(*minikitObject, Item::ItemType::Minikit, "../Recursos/img/minikit.png");
        minikitObject->AddComponent(minikit);
        AddObject(minikitObject);

        GameObject* minikitObject2 = new GameObject(false);
        minikitObject2->box.X = 640;
        minikitObject2->box.Y = 256;
        Item* minikit2 = new Item(*minikitObject2, Item::ItemType::Minikit, "../Recursos/img/minikit.png");
        minikitObject2->AddComponent(minikit2);
        AddObject(minikitObject2);

        GameObject* minikitObject3 = new GameObject(false);
        minikitObject3->box.X = 640;
        minikitObject3->box.Y = 2176;
        Item* minikit3 = new Item(*minikitObject3, Item::ItemType::Minikit, "../Recursos/img/minikit.png");
        minikitObject3->AddComponent(minikit3);
        AddObject(minikitObject3);

        GameObject* minikitObject4 = new GameObject(false);
        minikitObject4->box.X = 512;
        minikitObject4->box.Y = 1664;
        Item* minikit4 = new Item(*minikitObject4, Item::ItemType::Minikit, "../Recursos/img/minikit.png");
        minikitObject4->AddComponent(minikit4);
        AddObject(minikitObject4);

        GameObject* minikitObject5 = new GameObject(false);
        minikitObject5->box.X = 256;
        minikitObject5->box.Y = 2752;
        Item* minikit5 = new Item(*minikitObject5, Item::ItemType::Minikit, "../Recursos/img/minikit.png");
        minikitObject5->AddComponent(minikit5);
        AddObject(minikitObject5);



    } else {
        tileMapObject = new GameObject();
        AddObject(tileMapObject);
        tileMapObject->box.X = 0;
        tileMapObject->box.Y = 0;
        set<int> solidIDs = {7};
        tileSet = new TileSet("../Recursos/img/Tileset_teste.png", 64, 64);
        TileMap* tileMap = new TileMap(*tileMapObject, "../Recursos/map/map_test_tileset.txt", tileSet, solidIDs);
        tileMapObject->AddComponent(tileMap);

        GameObject* playerObject = new GameObject(false);
        playerObject->box.X = 448;
        playerObject->box.Y = 652;
        AddObject(playerObject);
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Player.png");
        Character* playerCharacter = new Character(*playerObject, "../Recursos/img/bonequinho.png");
        Camera::Follow(playerObject);
        playerObject->AddComponent(playerCharacter);
        PlayerController* playerController = new PlayerController(*playerObject);
        playerObject->AddComponent(playerController);

        GameObject* minikitObject = new GameObject(false);
        minikitObject->box.X = 896;
        minikitObject->box.Y = 1024;
        Item* minikit = new Item(*minikitObject, Item::ItemType::Minikit, "../Recursos/img/minikit.png");
        minikitObject->AddComponent(minikit);
        AddObject(minikitObject);

        GameObject* zombieObject = new GameObject(false);
        AddObject(zombieObject);
        Zombie* zmb = new Zombie(*zombieObject, 100);
        zombieObject->box.X = 896;
        zombieObject->box.Y = 1024 - 20;
        zombieObject->AddComponent(zmb);

        GameObject* minikitObject2 = new GameObject(false);
        minikitObject2->box.X = 384;
        minikitObject2->box.Y = 896;
        Item* minikit2 = new Item(*minikitObject2, Item::ItemType::Minikit, "../Recursos/img/minikit.png");
        minikitObject2->AddComponent(minikit2);
        AddObject(minikitObject2);

        GameObject* zombieObject2 = new GameObject(false);
        AddObject(zombieObject2);
        Zombie* zmb2 = new Zombie(*zombieObject2, 100);
        zombieObject2->box.X = 384;
        zombieObject2->box.Y = 896 - 20;
        zombieObject2->AddComponent(zmb2);

        GameObject* minikitObject3 = new GameObject(false);
        minikitObject3->box.X = 896;
        minikitObject3->box.Y = 768;
        Item* minikit3 = new Item(*minikitObject3, Item::ItemType::Minikit, "../Recursos/img/minikit.png");
        minikitObject3->AddComponent(minikit3);
        AddObject(minikitObject3);

        GameObject* minikitObject4 = new GameObject(false);
        minikitObject4->box.X = 320;
        minikitObject4->box.Y = 640;
        Item* minikit4 = new Item(*minikitObject4, Item::ItemType::Minikit, "../Recursos/img/minikit.png");
        minikitObject4->AddComponent(minikit4);
        AddObject(minikitObject4);

        GameObject* minikitObject5 = new GameObject(false);
        minikitObject5->box.X = 640;
        minikitObject5->box.Y = 320;
        Item* minikit5 = new Item(*minikitObject5, Item::ItemType::Minikit, "../Recursos/img/minikit.png");
        minikitObject5->AddComponent(minikit5);
        AddObject(minikitObject5);

    }

    //backgroundMusic.Open("../Recursos/audio/BGM.wav");
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

void StageState::Update(float dt) {
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

    if (GameData::ended) {
        Game& game = Game::GetInstance();
        EndState* endState = new EndState();
        game.Push(endState);
        popRequested = true;
    }

    Camera::Update(dt);
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
