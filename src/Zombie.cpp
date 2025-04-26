#include "../include/Zombie.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Animator.hpp"
#include "../include/Camera.hpp"
#include "../include/Collider.hpp"
#include "../include/Character.hpp"
#include "../include/Game.hpp"
#include <iostream>

#include "../include/InputManager.hpp"

int Zombie::zombieCounter = 0;

Zombie::Zombie(GameObject& associated, int hp):Component(associated), deathSound("../Recursos/audio/Dead.wav"), hitSound("../Recursos/audio/Hit0.wav") {
    hitpoints = hp;
    hasPlayedDeathSound = false;
    hit = false;
    zombieCounter += 1;
    onGround = false;
    runLeft = false;

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

    GameObject* tileMapObject = Game::GetInstance().GetState().GetTileMapObject();
    Component* tileMapComponent = tileMapObject->GetComponent("TileMap");
    TileMap* tileMap = dynamic_cast<TileMap*>(tileMapComponent);
    if (onGround) {
        //associated.box = associated.box + Vec2(100.0f *dt, 0);
        CheckDirection(dt, tileMap);
    } else {
        ySpeed = ySpeed + 250.0f * dt;
        Rect new_box = associated.box + Vec2(0, ySpeed * dt);
        if (!tileMap->IsColliding(new_box)) {
            associated.box = new_box;
        } else {
            ySpeed = 0;
            onGround = true;
        }
    }
}

void Zombie::CheckDirection(float dt, TileMap* tileMap) {
    Component* component = associated.GetComponent("Animator");
    Animator* animator = dynamic_cast<Animator*>(component);

    float speed = 100.0;
    if (runLeft) {
        animator->SetAnimation("walkingLeft");
        speed = speed * -1;
    } else {
        animator->SetAnimation("walking");
    }
    Rect new_box = associated.box + Vec2(speed * dt,0);
    if (!tileMap->IsColliding(new_box)) {
        int boxX = (new_box.X + (new_box.W/2))/tileMap->GetTileSetWidth();
        int boxY = (new_box.Y + new_box.H + 5)/tileMap->GetTileSetHeight();
        if (tileMap->GetCollisionType(boxX, boxY) != TileMap::TileCollisionType::Full) {
            runLeft = !runLeft;
        } else {
            associated.box = new_box;
        }
    } else {
        runLeft = !runLeft;
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

