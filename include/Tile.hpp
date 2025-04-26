#ifndef TILE_HPP
#define TILE_HPP
#include "Component.hpp"

using namespace std;

class Tile : public Component {
public:
    Tile(GameObject& associated, int id);
    void Update(float dt);
    void Render();
    void NotifyCollision(GameObject &other);
    int id;
    bool Is(string type);
};

#endif //TILE_HPP
