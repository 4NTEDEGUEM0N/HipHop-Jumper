#ifndef ENDSTATE_HPP
#define ENDSTATE_HPP

#include "Music.hpp"
#include "State.hpp"

class EndState : public State {
public:
    EndState();
    ~EndState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();

private:
    Music backgroundMusic;
    bool up;
    int counter;
    weak_ptr<GameObject> quitInstructionsObject;
    weak_ptr<GameObject> playInstructionsObject;
    GameObject* backgroudImage;
};

#endif //ENDSTATE_HPP
