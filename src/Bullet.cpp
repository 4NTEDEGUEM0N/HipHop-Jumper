#include "../include/Bullet.hpp"

#include <iostream>

#include "../include/Character.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/GameObject.hpp"
#include "../include/Collider.hpp"
#include "../include/Gun.hpp"

Bullet::Bullet(GameObject &associated, float angle, float speed, int damage, float maxDistance, bool targetsPlayer) : Component(associated) {
    SpriteRenderer* spriteRenderer = new SpriteRenderer(associated, "../Recursos/img/Bullet.png");
    associated.angleDeg = (angle * 180 / M_PI) + 90;
    associated.AddComponent(spriteRenderer);

    this->speed = Vec2(1, 0).rotate(angle) * speed;
    distanceLeft = maxDistance;
    this->damage = damage;

    Collider* collider = new Collider(associated, {1,1}, {0,-15});
    associated.AddComponent(collider);

    this->targetsPlayer = targetsPlayer;
}

void Bullet::Update(float dt) {
    associated.box = associated.box + speed * dt;
    distanceLeft -= speed.magnitude() * dt;

    if (distanceLeft <= 0) {
        associated.RequestDelete();
    }
}

void Bullet::Render() {}

bool Bullet::Is(string type) {
    return type == "Bullet";
}

int Bullet::GetDamage() {
    return damage;
}

void Bullet::NotifyCollision(GameObject &other) {
    if (other.GetComponent("Zombie") != nullptr) {
        associated.RemoveComponent(associated.GetComponent("Collider"));
    }
}

