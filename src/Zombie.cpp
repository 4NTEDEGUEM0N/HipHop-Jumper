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
    ySpeed = 0;
    hitting = false;

    SpriteRenderer* zmb = new SpriteRenderer(associated, "../Recursos/img/vigilante.png", 4,8);
    associated.AddComponent(zmb);
    //zmb->SetScale(0.1, 0.1);

    Animator *animator = new Animator(associated);
    animator->AddAnimation("walking", Animation(0, 11, 0.1));
    animator->AddAnimation("walkingLeft", Animation(0, 11, 0.1, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("dead", Animation(18, 18, 0));
    animator->AddAnimation("hit", Animation(12, 23, 0.08));
    animator->AddAnimation("hitLeft", Animation(12, 23, 0.08, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("hitting", Animation(24, 31, 0.05));
    animator->AddAnimation("hittingLeft", Animation(24, 31, 0.05, SDL_FLIP_HORIZONTAL));
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
    if (hit && hitTimer.Get() > 0.8 && hitpoints > 0 && not hitting) {
        hit = false;
        Component* component = associated.GetComponent("Animator");
        Animator* animator = dynamic_cast<Animator*>(component);
        animator->SetAnimation("walking");
    }

    deathTimer.Update(dt);
    if (hitpoints <= 0 && deathTimer.Get() > 5) {
        associated.RequestDelete();
    }

    if (hitpoints > 0 and !hit and !hitting) {
        GameObject* tileMapObject = Game::GetInstance().GetState().GetTileMapObject();
        Component* tileMapComponent = tileMapObject->GetComponent("TileMap");
        TileMap* tileMap = dynamic_cast<TileMap*>(tileMapComponent);
        if (onGround) {
            //associated.box = associated.box + Vec2(100.0f *dt, 0);
            CheckDirection(dt, tileMap);
        } else {
            ySpeed = ySpeed + 250.0f * dt;
            Rect new_box = associated.box + Vec2(0, ySpeed * dt);
            auto collisions = tileMap->IsColliding(new_box);
            if (collisions.empty()) {
                onGround = false;
                associated.box = new_box;
            } else {
                auto collision = collisions.front();
                associated.box.Y = (collision.tilePos.Y * tileMap->GetTileSetHeight())  - associated.box.H - 0.01;
                ySpeed = 0;
                onGround = true;
            }
        }
    }
    hittingTimer.Update(dt);
    if (hitting && hittingTimer.Get() > 0.4 && hitpoints > 0) {
        hitting = false;
        Component* component = associated.GetComponent("Animator");
        Animator* animator = dynamic_cast<Animator*>(component);
        animator->SetAnimation("walking");
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
    if (tileMap->IsColliding(new_box).size() == 0) {
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
    else if (other.GetComponent("Character") != nullptr) {
        hitting = true;
        hittingTimer.Restart();
        Component* component = associated.GetComponent("Animator");
        Animator* animator = dynamic_cast<Animator*>(component);
        (associated.box.X - other.box.X > 0) ? animator->SetAnimation("hittingLeft") : animator->SetAnimation("hitting");
    }
}

Zombie::~Zombie() {
    zombieCounter -= 1;
}

