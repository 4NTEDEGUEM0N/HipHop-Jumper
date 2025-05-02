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
#include "../include/Item.hpp"
#include "../include/InventoryItem.hpp"


Character* Character::player = nullptr;

Character::Character(GameObject& associated, string sprite) : Component(associated), deathSound("../Recursos/audio/Dead.wav"), hitSound("../Recursos/audio/Hit1.wav") {
    gun.reset();
    inventory = vector<GameObject>();
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

    canJump = true;
    canDoubleJump = false;
    canDash = false;
    dashing = false;
    dashTimer = Timer();

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

        if (task.type == Command::MOVE && !dashing) {
            if (task.pos == Vec2(0,0)) {
                string currentAnimation = animator->GetAnimation();
                if (currentAnimation == "walkingRight")
                    animator->SetAnimation("idle");
                else if (currentAnimation == "walkingLeft")
                    animator->SetAnimation("idleLeft");
            } else {
                Vec2 direction = task.pos;
                direction = direction.normalize();
                speed = direction * linearSpeed * 2;
                Rect new_box_x = associated.box + Vec2(speed.X * dt, 0);

                vector<TileMap::CollisionInfo> collisions = tileMap->IsColliding(new_box_x);
                if (collisions.size() == 0) {
                    associated.box = associated.box + Vec2(speed.X * dt, 0);
                } else {
                    for (TileMap::CollisionInfo collision : collisions) {
                        if (collision.type == TileMap::TileCollisionType::Full) {
                            if (collision.corner == TileMap::CollisionCorner::TopRight or collision.corner == TileMap::CollisionCorner::BottomRight) {
                                associated.box.X = (collision.tilePos.X * tileMap->GetTileSetWidth())  - associated.box.W - 0.01;
                            } else if (collision.corner == TileMap::CollisionCorner::TopLeft or collision.corner == TileMap::CollisionCorner::BottomLeft) {
                                associated.box.X = (collision.tilePos.X * tileMap->GetTileSetWidth()) + tileMap->GetTileSetWidth() + 0.01;
                            }
                        }
                    }
                }

                //associated.box = new_box;
            }
        } else if (task.type == Command::SHOOT && !dashing) {
            Component* component = gun.lock()->GetComponent("Gun");
            Gun* gunCpt = dynamic_cast<Gun*>(component);
            gunCpt->Shot(task.pos);
        } else if (task.type == Command::JUMP && !dashing) {
            if (onGround) {
                ySpeed = -400;
                onGround = false;
                canJump = false;
            } else if (canDoubleJump) {
                ySpeed = -400;
                canDoubleJump = false;
            }
        } else if (task.type == Command::DASH && canDash) {
            if (task.pos.X != 0) {
                speed = task.pos * linearSpeed * 9;
                canDash = false;
                dashing = true;
                dashTimer.Restart();
                ySpeed = 0;
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

    dashTimer.Update(dt);
    if (dashing && dashTimer.Get() <= 0.15) {
        Rect new_box_x = associated.box + Vec2(speed.X * dt, 0);
        if (tileMap->IsColliding(new_box_x).size() == 0) {
            associated.box = new_box_x;
        } else {
            dashing = false;
        }
    }
    if (dashTimer.Get() > 0.15) {
        dashing = false;
        if (dashTimer.Get() > 4)
            canDash = true;
    }

    if (!dashing) {
        // calculo da gravidade do personagem
        ySpeed = ySpeed + 500.0f * dt;
        // speedcap de queda
        if (ySpeed > 500)
            ySpeed = 500;
        Rect new_box_y = associated.box + Vec2(0, ySpeed * dt);
        vector<TileMap::CollisionInfo> collisions = tileMap->IsColliding(new_box_y);
        if (collisions.size() > 0) {
            for (TileMap::CollisionInfo collision : collisions) {
                if (collision.corner == TileMap::CollisionCorner::TopRight or collision.corner == TileMap::CollisionCorner::TopLeft)
                    ySpeed = 0;
                if (collision.corner == TileMap::CollisionCorner::BottomRight or collision.corner == TileMap::CollisionCorner::BottomLeft) {
                    ySpeed = 0;
                    onGround = true;
                    canJump = true;
                    canDoubleJump = true;

                    if (collision.type == TileMap::TileCollisionType::Full) {
                        associated.box.Y = (collision.tilePos.Y * tileMap->GetTileSetHeight())  - associated.box.H - 0.01;
                    } else if (collision.type == TileMap::TileCollisionType::TriangleTopLeft and collision.corner == TileMap::CollisionCorner::BottomRight) {
                        float tileY = collision.tilePos.Y * tileMap->GetTileSetHeight();
                        float tileX = collision.tilePos.X * tileMap->GetTileSetWidth();
                        float rampY = -((float)tileMap->GetTileSetHeight()/(float)tileMap->GetTileSetWidth()) * (associated.box.X + associated.box.W) + (tileY + tileMap->GetTileSetHeight() + ((float)tileMap->GetTileSetHeight()/(float)tileMap->GetTileSetWidth())*tileX);
                        associated.box.Y = rampY - associated.box.H - 0.01;
                    } else if (collision.type == TileMap::TileCollisionType::TriangleTopRight and collision.corner == TileMap::CollisionCorner::BottomLeft) {
                        float tileY = collision.tilePos.Y * tileMap->GetTileSetHeight();
                        float tileX = collision.tilePos.X * tileMap->GetTileSetWidth();
                        float rampY = ((float)tileMap->GetTileSetHeight()/(float)tileMap->GetTileSetWidth()) * (associated.box.X) + (tileY - ((float)tileMap->GetTileSetHeight()/(float)tileMap->GetTileSetWidth())*tileX);
                        associated.box.Y = rampY - associated.box.H - 0.01;
                    }
                }
            }
        } else {
            associated.box = associated.box + Vec2(0, ySpeed * dt);
            onGround = false;
            canJump = false;
        }
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
    } else if (other.GetComponent("Item") != nullptr) {
        Item* itemCpt = dynamic_cast<Item*>(other.GetComponent("Item"));
        switch (itemCpt->GetItemType()) {
            case Item::ItemType::Minikit: {
                GameObject* inventoryItemObject = new GameObject();

                InventoryItem* inventoryItemCpt = new InventoryItem(*inventoryItemObject, Item::ItemType::Minikit);
                inventoryItemObject->AddComponent(inventoryItemCpt);
                
                inventory.push_back(inventoryItemObject);

                break;
            }
                
            case Item::ItemType::SprayCap:

                break;
            
                
            case Item::ItemType::SprayBody:

                break;
                
            case Item::ItemType::SprayColor:

                break;
    
        }
    }
}

Vec2 Character::GetPosition() const {
    return associated.box.center();
}

int Character::GetHP() {
    return hp;
}

bool Character::CanJump() {
    return canJump;
}

bool Character::CanDoubleJump() {
    return canDoubleJump;
}


bool Character::CanDash() {
    return canDash;
}





