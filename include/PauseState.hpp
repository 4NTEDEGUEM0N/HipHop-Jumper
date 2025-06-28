#ifndef PAUSESTATE_HPP
#define PAUSESTATE_HPP

#include "State.hpp"
#include "Text.hpp"

class PauseState : public State {
public:
    PauseState();
    ~PauseState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();

private:
    Text* resumeText;
    Text* optionsText;
    Text* menuText;

};

#endif //PAUSESTATE_HPP
