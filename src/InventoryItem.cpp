
#include "../include/InventoryItem.hpp"

InventoryItem::InventoryItem(GameObject& associated, Item::ItemType type) : Component(associated) {
    this->type = type;
}

void InventoryItem::Update(float dt) {}

void InventoryItem::Render() {}

bool InventoryItem::Is(string type) {
    return type == "InventoryItem";
}

Item::ItemType InventoryItem::GetItemType() {
    return type;
}

