#ifndef INVENTORYITEM_HPP
#define INVENTORYITEM_HPP
#include "GameObject.hpp"
#include "Sound.hpp"
#include "Timer.hpp"
#include "Item.hpp"

using namespace std;

class InventoryItem : public Component {
public:
    InventoryItem(GameObject& associated, Item::ItemType type);
    void Update(float dt);
    void Render();
    bool Is(string type);
    
    Item::ItemType GetItemType();

private:
    Sound collectSound;
    bool collected;
    Timer collectTimer;

    Item::ItemType type;
    int counter;
    bool up;
};

#endif //INVENTORYITEM_HPP
