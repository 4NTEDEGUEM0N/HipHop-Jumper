#ifndef TITLESTATE_HPP
#define TITLESTATE_HPP
#include "State.hpp"

class TitleState : public State {
public:
    TitleState();
    ~TitleState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();

private:
    bool up;
    int counter;
    weak_ptr<GameObject> instructionsObject;
    GameObject* backgroudImage;
};

#endif //TITLESTATE_HPP
