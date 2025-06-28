#ifndef OPTIONSSTATE_HPP
#define OPTIONSSTATE_HPP

#include "KeyBindingManager.hpp"
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

    static KeyBindingManager::GameAction actionToRebind;

private:
    Text* soundText;
    Text* musicText;
    Text* jumpText;
    Text* leftText;
    Text* rightText;
    Text* dashText;
    Text* notebookText;
    Text* graffitiText;
};

#endif //OPTIONSSTATE_HPP
