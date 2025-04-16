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
                                                                   shotSound("../Recursos/audio/Range.wav"), reloadSound("../Recursos/audio/PumpAction.mp3"), character(character) {
    cdTimer = Timer();
    cooldown = 0;
    angle = 0;
    reloaded = true;
    reloading = false;

    SpriteRenderer* gun = new SpriteRenderer(associated, "../Recursos/img/Gun.png", 3, 2);
    gun->SetScale(0.9, 0.9);
    associated.AddComponent(gun);
    Animator* animator = new Animator(associated);
    animator->AddAnimation("idle", Animation(0, 0, 0));
    animator->AddAnimation("idleLeft", Animation(0, 0, 0, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("reload", Animation(1, 5, 0.08));
    animator->AddAnimation("reloadLeft", Animation(1, 5, 0.08, SDL_FLIP_VERTICAL));

    animator->SetAnimation("idle");
    associated.AddComponent(animator);
}

void Gun::Update(float dt) {
    if (character.expired()) {
        associated.RequestDelete();
        return;
    }

    Animator* animatorGun = dynamic_cast<Animator*>(associated.GetComponent("Animator"));
    Animator* animatorCharacter = dynamic_cast<Animator*>(character.lock()->GetComponent("Animator"));
    string characterAnimation = animatorCharacter->GetAnimation();
    string gunAnimation = animatorGun->GetAnimation();

    if (characterAnimation == "walkingLeft" || characterAnimation == "idleLeft")
        associated.angleDeg = (angle + M_PI) * 180 / M_PI;
    else
        associated.angleDeg = angle * 180 / M_PI;

    Vec2 character_center = character.lock()->box.center();
    float gunX = character_center.GetX() - associated.box.GetW() / 2;
    float gunY = character_center.GetY() - associated.box.GetH() / 2;
    associated.box.X = gunX;
    associated.box.Y = gunY + 20;

    Vec2 direction = Vec2(1, 0).rotate(angle);
    if (characterAnimation == "walkingLeft" || characterAnimation == "idleLeft") {
        associated.box = associated.box + direction * 10;
        associated.box.X = associated.box.X - direction.GetX() * 10;
    } else
        associated.box = associated.box + direction * 10;

    if (cooldown > 0) {
        cooldown -= dt;
    }

    if (cooldown <= 0 && !reloaded && !reloading) {
        characterAnimation = animatorCharacter->GetAnimation();
        if (characterAnimation == "walkingRight" || characterAnimation == "idle")
            animatorGun->SetAnimation("reload");
        else if (characterAnimation == "walkingLeft" || characterAnimation == "idleLeft")
            animatorGun->SetAnimation("reloadLeft");
        reloadSound.Play(1);
        cdTimer.Restart();
        reloading = true;
    }

    cdTimer.Update(dt);
    if (cdTimer.Get() > 0.5) {
        angle = 0;
        associated.angleDeg = angle * 180 / M_PI;
        if (characterAnimation == "walkingRight" || characterAnimation == "idle")
            animatorGun->SetAnimation("idle");
        else if (characterAnimation == "walkingLeft" || characterAnimation == "idleLeft")
            animatorGun->SetAnimation("idleLeft");

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

    angle = 0;
    associated.angleDeg = angle * 180 / M_PI;

    Vec2 direction = target - associated.box.center();
    direction = direction.normalize();
    angle = direction.angle();

    State& state = Game::GetInstance().GetState();

    Character* gunCharacter = dynamic_cast<Character*>(character.lock()->GetComponent("Character"));
    bool targetsPlayer = true;
    if (gunCharacter == Character::player) {
        targetsPlayer = false;

        GameObject* bulletObject2 = new GameObject();
        bulletObject2->box.X = associated.box.X + 35;
        bulletObject2->box.Y = associated.box.Y;
        state.AddObject(bulletObject2);
        Bullet* bullet2 = new Bullet(*bulletObject2, angle + 0.25f, 1000, 25, 1000, targetsPlayer);
        bulletObject2->AddComponent(bullet2);

        GameObject* bulletObject3 = new GameObject();
        bulletObject3->box.X = associated.box.X + 35;
        bulletObject3->box.Y = associated.box.Y;
        state.AddObject(bulletObject3);
        Bullet* bullet3 = new Bullet(*bulletObject3, angle - 0.25f, 1000, 25, 1000, targetsPlayer);
        bulletObject3->AddComponent(bullet3);
    }

    GameObject* bulletObject = new GameObject();
    bulletObject->box.X = associated.box.X + 35;
    bulletObject->box.Y = associated.box.Y;
    state.AddObject(bulletObject);

    Bullet* bullet = new Bullet(*bulletObject, angle, 1000, 25, 1000, targetsPlayer);
    bulletObject->AddComponent(bullet);


    shotSound.Play(1);
    cooldown = 50;
    cdTimer.Restart();
    reloaded = false;
}


