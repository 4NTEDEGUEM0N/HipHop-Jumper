#include "../include/Item.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Collider.hpp"

Item::Item(GameObject& associated, ItemData itemData, string file) :
    Component(associated),
    itemData(itemData),
    collectSound("../Recursos/audio/lego-star-wars-minikit.mp3")
{
    counter = 0;
    up = false;
    collected = false;
    collectTimer.Restart();

    SpriteRenderer* item = new SpriteRenderer(associated, itemData.iconPath);
    associated.AddComponent(item);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);
}

void Item::Update(float dt) {
    if (counter > 50)
        up = true;
    else if (counter < -50)
        up = false;
    if (up)
        counter -= 5;
    else
        counter += 5;
    associated.box.Y = associated.box.Y + counter*dt;

    if (collected) {
        if (collectTimer.Get() >= 0.5)
            associated.RequestDelete();
        collectTimer.Update(dt);
    }
}

void Item::Render() {}

bool Item::Is(string type) {
    return type == "Item";
}

bool Item::IsCollected() {
    return collected;
}

void Item::SetCollected() {
    collected = true;
}


void Item::TryCollect() {
    if (!collected) {
        collectSound.Play(1);
        collectTimer.Restart();
    }
}

void Item::NotifyCollision(GameObject& other) {
    if (other.GetComponent("Character")) {
        // exibir texto "pressione E pra coletar"? baseado nas informacoes do item
    }
}

ItemData Item::GetItemData() {
    return itemData;
}
