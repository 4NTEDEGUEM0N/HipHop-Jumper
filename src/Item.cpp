#include "../include/Item.hpp"
#include "../include/Animator.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Collider.hpp"

Item::Item(GameObject& associated, ItemData itemData) :
    Component(associated),
    itemData(itemData),
    collectSound(itemData.collectSound)
{
    counter = 0;
    up = false;
    collected = false;
    collectTimer.Restart();

    if (itemData.type == ItemData::Type::Minikit || itemData.type == ItemData::Type::Minikit2 || itemData.type == ItemData::Type::Minikit3) {
        itemSprite = new SpriteRenderer(associated, itemData.iconPath);
        associated.AddComponent(itemSprite);
    } else {
        itemSprite = new SpriteRenderer(associated, itemData.iconPath, 9, 9);
        associated.AddComponent(itemSprite);
        
        Animator* animator = new Animator(associated);
        animator->AddAnimation("static", Animation(0, 0, 1));
        animator->AddAnimation("spin", Animation(0, 74, 0.05));
        animator->SetAnimation("spin");
        associated.AddComponent(animator);
    }
    
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
        if (not scaled) {
            scaled = true;
            itemSprite->SetScale(0.001f, 0.001f);
        }
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
