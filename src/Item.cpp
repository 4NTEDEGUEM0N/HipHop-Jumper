#include "../include/Item.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Collider.hpp"

Item::Item(GameObject& associated, ItemType type, string file) : Component(associated), collectSound("../Recursos/audio/lego-star-wars-minikit.mp3") {
    this->type = type;
    counter = 0;
    up = false;
    collected = false;
    collectTimer.Restart();

    SpriteRenderer* item = new SpriteRenderer(associated, file);
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
        collectTimer.Update(dt);
        if (collectTimer.Get() >= 0.4)
            associated.RequestDelete();
    }
}

void Item::Render() {}

bool Item::Is(string type) {
    return type == "Item";
}

void Item::NotifyCollision(GameObject &other) {
    if (other.GetComponent("Character") != nullptr) {
        if (!collected) {
            collectSound.Play(1);
            collected = true;
        }
    }
}
