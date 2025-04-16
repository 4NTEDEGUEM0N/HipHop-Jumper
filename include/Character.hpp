#ifndef CHARACTER_HPP
#define CHARACTER_HPP
#include <memory>
#include <queue>

#include "Component.hpp"
#include "Timer.hpp"
#include "Vec2.hpp"
#include "Sound.hpp"

using namespace std;

class Character : public Component {
public:
    class Command {
    public:
        enum CommandType {
            MOVE,
            SHOOT
        };
        CommandType type;
        Vec2 pos;

        Command(CommandType type, float x, float y);

    };

private:
    weak_ptr<GameObject> gun;
    queue<Command> taskQueue;
    Vec2 speed;
    float linearSpeed;
    int hp;
    Timer deathTimer;
    bool dead;
    Timer damageCooldown;
    Sound deathSound;
    Sound hitSound;

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


};

#endif //CHARACTER_HPP
