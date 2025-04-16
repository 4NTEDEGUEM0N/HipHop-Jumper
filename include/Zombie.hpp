#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP
#include "Component.hpp"
#include "GameObject.hpp"
#include "Sound.hpp"
#include "Timer.hpp"

class Zombie : public Component {
private:
    int hitpoints;
    Sound deathSound;
    bool hasPlayedDeathSound;
    Sound hitSound;
    bool hit;
    Timer hitTimer;
    Timer deathTimer;

public:
    Zombie(GameObject& associated, int hp);
    ~Zombie();
    void Damage(int damage);
    void Update(float dt);
    void Render();
    bool Is(string type);
    void NotifyCollision(GameObject &other);

    static int zombieCounter;
};

#endif //ZOMBIE_HPP
