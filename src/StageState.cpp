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


GameObject* CreateMinikit(Vec2 pos) {
    ItemData minikitData = ItemData::Minikit();

    GameObject* minikitObject = new GameObject(false);
    minikitObject->box.X = pos.X*64;
    minikitObject->box.Y = pos.Y*64;
    Item* minikit = new Item(*minikitObject, minikitData);
    minikitObject->AddComponent(minikit);
    return minikitObject;
}

StageState::StageState() {
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

    int teste = 4;

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
        Item* minikit = new Item(*minikitObject, minikitData);
        minikitObject->AddComponent(minikit);
        AddObject(minikitObject);

        GameObject* minikitObject2 = new GameObject(false);
        minikitObject2->box.X = 640;
        minikitObject2->box.Y = 256;
        Item* minikit2 = new Item(*minikitObject2, minikitData);
        minikitObject2->AddComponent(minikit2);
        AddObject(minikitObject2);

        GameObject* minikitObject3 = new GameObject(false);
        minikitObject3->box.X = 640;
        minikitObject3->box.Y = 2176;
        Item* minikit3 = new Item(*minikitObject3, minikitData);
        minikitObject3->AddComponent(minikit3);
        AddObject(minikitObject3);

        GameObject* minikitObject4 = new GameObject(false);
        minikitObject4->box.X = 512;
        minikitObject4->box.Y = 1664;
        Item* minikit4 = new Item(*minikitObject4, minikitData);
        minikitObject4->AddComponent(minikit4);
        AddObject(minikitObject4);

        GameObject* minikitObject5 = new GameObject(false);
        minikitObject5->box.X = 256;
        minikitObject5->box.Y = 2752;
        Item* minikit5 = new Item(*minikitObject5, minikitData);
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
        Item* minikit = new Item(*minikitObject, minikitData);
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
        Item* minikit2 = new Item(*minikitObject2, minikitData);
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
        Item* minikit3 = new Item(*minikitObject3, minikitData);
        minikitObject3->AddComponent(minikit3);
        AddObject(minikitObject3);

        GameObject* minikitObject4 = new GameObject(false);
        minikitObject4->box.X = 960;
        minikitObject4->box.Y = 768;
        Item* minikit4 = new Item(*minikitObject4, minikitData);
        minikitObject4->AddComponent(minikit4);
        AddObject(minikitObject4);

        GameObject* minikitObject5 = new GameObject(false);
        minikitObject5->box.X = 1216;
        minikitObject5->box.Y = 448;
        Item* minikit5 = new Item(*minikitObject5, minikitData);
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
        TileSet* frenteTileSet = new TileSet("../Recursos/img/tileset_1fase2_frente_scaled.png", 64, 64);
        TileSet* collisionTileSet = new TileSet("../Recursos/img/tileset_1fase2.png", 64, 64);
        TileSet* backgroundTileSet = new TileSet("../Recursos/img/tileset_1fase2.png", 64, 64);
        TileSet* metroTileSet = new TileSet("../Recursos/img/tileset_1fase2_tras_scaled.png", 64, 64);

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
        TileMap* tileMap = new TileMap(*tileMapObject, "../Recursos/map/1fase2_scaled.txt", TileSets, solidIDs, 1);
        tileMapObject->AddComponent(tileMap);

        GameObject* playerObject = new GameObject(false);
        AddObject(playerObject);
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Player.png");
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/spray run test.png");
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Sprite Spray2_scaled.png");
        Character* playerCharacter = new Character(*playerObject, "../Recursos/img/sprite_spray_3_scaled.png");
        playerObject->box.X = 1024;
        playerObject->box.Y = 3392 - playerObject->box.H;
        Camera::Follow(playerObject);
        playerObject->AddComponent(playerCharacter);
        PlayerController* playerController = new PlayerController(*playerObject);
        playerObject->AddComponent(playerController);

        GameObject* ratoNPCObj = new GameObject();
        AddObject(ratoNPCObj);
        NPC* ratoNPC = new NPC(*ratoNPCObj, "../Recursos/img/RatoriousSentadoIdleScaled.png", "RATORIOUS B.I.G.", 4, 4);
        
        Animator* animator = new Animator(*ratoNPCObj);
        ratoNPCObj->AddComponent(animator);
        animator->AddAnimation("idle", Animation(0, 13, 0.1));
        animator->AddAnimation("standing", Animation(14, 14, 0));
        animator->SetAnimation("idle");
        
        ratoNPCObj->AddComponent(ratoNPC);
        ratoNPCObj->box.X = 2496;
        ratoNPCObj->box.Y = 3848 - ratoNPCObj->box.H;
        Collider* npcCollider = new Collider(*ratoNPCObj, {4,3});
        ratoNPCObj->AddComponent(npcCollider);

        ratoNPC->dialogs.emplace_back("Olá! Eu sou um rato maluco!");
        ratoNPC->dialogs.emplace_back("E você é muito besta.");
        ratoNPC->dialogs.emplace_back("Hum... Quem é você moleque? Espera, pela aparência... Spray...");
        ratoNPC->dialogs.emplace_back("Me disseram que você apareceria aqui para entrar na Crew.");
        ratoNPC->dialogs.emplace_back("Mas caramba, você é mais feio do que eu imaginava...");
        ratoNPC->dialogs.emplace_back("Enfim moleque, pra graffitar nessa área tem que ser bom de verdade, não \nqueremos qualquer um na nossa Crew.");
        ratoNPC->dialogs.emplace_back("O teste vai ser você fazer o graffiti mais ousado do metrô, ali pra trás o \ndesafio é grande. Duvido que vá ser bom o suficiente.");
        ratoNPC->dialogs.emplace_back("Aé, meu vulgo é Ratorious B.I.G., o maior graffiteiro da região, pode crer?");
        ratoNPC->dialogs.emplace_back("Caso passe no teste, vai encontrar com o Nine lá em cima. O moleque é firmeza,\nmas também é metido");
        ratoNPC->dialogs.emplace_back("Boa sorte!");

        
        backgroundMusic.Open("../Recursos/audio/TRACKS/CLOUD TRAP METRO LOOP.wav");
        backgroundMusic.Play();
        
        GameObject* detectionObj = new GameObject();
        detectionObj->box.X = 5056;//64;
        detectionObj->box.Y = 1088;//3712;
        detectionObj->box.W = 64;
        detectionObj->box.H = 64;
        AddObject(detectionObj);
        DetectionZone* detectionZone = new DetectionZone(*detectionObj);
        detectionObj->AddComponent(detectionZone);
        detectionZone->SetDetectFunction([this,playerObject]() {
            End(playerObject);
        });
    } else if (teste == 4) {
        TileSet* metroFrenteTileSet = new TileSet("../Recursos/img/1fase/cenario metro123_scaled.png", 64, 64);
        TileSet* finalFrenteTileSet = new TileSet("../Recursos/img/1fase/cenario 2 frente_scaled.png", 64, 64);
        TileSet* collisionTileSet = new TileSet("../Recursos/img/1fase/assets juntos primeira fase.png", 64, 64);
        TileSet* finalTrasTileSet = new TileSet("../Recursos/img/1fase/cenario 2 trás_scaled.png", 64, 64);
        TileSet* metroTrasTileSet = new TileSet("../Recursos/img/1fase/cenario metro trás_scaled.png", 64, 64);
        TileSet* detalhes1TileSet = new TileSet("../Recursos/img/1fase/Sprite sheets detalhes.png", 64, 64);
        TileSet* detalhes2TileSet = new TileSet("../Recursos/img/1fase/Sprite sheets detalhes.png", 64, 64);
        TileSet* detalhes3TileSet = new TileSet("../Recursos/img/1fase/tubo extensão sprite.png", 64, 64);
        TileSet* texturaFundoTileSet = new TileSet("../Recursos/img/1fase/tijolos.png", 64, 64);
        TileSet* fundoTileSet = new TileSet("../Recursos/img/1fase/tijolos.png", 64, 64);

        std::vector<TileSet*> TileSets;
        TileSets.push_back(metroFrenteTileSet);
        TileSets.push_back(finalFrenteTileSet);
        TileSets.push_back(collisionTileSet);
        TileSets.push_back(finalTrasTileSet);
        TileSets.push_back(metroTrasTileSet);
        TileSets.push_back(detalhes1TileSet);
        TileSets.push_back(detalhes2TileSet);
        TileSets.push_back(detalhes3TileSet);
        TileSets.push_back(texturaFundoTileSet);
        TileSets.push_back(fundoTileSet);

        tileMapObject = new GameObject();
        AddObject(tileMapObject);
        tileMapObject->box.X = 0;
        tileMapObject->box.Y = 0;
        set<int> solidIDs = {0,1,2,3,4,5,6,7,8,9,10,12,13,14,15,16,18,19};
        tileSet = collisionTileSet;
        TileMap* tileMap = new TileMap(*tileMapObject, "../Recursos/map/1fase_final_scaled.txt", TileSets, solidIDs, 2);
        tileMap->TriangleTopLeftIDs = {15};
        tileMap->TriangleTopRightIDs = {16};
        tileMap->TriangleBottomLeftIDs = {18};
        tileMap->TriangleBottomRightIDs = {19};
        tileMapObject->AddComponent(tileMap);

        GameObject* playerObject = new GameObject(false);
        AddObject(playerObject);
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Player.png");
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/spray run test.png");
        //Character* playerCharacter = new Character(*playerObject, "../Recursos/img/Sprite Spray2_scaled.png");
        Character* playerCharacter = new Character(*playerObject, "../Recursos/img/sprite_spray_3_scaled.png");
        /*Cordenadas úteis:
         * Início: 448, 8512
         * Fim: 5760, 1536
         * Super canto direito: 7936, 384
        */
        playerObject->box.X = 448;
        playerObject->box.Y = 8512 - playerObject->box.H;
        //playerObject->box.X = 448;
        //playerObject->box.Y = 8512 - playerObject->box.H;
        playerObject->box.Z = 1;
        Camera::Follow(playerObject);
        playerObject->AddComponent(playerCharacter);
        PlayerController* playerController = new PlayerController(*playerObject);
        playerObject->AddComponent(playerController);

        GameObject* ratoNPCObj = new GameObject();
        AddObject(ratoNPCObj);
        NPC* ratoNPC = new NPC(*ratoNPCObj, "../Recursos/img/RatoriousSentadoIdleScaled.png", "RATORIOUS B.I.G.", 4, 4);
        
        Animator* animator = new Animator(*ratoNPCObj);
        ratoNPCObj->AddComponent(animator);
        animator->AddAnimation("idle", Animation(0, 13, 0.1));
        animator->AddAnimation("standing", Animation(14, 14, 0));
        animator->SetAnimation("idle");
        
        ratoNPCObj->AddComponent(ratoNPC);
        ratoNPCObj->box.X = 3008;
        ratoNPCObj->box.Y = 8572 - ratoNPCObj->box.H;
        Collider* npcCollider = new Collider(*ratoNPCObj, {2,2});
        ratoNPCObj->AddComponent(npcCollider);

        ratoNPC->dialogs.emplace_back("Hum... Quem é você moleque? Espera, pela aparência... Spray...");
        ratoNPC->dialogs.emplace_back("Me disseram que você apareceria aqui para entrar na Crew.");
        ratoNPC->dialogs.emplace_back("Mas caramba, você é mais feio do que eu imaginava...");
        ratoNPC->dialogs.emplace_back("Enfim moleque, pra graffitar nessa área tem que ser bom de verdade, não \nqueremos qualquer um na nossa Crew.");
        ratoNPC->dialogs.emplace_back("O teste vai ser você fazer o graffiti mais ousado do metrô, ali pra trás o \ndesafio é grande. Duvido que vá ser bom o suficiente.");
        ratoNPC->dialogs.emplace_back("Aé, meu vulgo é Ratorious B.I.G., o maior graffiteiro da região, pode crer?");
        ratoNPC->dialogs.emplace_back("Caso passe no teste, vai encontrar com o Nine lá em cima. \nO moleque é firmeza, mas também é metido");
        ratoNPC->dialogs.emplace_back("Boa sorte!");

        backgroundMusic.Open("../Recursos/audio/TRACKS/CLOUD TRAP METRO LOOP.wav");
        backgroundMusic.SetIntro("../Recursos/audio/TRACKS/CLOUD TRAP METRO START.wav");

        GameObject* detectionObj = new GameObject();
        detectionObj->box.X = 7168;
        detectionObj->box.Y = 2176;
        detectionObj->box.W = 64*3;
        detectionObj->box.H = 64*3;
        AddObject(detectionObj);
        DetectionZone* detectionZone = new DetectionZone(*detectionObj);
        detectionObj->AddComponent(detectionZone);
        detectionZone->SetDetectFunction([this,playerObject]() {
            End(playerObject);
        });

        GameObject* detectionObj2 = new GameObject();
        detectionObj2->box.X = 192;
        detectionObj2->box.Y = 8448;
        detectionObj2->box.W = 64*2;
        detectionObj2->box.H = 64*2;
        AddObject(detectionObj2);
        DetectionZone* detectionZone2 = new DetectionZone(*detectionObj2);
        detectionObj2->AddComponent(detectionZone2);
        detectionZone2->SetDetectFunction([this,playerObject]() {
            End(playerObject);
        });

        AddObject(CreateMinikit({2,133}));
        AddObject(CreateMinikit({69,132}));
        AddObject(CreateMinikit({78,102}));
        AddObject(CreateMinikit({133,2}));
        AddObject(CreateMinikit({100,44}));
        AddObject(CreateMinikit({64,36}));

        ItemData sprayVermelhoData = ItemData::SprayColor_RED();
        ItemData sprayVerdeData = ItemData::SprayColor_GREEN();
        ItemData sprayAzulData = ItemData::SprayColor_BLUE();

        GameObject* redObject = new GameObject(false);
        redObject->box.X = 13*64;
        redObject->box.Y = 132*64;
        //redObject->box.X = 92*64;
        //redObject->box.Y = 51*64;
        Item* red = new Item(*redObject, sprayVermelhoData);
        redObject->AddComponent(red);
        AddObject(redObject);

        GameObject* greenObject = new GameObject(false);
        greenObject->box.X = 14*64;
        greenObject->box.Y = 132*64;
        //greenObject->box.X = 84*64;
        //greenObject->box.Y = 107*64;
        Item* green = new Item(*greenObject, sprayVerdeData);
        greenObject->AddComponent(green);
        AddObject(greenObject);

        GameObject* blueObject = new GameObject(false);
        blueObject->box.X = 127*64;
        blueObject->box.Y = 5*64;
        Item* blue = new Item(*blueObject, sprayAzulData);
        blueObject->AddComponent(blue);
        AddObject(blueObject);

        /*
        MusicManager::GetInstance().PlayMusic(
            "../Recursos/audio/TRACKS/CLOUD TRAP METRO START.wav",
            "../Recursos/audio/TRACKS/CLOUD TRAP METRO LOOP.wav",
            ""
        );
        */
        
        
        GameObject* dropObject = new GameObject(false);
        dropObject->box.X = 53 * 64 + 10;
        dropObject->box.Y = 130 * 64 + 5;
        SpriteRenderer* spriteDrop = new SpriteRenderer(*dropObject, "../Recursos/img/thedrop.png", 6, 6);
        dropObject->AddComponent(spriteDrop);
        AddObject(dropObject);
        
        Animator* animatorDrop = new Animator(*dropObject);
        animatorDrop->AddAnimation("idle", Animation(31, 31, 0));
        animatorDrop->AddAnimation("drop", Animation(0, 31, 0.10));
        animatorDrop->SetAnimation("idle");
        dropObject->AddComponent(animatorDrop);
        
        animatorDrop->SetFrameCallback("drop", 8, []() {
            Sound* dropSound = new Sound("../Recursos/audio/SINGLEDROP.wav");
            dropSound->Play(1);
        });
        
        GameObject* detectionObjDrop = new GameObject();
        detectionObjDrop->box.X = 38 * 64;
        detectionObjDrop->box.Y = 124 * 64;
        detectionObjDrop->box.W = 30 * 64;
        detectionObjDrop->box.H = 10 * 64;
        AddObject(detectionObjDrop);
        DetectionZone* detectionZoneDrop = new DetectionZone(*detectionObjDrop);
        detectionObjDrop->AddComponent(detectionZoneDrop);
        
        detectionZoneDrop->SetDetectFunction([animatorDrop]() {
            animatorDrop->SetAnimation("drop");
        });
        detectionZoneDrop->SetNoneFunction([animatorDrop]() {
            animatorDrop->SetAnimation("idle");
        });
        
        
        
        
        Sound* lampSound = new Sound("../Recursos/audio/AMBIENCE/LIGHT BUZZING.wav");
        
        GameObject* lampObject = new GameObject(false);
        lampObject->box.Z = 2;
        lampObject->box.X = 11 * 64;
        lampObject->box.Y = 130 * 64 + 12;
        SpriteRenderer* spriteLamp = new SpriteRenderer(*lampObject, "../Recursos/img/luz.png", 2, 1);
        lampObject->AddComponent(spriteLamp);
        AddObject(lampObject);
        
        Animator* animatorLamp = new Animator(*lampObject);
        animatorLamp->AddAnimation("off", Animation(1, 1, 0));
        animatorLamp->AddAnimation("on", Animation(0, 0, 0));
        animatorLamp->SetAnimation("off");
        lampObject->AddComponent(animatorLamp);
        
        GameObject* detectionObjLamp = new GameObject();
        detectionObjLamp->box.X = 11 * 64;
        detectionObjLamp->box.Y = 130 * 64;
        detectionObjLamp->box.W = 5 * 64;
        detectionObjLamp->box.H = 5 * 64;
        AddObject(detectionObjLamp);
        DetectionZone* detectionZoneLamp = new DetectionZone(*detectionObjLamp);
        detectionObjLamp->AddComponent(detectionZoneLamp);
        
        detectionZoneLamp->SetDetectFunction([animatorLamp, lampSound]() {
            animatorLamp->SetAnimation("on");
            lampSound->Play(1);
        });
        detectionZoneLamp->SetNoneFunction([animatorLamp]() {
            animatorLamp->SetAnimation("off");
        });
        
        
        GameObject* lampObject2 = new GameObject(false);
        lampObject2->box.Z = 2;
        lampObject2->box.X = 94 * 64 - 30;
        lampObject2->box.Y = 32 * 64 + 12;
        SpriteRenderer* spriteLamp2 = new SpriteRenderer(*lampObject2, "../Recursos/img/luz.png", 2, 1);
        lampObject2->AddComponent(spriteLamp2);
        AddObject(lampObject2);
        
        Animator* animatorLamp2 = new Animator(*lampObject2);
        animatorLamp2->AddAnimation("off", Animation(1, 1, 0));
        animatorLamp2->AddAnimation("on", Animation(0, 0, 0));
        animatorLamp2->SetAnimation("off");
        lampObject2->AddComponent(animatorLamp2);
        
        GameObject* detectionObjLamp2 = new GameObject();
        detectionObjLamp2->box.X = 94 * 64 - 32;
        detectionObjLamp2->box.Y = 32 * 64;
        detectionObjLamp2->box.W = 5 * 64;
        detectionObjLamp2->box.H = 5 * 64;
        AddObject(detectionObjLamp2);
        DetectionZone* detectionZoneLamp2 = new DetectionZone(*detectionObjLamp2);
        detectionObjLamp2->AddComponent(detectionZoneLamp2);
        
        detectionZoneLamp2->SetDetectFunction([animatorLamp2, lampSound]() {
            animatorLamp2->SetAnimation("on");
            lampSound->Play(1);
        });
        detectionZoneLamp2->SetNoneFunction([animatorLamp2]() {
            animatorLamp2->SetAnimation("off");
        });
        
        GameObject* lampObject3 = new GameObject(false);
        lampObject3->box.Z = 2;
        lampObject3->box.X = 103 * 64 - 36;
        lampObject3->box.Y = 32 * 64 + 12;
        SpriteRenderer* spriteLamp3 = new SpriteRenderer(*lampObject3, "../Recursos/img/luz.png", 2, 1);
        lampObject3->AddComponent(spriteLamp3);
        AddObject(lampObject3);

        Animator* animatorLamp3 = new Animator(*lampObject3);
        animatorLamp3->AddAnimation("off", Animation(1, 1, 0));
        animatorLamp3->AddAnimation("on", Animation(0, 0, 0));
        animatorLamp3->SetAnimation("off");
        lampObject3->AddComponent(animatorLamp3);

        GameObject* detectionObjLamp3 = new GameObject();
        detectionObjLamp3->box.X = 103 * 64 - 32;
        detectionObjLamp3->box.Y = 32 * 64;
        detectionObjLamp3->box.W = 5 * 64;
        detectionObjLamp3->box.H = 5 * 64;
        AddObject(detectionObjLamp3);
        DetectionZone* detectionZoneLamp3 = new DetectionZone(*detectionObjLamp3);
        detectionObjLamp3->AddComponent(detectionZoneLamp3);

        detectionZoneLamp3->SetDetectFunction([animatorLamp3, lampSound]() {
            animatorLamp3->SetAnimation("on");
            lampSound->Play(1);
        });
        detectionZoneLamp3->SetNoneFunction([animatorLamp3]() {
            animatorLamp3->SetAnimation("off");
        });

        GameObject* lampObject4 = new GameObject(false);
        lampObject4->box.Z = 2;
        lampObject4->box.X = 112 * 64 - 48;
        lampObject4->box.Y = 32 * 64 + 16;
        SpriteRenderer* spriteLamp4 = new SpriteRenderer(*lampObject4, "../Recursos/img/luz.png", 2, 1);
        lampObject4->AddComponent(spriteLamp4);
        AddObject(lampObject4);

        Animator* animatorLamp4 = new Animator(*lampObject4);
        animatorLamp4->AddAnimation("off", Animation(1, 1, 0));
        animatorLamp4->AddAnimation("on", Animation(0, 0, 0));
        animatorLamp4->SetAnimation("off");
        lampObject4->AddComponent(animatorLamp4);

        GameObject* detectionObjLamp4 = new GameObject();
        detectionObjLamp4->box.X = 112 * 64 - 48;
        detectionObjLamp4->box.Y = 32 * 64;
        detectionObjLamp4->box.W = 5 * 64;
        detectionObjLamp4->box.H = 5 * 64;
        AddObject(detectionObjLamp4);
        DetectionZone* detectionZoneLamp4 = new DetectionZone(*detectionObjLamp4);
        detectionObjLamp4->AddComponent(detectionZoneLamp4);

        detectionZoneLamp4->SetDetectFunction([animatorLamp4, lampSound]() {
            animatorLamp4->SetAnimation("on");
            lampSound->Play(1);
        });
        detectionZoneLamp4->SetNoneFunction([animatorLamp4]() {
            animatorLamp4->SetAnimation("off");
        });
        
        
        GameObject* lampObject5 = new GameObject(false);
        lampObject5->box.Z = 5;
        lampObject5->box.X = 85 * 64;
        lampObject5->box.Y = 33 * 64 - 20;
        lampObject5->angleDeg = 343.0f;

        SpriteRenderer* spriteLamp5 = new SpriteRenderer(*lampObject5, "../Recursos/img/luz.png", 2, 1);
        lampObject5->AddComponent(spriteLamp5);
        AddObject(lampObject5);

        Animator* animatorLamp5 = new Animator(*lampObject5);
        animatorLamp5->AddAnimation("off", Animation(1, 1, 0));
        animatorLamp5->AddAnimation("on", Animation(0, 0, 0));
        animatorLamp5->SetAnimation("off");
        lampObject5->AddComponent(animatorLamp5);

        GameObject* detectionObjLamp5 = new GameObject();
        detectionObjLamp5->box.X = 85 * 64;
        detectionObjLamp5->box.Y = 33 * 64;
        detectionObjLamp5->box.W = 5 * 64;
        detectionObjLamp5->box.H = 5 * 64;
        AddObject(detectionObjLamp5);

        DetectionZone* detectionZoneLamp5 = new DetectionZone(*detectionObjLamp5);
        detectionObjLamp5->AddComponent(detectionZoneLamp5);

        detectionZoneLamp5->SetDetectFunction([animatorLamp5, lampSound]() {
            animatorLamp5->SetAnimation("on");
            lampSound->Play(1);
        });
        detectionZoneLamp5->SetNoneFunction([animatorLamp5]() {
            animatorLamp5->SetAnimation("off");
        });

        
        
//        GameObject* lampObject = new GameObject(false);
//        lampObject->box.Z = 2;
//        lampObject->box.X = 11 * 64;
//        lampObject->box.Y = 130 * 64 + 10;
//        SpriteRenderer* spriteLamp = new SpriteRenderer(*lampObject, "../Recursos/img/luz.png", 2, 1);
//        lampObject->AddComponent(spriteLamp);
//        AddObject(lampObject);
//
//        Animator* animatorLamp = new Animator(*lampObject);
//        animatorLamp->AddAnimation("off", Animation(1, 1, 0));
//        animatorLamp->AddAnimation("on", Animation(0, 0, 0));
//        animatorLamp->SetAnimation("off");
//        lampObject->AddComponent(animatorLamp);
//
//        GameObject* detectionObjLamp = new GameObject();
//        detectionObjLamp->box.X = 11 * 64;
//        detectionObjLamp->box.Y = 130 * 64;
//        detectionObjLamp->box.W = 5 * 64;
//        detectionObjLamp->box.H = 5 * 64;
//        AddObject(detectionObjLamp);
//        DetectionZone* detectionZoneLamp = new DetectionZone(*detectionObjLamp);
//        detectionObjLamp->AddComponent(detectionZoneLamp);
//
//        detectionZoneLamp->SetDetectFunction([animatorLamp, lampSound]() {
//            animatorLamp->SetAnimation("on");
//            lampSound->Play(1);
//        });
//        detectionZoneLamp->SetNoneFunction([animatorLamp]() {
//            animatorLamp->SetAnimation("off");
//        });
    
        
        Rect* metroIntroAmbience = new Rect(1 * 64, 129 * 64, 71 * 64, 6 * 64);
        AmbientManager::GetInstance().AddRegion(*metroIntroAmbience, "../Recursos/audio/AMBIENCE/METRO ARRIVING.wav");
        
//        //TODO
//        Rect* dropAmbience = new Rect(38 * 64, 124 * 64, 30 * 64, 10 * 64);
//        AmbientManager::GetInstance().AddRegion(*dropAmbience, "../Recursos/audio/AMBIENCE/WATER DROPS 1.wav");
        

        Rect* metroMidAmbience = new Rect(0 * 64, 0 * 64, 135 * 64, 31 * 64);
        AmbientManager::GetInstance().AddRegion(*metroMidAmbience, "../Recursos/audio/AMBIENCE/METRO AMBIENCE 2.wav");
        
        Rect* metroEndAmbience = new Rect(83 * 64, 32 * 64, 44 * 64, 6 * 64);
        AmbientManager::GetInstance().AddRegion(*metroEndAmbience, "../Recursos/audio/AMBIENCE/METRO AMBIENCE.wav");
        
        Rect* sewersAmbience = new Rect(40 * 64, 38 * 64, 63 * 64, 91 * 64);
        Rect* sewersAmbience2 = new Rect(40 * 64, 20 * 64, 43 * 64, 18 * 64);
        AmbientManager::GetInstance().AddRegion(*sewersAmbience, "../Recursos/audio/AMBIENCE/WATER DROPS 2.wav");
        AmbientManager::GetInstance().AddRegion(*sewersAmbience2, "../Recursos/audio/AMBIENCE/WATER DROPS 2.wav");
    }

    hud = new HUD();

}

StageState::~StageState() {
    AmbientManager::GetInstance().StopAll();
}


void StageState::LoadAssets() {}

void StageState::Update(float dtt) {
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

bool YZ_Sort(GameObject* a, GameObject* b) {
    if (a->box.Z == b->box.Z)
        return a->box.Y < b->box.Y;
    return a->box.Z < b->box.Z;
}

void StageState::Render() {
    TileMap* tileMap = (TileMap*)tileMapObject->GetComponent("TileMap");
    tileMap->RenderLayer(9); // Camada
    tileMap->RenderLayer(8); // Camada
    tileMap->RenderLayer(7); // Camada
    tileMap->RenderLayer(6); // Camada
    tileMap->RenderLayer(5); // Camada
    tileMap->RenderLayer(4); // Camada
    tileMap->RenderLayer(3); // Camada
    tileMap->RenderLayer(2);  // Camada colisão

    vector<GameObject*> objectsToRender;
    for (const auto& objPtr : objectArray) {
        if (objPtr.get() != tileMapObject) {
            objectsToRender.push_back(objPtr.get());
        }
    }
    sort(objectsToRender.begin(), objectsToRender.end(), YZ_Sort);
    for (int i = 0; i < objectsToRender.size(); i++) {
        objectsToRender[i]->Render();
    }

    tileMap->RenderLayer(1); // Camada final frente
    tileMap->RenderLayer(0); // Camada metro frente

    hud->Render();
}

void StageState::Start() {
    StartArray();
    backgroundMusic.UpdateVolume();
    backgroundMusic.Play();
    GameData::playerVictory = false;
    GameData::ended = false;
}

void StageState::Pause() {
    backgroundMusic.Pause();
    AmbientManager::GetInstance().PauseAll();
}
void StageState::Resume() {
    backgroundMusic.UpdateVolume();
    backgroundMusic.Resume();
    AmbientManager::GetInstance().ResumeAll();
}

GameObject* StageState::GetTileMapObject() {
    return tileMapObject;
}

void StageState::End(GameObject* playerObject) {
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
    graffitiObj->box.Z = 0;
    graffitiObj->box.X = playerObject->box.X + playerObject->box.W/2 - graffitiObj->box.W/2;
    graffitiObj->box.Y = playerObject->box.Y - graffitiObj->box.H;
    AddObject(graffitiObj);
}
