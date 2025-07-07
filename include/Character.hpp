#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include <memory>
#include <queue>
#include <unordered_map>

#include "Component.hpp"
#include "Timer.hpp"
#include "Vec2.hpp"
#include "Sound.hpp"
#include "SpriteRenderer.hpp"

using namespace std;

class Character : public Component {
public:
    class Command {
    public:
        enum CommandType {
            MOVE,
            SHOOT,
            JUMP,
            DASH
        };
        CommandType type;
        Vec2 pos;

        Command(CommandType type, float x, float y);

    };

private:
    SpriteRenderer* characterSprite;
    weak_ptr<GameObject> gun;
    vector<GameObject> inventory;
    queue<Command> taskQueue;
    Vec2 speed;
    int hp;
    Timer deathTimer;
    bool dead;
    Timer damageCooldown;
    Sound deathSound;
    Sound hitSound;
    Sound spraySound;
    Sound jumpSound;
    Sound doublejumpSound;
    Sound dashSound;
    Sound stepSound;
    Sound landSound;
    Sound grabSound;

    bool onGround;
    Vec2 direction;
    float airGravity;
    float wallGravity;
    float groundAcceleration;
    float airAcceleration;
    float maxGroundSpeed;
    float maxFallSpeed;
    float jumpSpeed;
    float dashSpeed;
    bool moving;
    bool onWall;
    bool isSliding;

    bool canJump;
    bool canDoubleJump;
    bool canDash;
    bool dashing;
    Timer dashTimer;
    float dashDuration;
    bool isHit;
    Timer hitTimer;
    Timer wallJumpCooldown;
    bool isStepSoundPlaying;

    Vec2 colliderScale;

public:
    static Character* player;

    Character(GameObject& associated, string sprite);
    ~Character();
    void Start();
    void Update(float dt);
    void Render();
    bool Is(string type);
    void NotifyCollision(GameObject &other);
    void Issue(Command task);
    Vec2 GetPosition() const;
    int GetInventorySize();
    int GetHP();
    bool CanJump();
    bool CanDoubleJump();
    bool CanDash();
    float GetDamageCooldownTimer();

    static void AddGraffiti(SDL_Texture* texture);
    static int currentGraffitiId;
    static vector<SDL_Texture*> graffitiArray;
    static void SetCurrentGraffitiId(int id);


};

#endif //CHARACTER_HPP
