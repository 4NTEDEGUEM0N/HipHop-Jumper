#include "../include/Tile.hpp"

#include <iostream>

#include "../include/Collider.hpp"

Tile::Tile(GameObject &associated, int id) : Component(associated) {
    this->id = id;
    Collider* collider = new Collider(associated, {0.9, 0.9});
    associated.AddComponent(collider);
}

bool Tile::Is(string type) {
    return type == "Tile";
}

void Tile::Update(float dt) {}

void Tile::Render() {}

void Tile::NotifyCollision(GameObject &other) {
    if (other.GetComponent("Tile") != nullptr) {
        cerr << "Colisao Tile x Tile" << endl;
    } else if (other.GetComponent("Character") != nullptr) {
        cerr << "Colisao Tile x Character" << endl;
    }
}
