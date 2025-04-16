#include "../include/Zombie.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Animator.hpp"
#include "../include/Camera.hpp"
#include "../include/Collider.hpp"
#include "../include/Character.hpp"
#include <iostream>

#include "../include/InputManager.hpp"

int Zombie::zombieCounter = 0;

Zombie::Zombie(GameObject& associated, int hp):Component(associated), deathSound("../Recursos/audio/Dead.wav"), hitSound("../Recursos/audio/Hit0.wav") {
    hitpoints = hp;
    hasPlayedDeathSound = false;
    hit = false;
    zombieCounter += 1;

    SpriteRenderer* zmb = new SpriteRenderer(associated, "../Recursos/img/Enemy.png", 3,2);
    associated.AddComponent(zmb);

    Animator *animator = new Animator(associated);
    animator->AddAnimation("walking", Animation(0, 3, 0.2));
    animator->AddAnimation("walkingLeft", Animation(0, 3, 0.2, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("dead", Animation(5, 5, 0));
    animator->AddAnimation("hit", Animation(4, 4, 0));
    animator->AddAnimation("hitLeft", Animation(4, 4, 0, SDL_FLIP_HORIZONTAL));
    animator->SetAnimation("walking");
    associated.AddComponent(animator);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);


}

void Zombie::Damage(int damage) {
    hitpoints -= damage;
    if (hitpoints <= 0 && !hasPlayedDeathSound) {
        Component* component = associated.GetComponent("Animator");
        Animator* animator = dynamic_cast<Animator*>(component);
        animator->SetAnimation("dead");

        hasPlayedDeathSound = true;
        deathSound.Play(1);
        deathTimer.Restart();

        associated.RemoveComponent(associated.GetComponent("Collider"));

    } else if (hitpoints > 0 && !hasPlayedDeathSound) {
        hit = true;
        hitTimer.Restart();
        Component* component = associated.GetComponent("Animator");
        Animator* animator = dynamic_cast<Animator*>(component);
        if (animator->GetAnimation() == "walking")
            animator->SetAnimation("hit");
        else
            animator->SetAnimation("hitLeft");
    }
}

void Zombie::Update(float dt) {
    /*if (InputManager::GetInstance().MousePress(LEFT_MOUSE_BUTTON)) {
        int mouseX = InputManager::GetInstance().GetMouseX();
        int mouseY = InputManager::GetInstance().GetMouseY();
        float cameraX = Camera::pos.GetX();
        float cameraY = Camera::pos.GetY();
        Vec2 mousePos = Vec2(mouseX + cameraX, mouseY + cameraY);

        if (associated.box.contains(mousePos)) {
            hitSound.Play(1);
            Damage(25);
        }
    }*/

    hitTimer.Update(dt);
    if (hit && hitTimer.Get() > 0.5 && hitpoints > 0) {
        hit = false;
        Component* component = associated.GetComponent("Animator");
        Animator* animator = dynamic_cast<Animator*>(component);
        animator->SetAnimation("walking");
    }

    deathTimer.Update(dt);
    if (hitpoints <= 0 && deathTimer.Get() > 5) {
        associated.RequestDelete();
    }

    if (Character::player != nullptr && hitpoints > 0) {
        Vec2 playerPos = Character::player->GetPosition();
        Vec2 direction = playerPos - associated.box.center();
        Component* component = associated.GetComponent("Animator");
        Animator* animator = dynamic_cast<Animator*>(component);
        if (hitTimer.Get() > 0.5) {
            if (direction.GetX() < 0)
            animator->SetAnimation("walkingLeft");
        else
            animator->SetAnimation("walking");
        }

        associated.box = associated.box + direction.normalize() * 100 * dt;
    }
}

void Zombie::Render() {}

bool Zombie::Is(string type) {
    return type == "Zombie";
}

void Zombie::NotifyCollision(GameObject &other) {
    if (other.GetComponent("Bullet") != nullptr) {
        hitSound.Play(1);
        Damage(25);
    }
}

Zombie::~Zombie() {
    zombieCounter -= 1;
}

