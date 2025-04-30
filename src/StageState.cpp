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


StageState::StageState() {
    GameObject* bgObject = new GameObject();
    AddObject(bgObject);
    SpriteRenderer* bg = new SpriteRenderer(*bgObject, "../Recursos/img/Background.png");
    bg->SetCameraFollower(true);
    bgObject->AddComponent(bg);

    tileMapObject = new GameObject();
    AddObject(tileMapObject);
    tileMapObject->box.X = 0;
    tileMapObject->box.Y = 0;
    tileSet = new TileSet("../Recursos/img/Tileset.png", 64, 64);
    TileMap* tileMap = new TileMap(*tileMapObject, "../Recursos/map/map_vertical.txt", tileSet);
    tileMapObject->AddComponent(tileMap);

    //backgroundMusic.Open("../Recursos/audio/BGM.wav");
    //backgroundMusic.Play();

    GameObject* playerObject = new GameObject(false);
    playerObject->box.X = 198;
    playerObject->box.Y = 3690;
    AddObject(playerObject);
    Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Player.png");
    Camera::Follow(playerObject);
    playerObject->AddComponent(playerCharacter);
    PlayerController* playerController = new PlayerController(*playerObject);
    playerObject->AddComponent(playerController);

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
