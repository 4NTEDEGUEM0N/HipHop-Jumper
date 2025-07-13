#include "../include/Character.hpp"

#include <iostream>
#include <cmath>

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
#include "../include/KeyBindingManager.hpp"
#include "../include/NPC.hpp"


Character* Character::player = nullptr;
vector<SDL_Texture*> Character::graffitiArray;
int Character::currentGraffitiId = -1;

Character::Character(GameObject& associated, string sprite) : Component(associated), deathSound("../Recursos/audio/MOVIMENTOS/FIGHT VOCALIZE MALE HURT.wav"),
    hitSound("../Recursos/audio/MOVIMENTOS/FIGHT VOCALIZE MALE.wav"),
    spraySound("../Recursos/audio/GRAFFITI/CANSHAKESPRAY.wav"),
    jumpSound("../Recursos/audio/MOVIMENTOS/JUMP1.wav"),
    doublejumpSound("../Recursos/audio/MOVIMENTOS/DOUBLEJUMP1.wav"),
    dashSound("../Recursos/audio/MOVIMENTOS/CLOTHWHOOSH.wav"),
    stepSound("../Recursos/audio/PASSOS MOVIMENTOS DIVERSOS/PASSOS CONCRETO 1 DEVAGAR.wav"),
    landSound("../Recursos/audio/MOVIMENTOS/JUMPFINISH.wav"),
    grabSound("../Recursos/audio/MOVIMENTOS/WHOOSH .wav")
{
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
    isSliding = false;
    
    direction = {1,0};


    //SpriteRenderer* character = new SpriteRenderer(associated, sprite, 3, 4);
    characterSprite = new SpriteRenderer(associated, sprite, 5, 5);
    //characterSprite->SetScale(0.08,0.08);
    associated.AddComponent(characterSprite);

    Animator* animator = new Animator(associated);

    animator->AddAnimation("idle", Animation(0, 2, 0.1));
    animator->AddAnimation("walking", Animation(4, 11, 0.1));
    animator->AddAnimation("dead", Animation(23, 23, 0));
    animator->AddAnimation("jump", Animation(16, 16, 0.1));
    animator->AddAnimation("falling", Animation(19, 19, 0));
    animator->AddAnimation("wallGrab", Animation(24, 24, 0));
    animator->AddAnimation("dash", Animation(12, 15, 0.05));
    animator->AddAnimation("hit", Animation(22, 22, 0));


    animator->SetAnimation("idle");
    associated.AddComponent(animator);

    colliderScale = Vec2(0.6, 0.8);
    Collider* collider = new Collider(associated);
    collider->SetScale(colliderScale);
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
    if (GameData::ended) {
        speed.X = 0;
        direction.X = 1;
    }
    Component* component = associated.GetComponent("Animator");
    Animator* animator = dynamic_cast<Animator*>(component);

    GameObject* tileMapObject = Game::GetInstance().GetState().GetTileMapObject();
    Component* tileMapComponent = tileMapObject->GetComponent("TileMap");
    TileMap* tileMap = dynamic_cast<TileMap*>(tileMapComponent);

    while(!taskQueue.empty()) {
        Command task = taskQueue.front();
        taskQueue.pop();

        if (task.type == Command::MOVE && !dashing && !isHit && !isSliding) {
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
            if (canJump) {
                jumpSound.Play(1);
                speed.Y = jumpSpeed;
                onGround = false;
                canJump = false;
                animator->SetAnimation("jump");
                if (onWall) {
                    speed.X = maxGroundSpeed*2*direction.X*-1;
                    speed.Y = jumpSpeed*3/4;
                    wallJumpCooldown.Restart();
                }
            } else if (canDoubleJump) {
                doublejumpSound.Play(1);
                speed.Y = jumpSpeed;
                canDoubleJump = false;
                onGround = false;
                animator->SetAnimation("jump");
            }
        } else if (task.type == Command::DASH && canDash  && !isHit) {
            if (task.pos.X != 0) {
                speed = task.pos * dashSpeed;
                canDash = false;
                dashing = true;
                dashTimer.Restart();
                speed.Y = 0;
                dashSound.Play(1);
                animator->SetAnimation("dash");
            }
        }

        if (hp <= 0 && !dead) {
            
            State& state = Game::GetInstance().GetState();
            GameObject* deathObject = new GameObject(false);
            state.AddObject(deathObject);
            
            SpriteRenderer* deathSpriteRenderer = new SpriteRenderer(*deathObject, "../Recursos/img/morte.png",2,2);
            deathSpriteRenderer->SetScale(0.10, 0.10);
            
            deathObject->box.X = associated.box.X;// - playerObject.box.W/2 - playerObject->box.W/2;
            deathObject->box.Y = associated.box.Y;// - playerObject.box.H/2 - deathObject->box.H/2;
            deathObject->AddComponent(deathSpriteRenderer);
            
            Animator* deathAnimator = new Animator(*deathObject);
            deathAnimator->AddAnimation("morte", Animation(0, 3, 0.05));
            deathObject->AddComponent(deathAnimator);
            
            deathAnimator->SetAnimation("morte");
            
            characterSprite->SetScale(0.0001, 0.0001);
            
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
        if (tileMap->IsColliding(new_box_x, colliderScale).size() == 0) {
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
        if (tileMap->IsColliding(new_box_x, colliderScale).size() == 0) {
            associated.box = new_box_x;
            animator->SetAnimation("hit");
        } else {
            isHit = false;
        }
    }
    if (hitTimer.Get() > 0.5 and isHit) {
        isHit = false;
    }

    wallJumpCooldown.Update(dt);
    if (!dashing && !isHit) {
        if (!moving and speed.X != 0) {
            float friction;
            if (onGround) {
                friction = groundAcceleration;
            } else
                friction = airAcceleration;
            speed = speed - direction * friction * dt;
            if (wallJumpCooldown.Get() > 0.15f) {
                if (speed.X < 0 and direction.X > 0) {
                    speed.X = 0;
                    moving = false;
                }
                if (speed.X > 0 and direction.X < 0) {
                    speed.X = 0;
                    moving = false;
                }
            }
        }
        Rect new_box_x = associated.box + Vec2(speed.X * dt, 0);
        vector<TileMap::CollisionInfo> x_collisions = tileMap->IsColliding(new_box_x, colliderScale);

        if (x_collisions.empty()) {
            associated.box = new_box_x;
            onWall = false; // Se não há colisão em X, não estamos em uma parede
            if (speed.X < 0) characterSprite->SetFlip(SDL_FLIP_HORIZONTAL);
            else if (speed.X > 0) characterSprite->SetFlip(SDL_FLIP_NONE);
        } else {
            float originalSpeedX = speed.X;
            speed.X = 0; // Para o movimento horizontal
            bool wallCollision = false;

            for (const auto& col : x_collisions) {
                if (col.type == TileMap::TileCollisionType::Full) {
                    wallCollision = true;
                    float colliderWidth = associated.box.W * colliderScale.X;
                    float offsetSide = (associated.box.W - colliderWidth) / 2.0f;

                    // Se estava indo para a direita (velocidade positiva)
                    if (originalSpeedX > 0) {
                        //associated.box.X = (col.tilePos.X * tileMap->GetTileSetWidth()) - associated.box.W - 0.01f;
                        float tileLeftX = col.tilePos.X * tileMap->GetTileSetWidth();
                        associated.box.X = tileLeftX - colliderWidth - offsetSide - 0.01f;
                    }
                    // Se estava indo para a esquerda (velocidade negativa)
                    else if (originalSpeedX < 0) {
                        //associated.box.X = (col.tilePos.X * tileMap->GetTileSetWidth()) + tileMap->GetTileSetWidth() + 0.01f;
                        float tileRightX = (col.tilePos.X * tileMap->GetTileSetWidth()) + tileMap->GetTileSetWidth();
                        associated.box.X = tileRightX - offsetSide + 0.01f;
                    }

                    // Uma vez que ajustamos a posição por um tile sólido, podemos parar de verificar.
                    // Isso evita ajustes múltiplos se a box tocar dois tiles ao mesmo tempo.
                    break;
                }
            }

            if (wallCollision && !onGround) { // Só ativa 'onWall' se estiver no ar
                if (onWall == false) grabSound.Play(1);
                onWall = true;
                canJump = true; // Permite o Wall Jump
                animator->SetAnimation("wallGrab");
            }
        }
    }

    if (!dashing) {
        float gravity = 0;
        if (onWall && speed.Y >= 0 && !isSliding) gravity = wallGravity;
        else if (!isSliding) gravity = airGravity;

        speed.Y += gravity * dt;
        if (speed.Y > maxFallSpeed) speed.Y = maxFallSpeed;

        Rect new_box_y = associated.box + Vec2(0, speed.Y * dt);
        vector<TileMap::CollisionInfo> y_collisions = tileMap->IsColliding(new_box_y, colliderScale);

        if (y_collisions.empty()) {
            isSliding = false;
            associated.box = new_box_y;
            onGround = false;
            if (!onWall) canJump = false;

            if (speed.Y < 0 && !isHit) animator->SetAnimation("jump");
            else if (!onWall && !isHit) animator->SetAnimation("falling");

        } else {
            // Determina se a colisão foi com o chão ou com o teto
            bool landed = false;
            bool hitCeiling = false;
            isSliding = false;

            for (const auto& col : y_collisions) {
                // Se a velocidade Y é positiva, estamos caindo.
                if (speed.Y > 0) {
                    landed = true;
                    float colliderHeight = associated.box.H * colliderScale.Y;
                    float verticalOffset = (associated.box.H - colliderHeight) / 2.0f;
                    float colliderWidth = associated.box.W * colliderScale.X;
                    float horizontalOffset = (associated.box.W - colliderWidth) / 2.0f;
                    // Lógica para rampas (a sua já era boa!)
                    if (col.type == TileMap::TileCollisionType::TriangleTopLeft) {
                        isSliding = true;
                        speed.Y = maxFallSpeed;
                        speed.X = -maxFallSpeed;
                        direction = Vec2(-1, 0);

                        float tileY = col.tilePos.Y * tileMap->GetTileSetHeight();
                        float tileX = col.tilePos.X * tileMap->GetTileSetWidth();
                        float rampY = -((float)tileMap->GetTileSetHeight()/(float)tileMap->GetTileSetWidth()) * (associated.box.X + associated.box.W - horizontalOffset) + (tileY + tileMap->GetTileSetHeight() + ((float)tileMap->GetTileSetHeight()/(float)tileMap->GetTileSetWidth())*tileX);
                        associated.box.Y = rampY - colliderHeight - verticalOffset - 0.01;

                        landed = false;
                        canJump = false;
                        canDoubleJump = false;
                        canDash = false;
                        dashTimer.Restart();
                        break;
                    } else if (col.type == TileMap::TileCollisionType::TriangleTopRight) {
                        isSliding = true;
                        speed.Y = maxFallSpeed;
                        speed.X = maxFallSpeed;
                        direction = Vec2(1, 0);

                        float tileY = col.tilePos.Y * tileMap->GetTileSetHeight();
                        float tileX = col.tilePos.X * tileMap->GetTileSetWidth();
                        float rampY = ((float)tileMap->GetTileSetHeight()/(float)tileMap->GetTileSetWidth()) * (associated.box.X + horizontalOffset) + (tileY - ((float)tileMap->GetTileSetHeight()/(float)tileMap->GetTileSetWidth())*tileX);
                        associated.box.Y = rampY - colliderHeight - verticalOffset - 0.01;

                        landed = false;
                        canJump = false;
                        canDoubleJump = false;
                        canDash = false;
                        dashTimer.Restart();
                        break;
                    }
                    // Para tiles normais, apenas ajusta a posição
                    else {
                        landed = true;
                        //associated.box.Y = (col.tilePos.Y * tileMap->GetTileSetHeight()) - (associated.box.H*colliderScale.Y) - 0.01;
                        float colliderHeight = associated.box.H * colliderScale.Y;
                        // O centro do GameObject deve ficar a meia altura do colisor acima do chão do tile.
                        associated.box.Y = (col.tilePos.Y * tileMap->GetTileSetHeight()) - (associated.box.H / 2.0f) - (colliderHeight / 2.0f);
                        // Uma pequena correção para garantir que não haja sobreposição.
                        associated.box.Y -= 0.01f;
                    }
                }
                // Se a velocidade Y é negativa, estamos subindo
                else if (speed.Y < 0) {
                    hitCeiling = true;
                    associated.box.Y = (col.tilePos.Y * tileMap->GetTileSetHeight()) + tileMap->GetTileSetHeight() + 0.01;
                }
            }

            if (landed) {
                if (!onGround) landSound.Play(1);
                onGround = true;
                canJump = true;
                canDoubleJump = true;
                if (!isSliding) {
                    speed.Y = 0;
                }
            }

            if (hitCeiling) {
                speed.Y = 0;
                animator->SetAnimation("falling");
            }
        }
    }

    deathTimer.Update(dt);
    //if (dead && deathTimer.Get() > 1) {
    //    deathObject.RequestDelete();
    //}
    
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
    if (hp > 0 && !dashing) {
        // NOVO: Condição para animação de deslize
        if (isSliding) {
            animator->SetAnimation("falling"); // Ou "falling" se não tiver uma específica
            // Ajusta a direção do sprite durante o deslize
            if (speed.X < 0) {
                characterSprite->SetFlip(SDL_FLIP_HORIZONTAL);
            } else if (speed.X > 0) {
                characterSprite->SetFlip(SDL_FLIP_NONE);
            }
        } else if (onGround) {
            if (speed.X != 0)
                animator->SetAnimation("walking");
            else if (speed.X == 0 && !moving)
                animator->SetAnimation("idle");
        }
    }
    
    bool shouldPlayStep = onGround && fabs(speed.X) > 10.0f && !dashing && !isHit && !dead;

    //bool shouldPlayStep = onGround && moving && !dashing && !isHit && !dead;

    
    if (shouldPlayStep) {
        stepSound.Play(-1);  // loop
    } else {
        stepSound.Stop();
    }
    
//    if (shouldPlayStep) {
//        if (!stepSound.IsPlaying()) {
//            stepSound.Play(-1);  // Toca em loop infinito
//        }
//    } else {
//        if (stepSound.IsPlaying()) {
//            stepSound.Stop();  // Para quando necessário
//        }
//    }

    KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
    if (keybinder.IsActionPressed(KeyBindingManager::GameAction::GRAFFITI) and !graffitiArray.empty()) {
        SDL_Texture* texture = graffitiArray[currentGraffitiId];
        GameObject* graffitiObj = new GameObject(true);
        SpriteRenderer* graffiti = new SpriteRenderer(*graffitiObj, texture, to_string(currentGraffitiId));
        graffitiObj->AddComponent(graffiti);
        graffiti->SetScale(0.3f, 0.3f);
        graffitiObj->box.X = associated.box.X + associated.box.W/2 - graffitiObj->box.W/2;
        graffitiObj->box.Y = associated.box.Y - graffitiObj->box.H;
        graffiti->Render();
        spraySound.Play(); //randomize sound?

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
    if (other.GetComponent("NPC") != nullptr) {
        InputManager& input = InputManager::GetInstance();
        Component* cmp = other.GetComponent("NPC");
        NPC* npcCpt = dynamic_cast<NPC*>(cmp);
        if (!npcCpt->hasDialog) {
            stepSound.Stop();
        }
        if (input.KeyPress(SDLK_RETURN)) {
            npcCpt->RepeatDialog();
            stepSound.Stop();
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
    currentGraffitiId = graffitiArray.size() - 1;
    
}

void Character::SetCurrentGraffitiId(int id) {
    currentGraffitiId = id;
}


float Character::GetDamageCooldownTimer() {
    return damageCooldown.Get();
}

