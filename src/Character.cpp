#include "../include/Character.hpp"

#include <iostream>

#include "../include/SpriteRenderer.hpp"
#include "../include/Animator.hpp"
#include "../include/Collider.hpp"
#include "../include/Gun.hpp"
#include "../include/Game.hpp"
#include "../include/Zombie.hpp"
#include "../include/Camera.hpp"
#include "../include/Bullet.hpp"
#include "../include/GameData.hpp"
#include "../include/StageState.hpp"
#include "../include/TileMap.hpp"


Character* Character::player = nullptr;

Character::Character(GameObject& associated, string sprite) : Component(associated), deathSound("../Recursos/audio/Dead.wav"), hitSound("../Recursos/audio/Hit1.wav") {
    gun.reset();
    taskQueue = queue<Command>();
    speed = Vec2(0, 0);
    linearSpeed = 200;
    hp = 100;
    deathTimer = Timer();
    dead = false;
    damageCooldown = Timer();
    ySpeed = 0;
    onGround = true;
    if (player == nullptr)
        player = this;

    SpriteRenderer* character = new SpriteRenderer(associated, sprite, 3, 4);
    associated.AddComponent(character);

    Animator* animator = new Animator(associated);
    animator->AddAnimation("idle", Animation(6, 9, 0.2));
    animator->AddAnimation("idleLeft", Animation(6, 9, 0.2, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("walkingRight", Animation(0, 5, 0.2));
    animator->AddAnimation("walkingLeft", Animation(0, 5, 0.2, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("dead", Animation(10, 11, 0.5));

    animator->SetAnimation("idle");
    associated.AddComponent(animator);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);
}

Character::~Character() {
    if (player == this) {
        player = nullptr;
    }
}

void Character::Start() {
    State& state = Game::GetInstance().GetState();

    GameObject* gunObject = new GameObject();
    weak_ptr<GameObject> gunObjectPtr = state.AddObject(gunObject);

    Gun* gunCpt = new Gun(*gunObject, weak_ptr<GameObject>(state.GetObjectPtr(&associated)));
    gunObject->AddComponent(gunCpt);

    gun = gunObjectPtr;
}

void Character::Update(float dt) {
    Component* component = associated.GetComponent("Animator");
    Animator* animator = dynamic_cast<Animator*>(component);

    GameObject* tileMapObject = Game::GetInstance().GetState().GetTileMapObject();
    Component* tileMapComponent = tileMapObject->GetComponent("TileMap");
    TileMap* tileMap = dynamic_cast<TileMap*>(tileMapComponent);

    while(!taskQueue.empty()) {
        Command task = taskQueue.front();
        taskQueue.pop();

        if (task.type == Command::MOVE) {
            if (task.pos == Vec2(0,0)) {
                string currentAnimation = animator->GetAnimation();
                if (currentAnimation == "walkingRight")
                    animator->SetAnimation("idle");
                else if (currentAnimation == "walkingLeft")
                    animator->SetAnimation("idleLeft");
            } else {
                Vec2 direction = task.pos;
                direction = direction.normalize();
                speed = direction * linearSpeed;
                Rect new_box_x = associated.box + Vec2(speed.X * dt, 0);

                if (!tileMap->IsColliding(new_box_x)) {
                    associated.box = associated.box + Vec2(speed.X * dt, 0);
                }

                //associated.box = new_box;
            }
        } else if (task.type == Command::SHOOT) {
            Component* component = gun.lock()->GetComponent("Gun");
            Gun* gunCpt = dynamic_cast<Gun*>(component);
            gunCpt->Shot(task.pos);
        }

        if (task.type == Command::JUMP) {
            if (onGround) {
                ySpeed = -300;
                onGround = false;
            }

        }

        if (hp <= 0 && !dead) {
            animator->SetAnimation("dead");
            dead = true;
            deathSound.Play(1);
            deathTimer.Restart();
            if (this == player) {
                Camera::Unfollow();
                Component* playerController = associated.GetComponent("PlayerController");
                if (playerController != nullptr)
                    associated.RemoveComponent(playerController);
            } else {
                Component* aiController = associated.GetComponent("AIController");
                if (aiController != nullptr)
                    associated.RemoveComponent(aiController);
            }
            Component* collider = associated.GetComponent("Collider");
            if (collider != nullptr)
                associated.RemoveComponent(collider);
        }

        if (task.type == Command::MOVE && hp > 0) {
            if (task.pos.GetX() < 0) {
                animator->SetAnimation("walkingLeft");
            } else if (task.pos.GetX() > 0 || task.pos.GetY() != 0) {
                animator->SetAnimation("walkingRight");
            }
        }
        damageCooldown.Update(dt);
    }

    ySpeed = ySpeed + 250.0f * dt;
    if (ySpeed > 500)
        ySpeed = 500;
    Rect new_box_y = associated.box + Vec2(0, ySpeed * dt);
    if (!tileMap->IsColliding(new_box_y)) {
        associated.box = associated.box + Vec2(0, ySpeed * dt);
    } else if (ySpeed > 0) {
        ySpeed = 0;
        onGround = true;
    } else {
        ySpeed = 0;
    }


    deathTimer.Update(dt);
    if (dead && deathTimer.Get() > 2) {
        if (this == player) {
            GameData::ended = true;
            GameData::playerVictory = false;
        }
        associated.RequestDelete();
    }
}

void Character::Render() {}

bool Character::Is(string type) {
    return type == "Character";
}

void Character::Issue(Command task) {
    taskQueue.push(task);
}

Character::Command::Command(CommandType type, float x, float y) {
    this->type = type;
    this->pos = Vec2(x, y);
}

void Character::NotifyCollision(GameObject &other) {
    if (other.GetComponent("Zombie") != nullptr) {
        if (damageCooldown.Get() > 1 && hp > 0) {
            if (this == player) {
                hp -= 25;
                damageCooldown.Restart();
                hitSound.Play(1);
            }
        }
    } else if (other.GetComponent("Bullet") != nullptr) {
        Bullet* bulletCpt = dynamic_cast<Bullet*>(other.GetComponent("Bullet"));
        if (bulletCpt->targetsPlayer) {
            if (this == player) {
                hp -= 25;
                hitSound.Play(1);
            }
        } else {
            if (this != player) {
                hp -= 25;
                hitSound.Play(1);
            }
        }
    }
}

Vec2 Character::GetPosition() const {
    return associated.box.center();
}

int Character::GetHP() {
    return hp;
}




