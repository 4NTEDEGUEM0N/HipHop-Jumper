#include "../include/StageTutorialState.hpp"
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

StageTutorialState::StageTutorialState() {
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


    TileSet* collisionTileSet = new TileSet("../Recursos/img/1fase/assets juntos primeira fase.png", 64, 64);
    TileSet* fundoTileSet = new TileSet("../Recursos/img/bloco.png", 64, 64);

    std::vector<TileSet*> TileSets;

    TileSets.push_back(collisionTileSet);
    TileSets.push_back(fundoTileSet);

    tileMapObject = new GameObject();
    AddObject(tileMapObject);
    tileMapObject->box.X = 0;
    tileMapObject->box.Y = 0;
    set<int> solidIDs = {0,1,2,3,4,5,6,7,8,9,10,12,13,14,15,16,18,19};
    tileSet = collisionTileSet;
    TileMap* tileMap = new TileMap(*tileMapObject, "../Recursos/map/tutorial.txt", TileSets, solidIDs, 0);
    tileMapObject->AddComponent(tileMap);

    GameObject* playerObject = new GameObject(false);
    AddObject(playerObject);
    Character* playerCharacter = new Character(*playerObject, "../Recursos/img/sprite_spray_3_scaled.png");
    playerObject->box.X = 2*64;
    playerObject->box.Y = 9*64 - playerObject->box.H;
    playerObject->box.Z = 1;
    Camera::Follow(playerObject);
    playerObject->AddComponent(playerCharacter);
    PlayerController* playerController = new PlayerController(*playerObject);
    playerObject->AddComponent(playerController);

    GameObject* respawnObj = new GameObject();
    respawnObj->box.X = 0;
    respawnObj->box.Y = tileMap->GetHeight()*tileMap->GetTileSetHeight() + playerObject->box.H;
    respawnObj->box.W = 64*tileMap->GetWidth();
    respawnObj->box.H = 64*1;
    AddObject(respawnObj);
    DetectionZone* respawnDetectionZone = new DetectionZone(*respawnObj);
    respawnObj->AddComponent(respawnDetectionZone);
    respawnDetectionZone->SetDetectFunction([this,playerObject]() {
        playerObject->box.X = 2*64;
        playerObject->box.Y = 9*64 - playerObject->box.H;
    });

    GameObject* zombieObject = new GameObject(false);
    AddObject(zombieObject);
    Zombie* zmb = new Zombie(*zombieObject, 100);
    zombieObject->box.X = 49*64;
    zombieObject->box.Y = 9*64;
    zombieObject->AddComponent(zmb);

    ItemData sprayVermelhoData = ItemData::SprayColor_RED();
    GameObject* redObject = new GameObject(false);
    redObject->box.X = 50*64;
    redObject->box.Y = 10*64;
    Item* red = new Item(*redObject, sprayVermelhoData);
    redObject->AddComponent(red);
    AddObject(redObject);

    GameObject* endObj = new GameObject();
    endObj->box.X = 59*64-10;
    endObj->box.Y = 8*64;
    endObj->box.W = 64*2;
    endObj->box.H = 64*3;
    AddObject(endObj);
    DetectionZone* endDetectionZone = new DetectionZone(*endObj);
    endObj->AddComponent(endDetectionZone);
    endDetectionZone->SetDetectFunction([this,playerObject]() {
        Game& game = Game::GetInstance();
        TitleState* tileState = new TitleState();
        game.Push(tileState);
        game.SuperPop();
    });


    KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
    string movimentos = "Movimentação:\n\n"
        "Esquerda -> Pressione " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::MOVE_LEFT))) + "\n"
        "Direita -> Pressione " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::MOVE_RIGHT)));

    GameObject* movimentosObj = new GameObject();
    AddObject(movimentosObj);
    Text* movimentosText = new Text(*movimentosObj,"../Recursos/font/KGPerfectPenmanship.ttf", 24, Text::SOLID, movimentos, {0, 0, 0, 255});
    movimentosObj->AddComponent(movimentosText);
    movimentosObj->box.X = 1*64;
    movimentosObj->box.Y = 6*64;

    string pulo = "Pressione " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::JUMP))) + " para pular!";

    GameObject* puloObj = new GameObject();
    AddObject(puloObj);
    Text* puloText = new Text(*puloObj,"../Recursos/font/KGPerfectPenmanship.ttf", 24, Text::SOLID, pulo, {0, 0, 0, 255});
    puloObj->AddComponent(puloText);
    puloObj->box.X = 7*64 + 32;
    puloObj->box.Y = 6*64;

    string pulo_duplo = "Pule outra vez no ar\n"
                        "para dar um pulo duplo!";

    GameObject* puloDuploObj = new GameObject();
    AddObject(puloDuploObj);
    Text* puloDuploText = new Text(*puloDuploObj,"../Recursos/font/KGPerfectPenmanship.ttf", 24, Text::SOLID, pulo_duplo, {0, 0, 0, 255});
    puloDuploObj->AddComponent(puloDuploText);
    puloDuploObj->box.X = 13*64 - 10;
    puloDuploObj->box.Y = 8*64;

    string dash = "Utilize o dash (" + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::DASH))) + " + Direção)\n"
                    "para superar grandes distâncias!";

    GameObject* dashObj = new GameObject();
    AddObject(dashObj);
    Text* dashText = new Text(*dashObj,"../Recursos/font/KGPerfectPenmanship.ttf", 24, Text::SOLID, dash, {0, 0, 0, 255});
    dashObj->AddComponent(dashText);
    dashObj->box.X = 17*64;
    dashObj->box.Y = 3*64;

    string wall_jump = "Pule em direção a parade para agarra-la.\n"
                    "Pule novamente para realizar um wall jump!";

    GameObject* wallJumpObj = new GameObject();
    AddObject(wallJumpObj);
    Text* wallJumpText = new Text(*wallJumpObj,"../Recursos/font/KGPerfectPenmanship.ttf", 24, Text::SOLID, wall_jump, {0, 0, 0, 255});
    wallJumpObj->AddComponent(wallJumpText);
    wallJumpObj->box.X = 29*64 - 25;
    wallJumpObj->box.Y = 3*64;

    string caderno = "Aperte " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::NOTEBOOK))) + " para abrir o seu caderno.\n"
                    "Nele você encontra informações úteis!";

    GameObject* cadernoObj = new GameObject();
    AddObject(cadernoObj);
    Text* cadernoText = new Text(*cadernoObj,"../Recursos/font/KGPerfectPenmanship.ttf", 24, Text::SOLID, caderno, {0, 0, 0, 255});
    cadernoObj->AddComponent(cadernoText);
    cadernoObj->box.X = 37*64 +5;
    cadernoObj->box.Y = 3*64;

    string cuidado_vigilante = "Cuidado com o vigilante!.\n\n"
                    "Ao encostar nele você perde vida\n"
                    "e é empurrado para longe!";

    GameObject* cuidadoVigilanteObj = new GameObject();
    AddObject(cuidadoVigilanteObj);
    Text* cuidadoVigilanteText = new Text(*cuidadoVigilanteObj,"../Recursos/font/KGPerfectPenmanship.ttf", 24, Text::SOLID, cuidado_vigilante, {0, 0, 0, 255});
    cuidadoVigilanteObj->AddComponent(cuidadoVigilanteText);
    cuidadoVigilanteObj->box.X = 41*64 +5;
    cuidadoVigilanteObj->box.Y = 6*64;

    string stun_vigilante = "Aperte " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::STUNGLEE))) + " próximo vigilante para\n"
                    "jogar um spray na cara dele!";

    GameObject* stunVigilanteObj = new GameObject();
    AddObject(stunVigilanteObj);
    Text* stunVigilanteText = new Text(*stunVigilanteObj,"../Recursos/font/KGPerfectPenmanship.ttf", 24, Text::SOLID, stun_vigilante, {0, 0, 0, 255});
    stunVigilanteObj->AddComponent(stunVigilanteText);
    stunVigilanteObj->box.X = 48*64;
    stunVigilanteObj->box.Y = 7*64;

    string parabens = "Boa! Você está pronto!\n"
                    "Siga em frente ->";

    GameObject* parabensObj = new GameObject();
    AddObject(parabensObj);
    Text* parabensText = new Text(*parabensObj,"../Recursos/font/KGPerfectPenmanship.ttf", 24, Text::SOLID, parabens, {0, 0, 0, 255});
    parabensObj->AddComponent(parabensText);
    parabensObj->box.X = 56*64;
    parabensObj->box.Y = 8*64;

    hud = new HUD();

}

StageTutorialState::~StageTutorialState() {
    AmbientManager::GetInstance().StopAll();
}


void StageTutorialState::LoadAssets() {}

void StageTutorialState::Update(float dtt) {
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

void StageTutorialState::Render() {
    RenderArray();
    hud->Render();
}

void StageTutorialState::Start() {
    StartArray();
    backgroundMusic.UpdateVolume();
    backgroundMusic.Play();
    GameData::playerVictory = false;
    GameData::ended = false;
}

void StageTutorialState::Pause() {
    backgroundMusic.Pause();
    AmbientManager::GetInstance().PauseAll();
}
void StageTutorialState::Resume() {
    backgroundMusic.UpdateVolume();
    backgroundMusic.Resume();
    AmbientManager::GetInstance().ResumeAll();
}

GameObject* StageTutorialState::GetTileMapObject() {
    return tileMapObject;
}

void StageTutorialState::End(GameObject* playerObject) {
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
        file = "../Recursos/img/vermelho_verde_azul.png";
    }
    else if (hasRed && hasGreen) {
        file = "../Recursos/img/vermelho_verde.png";
    }
    else if (hasRed && hasBlue) {
        file = "../Recursos/img/vermelho_azul.png";
    }
    else if (hasGreen && hasBlue) {
        file = "../Recursos/img/verde_azul.png";
    }
    else if (hasRed) {
        file = "../Recursos/img/vermelho.png";
    }
    else if (hasGreen) {
        file = "../Recursos/img/verde.png";
    }
    else if (hasBlue) {
        file = "../Recursos/img/azul.png";
    }
    else {
        file = "../Recursos/img/nenhum.png";
    }

    GameObject* graffitiObj = new GameObject(true);
    SpriteRenderer* graffiti = new SpriteRenderer(*graffitiObj, file);
    graffitiObj->AddComponent(graffiti);
    graffiti->SetScale(0.3f, 0.3f);
    graffitiObj->box.X = playerObject->box.X + playerObject->box.W/2 - graffitiObj->box.W/2;
    graffitiObj->box.Y = playerObject->box.Y - graffitiObj->box.H;
    AddObject(graffitiObj);
}
