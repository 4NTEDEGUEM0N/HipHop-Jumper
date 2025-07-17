#ifndef TITLESTATE_HPP
#define TITLESTATE_HPP

#include "Music.hpp"
#include "State.hpp"
#include "Text.hpp"

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
    Text* playText;
    Text* optionsText;
    Text* creditosText;
    Text* quitText;
    bool quit = false;
    Music backgroundMusic;

};

#endif //TITLESTATE_HPP
