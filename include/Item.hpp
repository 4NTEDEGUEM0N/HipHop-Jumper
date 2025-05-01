#ifndef ITEM_HPP
#define ITEM_HPP
#include "GameObject.hpp"
#include "Sound.hpp"
#include "Timer.hpp"

using namespace std;

class Item : public Component {
public:
    enum class ItemType {
        Minikit,
        SprayCap,
        SprayBody,
        SprayColor
    };
    Item(GameObject& associated, ItemType type, string file);
    void Update(float dt);
    void Render();
    bool Is(string type);
    void NotifyCollision(GameObject &other);

private:
    Sound collectSound;
    bool collected;
    Timer collectTimer;

    ItemType type;
    int counter;
    bool up;
};

#endif //ITEM_HPP
