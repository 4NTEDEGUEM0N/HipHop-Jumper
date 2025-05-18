#ifndef INVENTORYITEM_HPP
#define INVENTORYITEM_HPP
#include "GameObject.hpp"
#include "Sound.hpp"
#include "Timer.hpp"
#include "Item.hpp"

using namespace std;

class InventoryItem : public Component {
public:
    InventoryItem(GameObject& associated, const ItemData& itemData);
    void Update(float dt);
    void Render();
    bool Is(string type);
    
    ItemData GetItemData();

private:
    Sound collectSound;
    bool collected;
    Timer collectTimer;

    ItemData itemData;
    int counter;
    bool up;
};

#endif //INVENTORYITEM_HPP
