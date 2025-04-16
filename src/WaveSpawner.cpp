#include "../include/WaveSpawner.hpp"
#include "../include/Game.hpp"
#include "../include/State.hpp"
#include "../include/Zombie.hpp"
#include "../include/Character.hpp"
#include "../include/AIController.hpp"
#include "iostream"
#include "../include/GameData.hpp"

WaveSpawner::WaveSpawner(GameObject &associated): Component(associated) {
    zombieCounter = 0;
    zombieCooldownTimer.Restart();
    currentWave = 0;
    waves.emplace_back(5,5);
    waves.emplace_back(10,2);
    waves.emplace_back(15,3);
}

void WaveSpawner::Update(float dt) {
    zombieCooldownTimer.Update(dt);


    if (zombieCounter < waves.begin()->zombies && zombieCooldownTimer.Get() > waves.begin()->cooldown) {
        State& state = Game::GetInstance().GetState();
        GameObject* zombieObject = new GameObject();
        state.AddObject(zombieObject);
        Zombie* zmb = new Zombie(*zombieObject, 100);
        int waveX = 640 + rand()%(1856 - 640 + 1);
        int waveY = 512 + rand()%(1984 - 512 + 1);
        zombieObject->box.X = waveX;
        zombieObject->box.Y = waveY;
        zombieObject->AddComponent(zmb);

        zombieCounter +=1;
        zombieCooldownTimer.Restart();
    } else if (zombieCounter == waves.begin()->zombies && Zombie::zombieCounter == 0) {
        waves.erase(waves.begin());
        zombieCounter = 0;
        currentWave += 1;

        State& state = Game::GetInstance().GetState();
        int waveX = 640 + rand()%(1856 - 640 + 1);
        int waveY = 512 + rand()%(1984 - 512 + 1);
        GameObject* aiObject = new GameObject();
        aiObject->box.X = waveX;
        aiObject->box.Y = waveY;
        state.AddObject(aiObject);
        Character* aiCharacter = new Character(*aiObject, "../Recursos/img/NPC.png");
        aiObject->AddComponent(aiCharacter);
        AIController* aiController = new AIController(*aiObject);
        aiObject->AddComponent(aiController);
    }

    if (waves.begin() == waves.end()) {
        GameData::ended = true;
        GameData::playerVictory = true;
        associated.RequestDelete();
    }
}

void WaveSpawner::Render() {}

bool WaveSpawner::Is(string type) {
    return type == "WaveSpawner";
}



