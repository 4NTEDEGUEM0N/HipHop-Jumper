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
#include "../include/InputManager.hpp"
#include "../include/StageState.hpp"
#include "../include/TileMap.hpp"
#include "../include/Item.hpp"
#include "../include/InventoryItem.hpp"


Character* Character::player = nullptr;
vector<SDL_Texture*> Character::graffitiArray;
int Character::id = 0;

Character::Character(GameObject& associated, string sprite) : Component(associated), deathSound("../Recursos/audio/Dead.wav"), hitSound("../Recursos/audio/Hit1.wav") {
    gun.reset();
    inventory = vector<GameObject>();
    taskQueue = queue<Command>();
    speed = Vec2(0, 0);
    maxGroundSpeed = 500;
    maxFallSpeed = 700;
    airGravity = 2000;
    wallGravity = 50;
    groundAcceleration = 3000;
    airAcceleration = 1500;
    jumpSpeed = -800;
    dashSpeed = 2000;
    hp = 100;
    deathTimer = Timer();
    dead = false;
    damageCooldown = Timer();
    onGround = true;
    onWall = false;
    if (player == nullptr)
        player = this;

    canJump = true;
    canDoubleJump = false;
    canDash = false;
    dashing = false;
    dashTimer = Timer();
    isHit = false;
    dashTimer = Timer();
    dashDuration = 0.15;
    moving = false;


    //SpriteRenderer* character = new SpriteRenderer(associated, sprite, 3, 4);
    characterSprite = new SpriteRenderer(associated, sprite, 6, 5);
    characterSprite->SetScale(0.08,0.08);
    associated.AddComponent(characterSprite);

    Animator* animator = new Animator(associated);

    animator->AddAnimation("idle", Animation(0, 2, 0.1));
    animator->AddAnimation("walking", Animation(4, 11, 0.1));
    animator->AddAnimation("dead", Animation(25, 25, 0));
    animator->AddAnimation("jump", Animation(16, 16, 0.1));
    animator->AddAnimation("falling", Animation(23, 23, 0));
    animator->AddAnimation("wallGrab", Animation(28, 28, 0));
    animator->AddAnimation("dash", Animation(12, 15, 0.05));
    animator->AddAnimation("hit", Animation(26, 26, 0));


    animator->SetAnimation("idle");
    associated.AddComponent(animator);

    Collider* collider = new Collider(associated);
    //collider->SetScale(Vec2(0.5, 0.5));
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

        if (task.type == Command::MOVE && !dashing && !isHit) {
            if (task.pos == Vec2(0,0)) {
                moving = false;
            } else {
                moving = true;
                direction = task.pos;
                float accel;
                if (onGround) {
                    accel = groundAcceleration;
                } else
                    accel = airAcceleration;
                speed = speed + direction * accel * dt;
                if (speed.X > maxGroundSpeed)
                    speed.X = maxGroundSpeed;
                if (speed.X < -maxGroundSpeed)
                    speed.X = -maxGroundSpeed;
            }
        } else if (task.type == Command::SHOOT && !dashing && !isHit) {
            Component* component = gun.lock()->GetComponent("Gun");
            Gun* gunCpt = dynamic_cast<Gun*>(component);
            gunCpt->Shot(task.pos);
        } else if (task.type == Command::JUMP && !dashing && !isHit) {
            moving = false;
            if (canJump) {
                speed.Y = jumpSpeed;
                onGround = false;
                canJump = false;
                animator->SetAnimation("jump");
                if (onWall)
                    speed.X = maxGroundSpeed*2*direction.X*-1;
            } else if (canDoubleJump) {
                speed.Y = jumpSpeed;
                canDoubleJump = false;
                animator->SetAnimation("jump");
            }
        } else if (task.type == Command::DASH && canDash  && !isHit) {
            if (task.pos.X != 0) {
                speed = task.pos * dashSpeed;
                canDash = false;
                dashing = true;
                dashTimer.Restart();
                speed.Y = 0;
                moving = false;
                animator->SetAnimation("dash");
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
        damageCooldown.Update(dt);
    }

    dashTimer.Update(dt);
    if (dashing && dashTimer.Get() <= dashDuration) {
        Rect new_box_x = associated.box + Vec2(speed.X * dt, 0);
        if (tileMap->IsColliding(new_box_x).size() == 0) {
            associated.box = new_box_x;
        } else {
            dashing = false;
        }
    }
    if (dashTimer.Get() > dashDuration) {
        dashing = false;
        if (dashTimer.Get() > 4)
            canDash = true;
    }

    hitTimer.Update(dt);
    if (hitTimer.Get() <= 0.5 and isHit) {
        Rect new_box_x = associated.box + Vec2(speed.X * dt,0);
        if (tileMap->IsColliding(new_box_x).size() == 0) {
            associated.box = new_box_x;
            animator->SetAnimation("hit");
        } else {
            isHit = false;
        }
    }
    if (hitTimer.Get() > 0.5 and isHit) {
        isHit = false;
    }

    if (!dashing && !isHit) {
        if (!moving and speed.X != 0) {
            float friction;
            if (onGround) {
                friction = groundAcceleration;
            } else
                friction = airAcceleration;
            speed = speed - direction * friction * dt;
            if (speed.X < 0 and direction.X > 0) speed.X = 0;
            if (speed.X > 0 and direction.X < 0) speed.X = 0;
        }
        Rect new_box_x = associated.box + Vec2(speed.X * dt, 0);

        vector<TileMap::CollisionInfo> collisions = tileMap->IsColliding(new_box_x);
        if (collisions.size() == 0) {
            onWall = false;
            associated.box = new_box_x;
            if (speed.X < 0)
                characterSprite->SetFlip(SDL_FLIP_HORIZONTAL);
            else if (speed.X > 0)
                characterSprite->SetFlip(SDL_FLIP_NONE);
        } else {
            bool TopLeft = false, TopRight = false, BottomLeft = false, BottomRight = false, CenterBottom = false, CenterTop = false, CenterLeft = false, CenterRight = false;
            for (TileMap::CollisionInfo collision : collisions) {
                if (collision.type == TileMap::TileCollisionType::Full) {
                    speed.X = 0;
                    if (collision.corner == TileMap::CollisionCorner::TopRight or collision.corner == TileMap::CollisionCorner::BottomRight) {
                        associated.box.X = (collision.tilePos.X * tileMap->GetTileSetWidth())  - associated.box.W - 0.01;
                    } else if (collision.corner == TileMap::CollisionCorner::TopLeft or collision.corner == TileMap::CollisionCorner::BottomLeft) {
                        associated.box.X = (collision.tilePos.X * tileMap->GetTileSetWidth()) + tileMap->GetTileSetWidth() + 0.01;
                    }
                }
                switch (collision.corner) {
                    case TileMap::CollisionCorner::TopLeft:
                        TopLeft = true;
                        break;
                    case TileMap::CollisionCorner::TopRight:
                        TopRight = true;
                        break;
                    case TileMap::CollisionCorner::BottomLeft:
                        BottomLeft = true;
                        break;
                    case TileMap::CollisionCorner::BottomRight:
                        BottomRight = true;
                        break;
                    case TileMap::CollisionCorner::CenterBottom:
                        CenterBottom = true;
                        break;
                    case TileMap::CollisionCorner::CenterTop:
                        CenterTop = true;
                        break;
                    case TileMap::CollisionCorner::CenterRight:
                        CenterRight = true;
                        break;
                    case TileMap::CollisionCorner::CenterLeft:
                        CenterLeft = true;
                        break;
                }
            }
            if ((TopLeft or CenterLeft or BottomLeft) and (not CenterBottom and not BottomRight)) {
                animator->SetAnimation("wallGrab");
                onWall = true;
                canJump = true;
            } else if ((TopRight or CenterRight or BottomRight) and (not CenterBottom and not BottomLeft)) {
                animator->SetAnimation("wallGrab");
                onWall = true;
                canJump = true;
            } else {
                onWall = false;
            }
        }
    }

    if (!dashing) {
        float gravity;
        if (onWall and speed.Y >= 0)
            gravity = wallGravity;
        else
            gravity = airGravity;

        // calculo da gravidade do personagem
        speed.Y = speed.Y + gravity * dt;
        // speedcap de queda
        if (speed.Y > maxFallSpeed)
            speed.Y = maxFallSpeed;
        Rect new_box_y = associated.box + Vec2(0, speed.Y * dt);
        vector<TileMap::CollisionInfo> collisions = tileMap->IsColliding(new_box_y);
        if (collisions.size() > 0) {
            for (TileMap::CollisionInfo collision : collisions) {
                if (collision.corner == TileMap::CollisionCorner::TopRight or collision.corner == TileMap::CollisionCorner::TopLeft) {
                    speed.Y = 0;
                    animator->SetAnimation("falling");
                }
                if (collision.corner == TileMap::CollisionCorner::BottomRight or collision.corner == TileMap::CollisionCorner::BottomLeft) {
                    speed.Y = 0;
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
            associated.box = associated.box + Vec2(0, speed.Y * dt);
            onGround = false;
            if (not onWall)
                canJump = false;
            if (speed.Y < 0 and not isHit)
                animator->SetAnimation("jump");
            else if (not onWall and not isHit)
                animator->SetAnimation("falling");
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

    if (direction.X < 0) {
        characterSprite->SetFlip(SDL_FLIP_HORIZONTAL);
    } else if (direction.X > 0) {
        characterSprite->SetFlip(SDL_FLIP_NONE);
    }
    if (hp > 0 and onGround and !dashing) {
        if (speed.X != 0)
            animator->SetAnimation("walking");
        else if (speed.X == 0 and not moving)
            animator->SetAnimation("idle");
    }

    InputManager& input = InputManager::GetInstance();
    if (input.KeyPress(SDLK_e) and !graffitiArray.empty()) {
        id = graffitiArray.size() - 1;
        SDL_Texture* texture = graffitiArray[id];
        GameObject* graffitiObj = new GameObject(true);
        SpriteRenderer* graffiti = new SpriteRenderer(*graffitiObj, texture, to_string(id));
        graffitiObj->AddComponent(graffiti);
        graffiti->SetScale(0.3f, 0.3f);
        graffitiObj->box.X = associated.box.X + associated.box.W/2 - graffitiObj->box.W/2;
        graffitiObj->box.Y = associated.box.Y - graffitiObj->box.H;
        graffiti->Render();

        State& state = Game::GetInstance().GetState();
        state.AddObject(graffitiObj);
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
                speed.Y = jumpSpeed/1.5;
                canJump = false;
                canDoubleJump = false;
                canDash = false;
                dashing = false;
                dashTimer.Restart();
                isHit = true;
                hitTimer.Restart();
                direction = (associated.box.X - other.box.X > 0) ? Vec2(1,0) : Vec2(-1,0);
                speed.X = direction.X * maxGroundSpeed;
                onGround = false;
                moving = false;
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
        if (!itemCpt->IsCollected()) {
            itemCpt->TryCollect();
            itemCpt->SetCollected();
            ItemData itemData = itemCpt->GetItemData();
            
            switch (itemData.type) {
                case ItemData::Type::Minikit: {
                    GameObject* inventoryItemObject = new GameObject();

                    InventoryItem* inventoryItemCpt = new InventoryItem(*inventoryItemObject, itemData);
                    inventoryItemObject->AddComponent(inventoryItemCpt);
                    
                    inventory.push_back(inventoryItemObject);

                    break;
                }
                    
                case ItemData::Type::SprayCap:

                    break;
                
                    
                case ItemData::Type::SprayBody:

                    break;
                    
                case ItemData::Type::SprayColor:

                    break;
        
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

int Character::GetInventorySize() {
    return inventory.size();
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

void Character::AddGraffiti(SDL_Texture *texture) {
    auto renderer = Game::GetInstance().GetRenderer();

    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

    // Define o blend mode correto antes de ler pixels
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // Cria uma surface temporária com canal alfa
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA8888);

    // Lê os pixels da canvasTexture
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_RGBA8888, surface->pixels, surface->pitch);
    SDL_SetRenderTarget(renderer, nullptr);

    // Cria uma nova textura que suportará transparência
    SDL_Texture* newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, w, h);
    SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND); // <- IMPORTANTE!

    // Atualiza a nova textura com os dados da surface
    SDL_UpdateTexture(newTexture, nullptr, surface->pixels, surface->pitch);

    SDL_FreeSurface(surface);

    graffitiArray.emplace_back(newTexture);
}

float Character::GetDamageCooldownTimer() {
    return damageCooldown.Get();
}

