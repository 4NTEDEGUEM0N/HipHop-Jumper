
#include "../include/InventoryItem.hpp"

InventoryItem::InventoryItem(GameObject& associated, const ItemData& itemData) : Component(associated), itemData(itemData) {
}

void InventoryItem::Update(float dt) {}

void InventoryItem::Render() {}

bool InventoryItem::Is(string type) {
    return type == "InventoryItem";
}

ItemData InventoryItem::GetItemData() {
    return itemData;
}

