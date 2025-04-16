#ifndef AICONTROLLER_HPP
#define AICONTROLLER_HPP
#include "Component.hpp"
#include "Timer.hpp"
#include "Vec2.hpp"

class AIController : public Component {
private:
    int npcCounter;
    enum AIState {
        MOVING,
        RESTING
    };
    AIState state;
    Timer restTimer;
    Vec2 destination;

public:
    AIController(GameObject& associated);
    void Update(float dt);
    void Render();
    bool Is(string type);
};

#endif //AICONTROLLER_HPP
