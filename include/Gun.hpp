#ifndef GUN_HPP
#define GUN_HPP
#include <memory>

#include "Component.hpp"
#include "Sound.hpp"
#include "Timer.hpp"
#include "Vec2.hpp"

using namespace std;

class Gun : public Component {
private:
    Sound shotSound;
    Sound reloadSound;
    int cooldown;
    Timer cdTimer;
    weak_ptr<GameObject> character;
    float angle;
    bool reloaded;
    bool reloading;

public:
    Gun(GameObject& associated, weak_ptr<GameObject> character);
    void Update(float dt);
    void Render();
    bool Is(string type);
    void Shot(Vec2 target);
};

#endif //GUN_HPP
