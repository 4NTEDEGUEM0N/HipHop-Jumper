#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "GameObject.hpp"
#include "Rect.hpp"
#include "Vec2.hpp"

using namespace std;

class Collider: public Component {
private:
    Vec2 scale;
    Vec2 offset;

public:
    Rect box;
    Collider(GameObject& associated, Vec2 scale = {1,1}, Vec2 offset = {0,0});
    void Update(float dt);
    void Render();
    bool Is(string type);
    void SetScale(Vec2 scale);
    void SetOffset(Vec2 offset);

    static bool showCollision;
};

#endif //COLLIDER_HPP
