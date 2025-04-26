#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP
#include "Component.hpp"

using namespace std;

class PlayerController : public Component{
public:
    PlayerController(GameObject& associated);
    void Start();
    void Update(float dt);
    void Render();
    bool Is(string type);
};

#endif //PLAYERCONTROLLER_HPP
