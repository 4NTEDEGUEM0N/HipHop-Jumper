#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <string>
class GameObject; //#include "GameObject.hpp"

using namespace std;

class Component {
protected:
    GameObject& associated;

public:
    Component(GameObject& associated);
    virtual ~Component();

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual bool Is(string type) = 0;
    virtual void Start();
    virtual void NotifyCollision(GameObject& other);
};

#endif //COMPONENT_HPP
