#include "../include/Gun.hpp"

#include <iostream>

#include "../include/SpriteRenderer.hpp"
#include "../include/Animator.hpp"
#include "../include/Bullet.hpp"
#include "../include/InputManager.hpp"
#include "../include/Camera.hpp"
#include "../include/Character.hpp"
#include "../include/Game.hpp"

Gun::Gun(GameObject& associated, weak_ptr<GameObject> character) : Component(associated),
                                                                   shotSound("../Recursos/audio/GRAFFITI/SPRAY 1.wav"), reloadSound("../Recursos/audio/GRAFFITI/CANSHAKESPRAY.wav"), character(character) {
    cdTimer = Timer();
    cooldown = 0;
    angle = 0;
    reloaded = true;
    reloading = false;

    SpriteRenderer* gun = new SpriteRenderer(associated, "../Recursos/img/spray_can.png", 1, 1);
    gun->SetScale(0.07, 0.07);
    associated.AddComponent(gun);
}

void Gun::Update(float dt) {
    if (character.expired()) {
        associated.RequestDelete();
        return;
    }

    Vec2 character_center = character.lock()->box.center();
    associated.box.X = character_center.GetX() - associated.box.W/2;
    associated.box.Y = character_center.GetY() - 20;

    if (cooldown > 0) {
        cooldown -= dt;
    }

    if (cooldown <= 0 && !reloaded && !reloading) {
        reloadSound.Play(1);
        cdTimer.Restart();
        reloading = true;
    }

    cdTimer.Update(dt);
    if (cdTimer.Get() > 0.5) {
        if (reloading) {
            reloaded = true;
            reloading = false;
        }
    }
}

void Gun::Render() {}

bool Gun::Is(string type) {
    return type == "Gun";
}

void Gun::Shot(Vec2 target) {
    if (cooldown > 0 || !reloaded) {
        return;
    }

    //angle = 90;
    //associated.angleDeg = 270;

    //Vec2 direction = target - associated.box.center();
    //direction = direction.normalize();
    //angle = direction.angle();
    int flipSpawnOffset, bulletSpeed;
    
    if (Character::player->direction.X > 0) {
        angle = 0;
        flipSpawnOffset = 1;
    } else {
        angle = M_PI;
        flipSpawnOffset = -1;
    }

    State& state = Game::GetInstance().GetState();

    Character* gunCharacter = dynamic_cast<Character*>(character.lock()->GetComponent("Character"));

    GameObject* bulletObject = new GameObject(false);
    state.AddObject(bulletObject);
    
    //bulletSpeed = 300 +Character::player->speed.X;

    Bullet* bullet = new Bullet(*bulletObject, angle, 400, 25, 200, false);
    bulletObject->box.X = associated.box.X - associated.box.W/2 - bulletObject->box.W/2 + (40 * flipSpawnOffset);
    bulletObject->box.Y = associated.box.Y - associated.box.H/2 - bulletObject->box.H/2;
    bulletObject->AddComponent(bullet);
    
    bulletObject->angleDeg = 270 * flipSpawnOffset;

    shotSound.Play(1);
    cooldown = 50;
    cdTimer.Restart();
    reloaded = false;
}


