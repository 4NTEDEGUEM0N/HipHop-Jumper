#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP
#include "Component.hpp"
#include "GameObject.hpp"
#include "Sound.hpp"
#include "TileMap.hpp"
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
    bool runLeft;
    bool onGround;
    float ySpeed;
    bool hitting;
    Timer hittingTimer;

public:
    Zombie(GameObject& associated, int hp);
    ~Zombie();
    void Damage(int damage);
    void Update(float dt);
    void Render();
    bool Is(string type);
    void NotifyCollision(GameObject &other);
    void CheckDirection(float dt, TileMap* tileMap);
    bool IsTakingHit();

    static int zombieCounter;
};

#endif //ZOMBIE_HPP
