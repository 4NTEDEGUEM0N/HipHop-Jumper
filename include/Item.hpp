#ifndef ITEM_HPP
#define ITEM_HPP
#include "ItemData.hpp"
#include "GameObject.hpp"
#include "Sound.hpp"
#include "SpriteRenderer.hpp"
#include "Timer.hpp"

using namespace std;

class Item : public Component {
public:
    Item(GameObject& associated, ItemData itemData);
    void Update(float dt);
    void Render();
    bool Is(string type);
    bool IsCollected();
    void SetCollected();
    void TryCollect();
    void NotifyCollision(GameObject &other);
    
    ItemData GetItemData();

private:
    Sound collectSound;
    bool collected;
    Timer collectTimer;

    ItemData itemData;
    SpriteRenderer* itemSprite;
    bool scaled = false;
    int counter;
    bool up;
};

#endif //ITEM_HPP
