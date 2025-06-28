#ifndef OPTIONSSTATE_HPP
#define OPTIONSSTATE_HPP

#include "State.hpp"
#include "../include/Text.hpp"

class OptionsState : public State {
public:
    OptionsState();
    ~OptionsState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();

private:
    Text* soundText;
    Text* musicText;
};

#endif //OPTIONSSTATE_HPP
