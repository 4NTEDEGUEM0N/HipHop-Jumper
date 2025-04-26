#ifndef WAVESPAWNER_HPP
#define WAVESPAWNER_HPP
#include <vector>
#include "Component.hpp"
#include "Timer.hpp"
#include "Wave.hpp"

class WaveSpawner: public Component {
private:
    int zombieCounter;
    vector<Wave> waves;
    Timer zombieCooldownTimer;
    int currentWave;

public:
    WaveSpawner(GameObject& associated);
    void Update(float dt);
    void Render();
    bool Is(string type);
};

#endif //WAVESPAWNER_HPP
