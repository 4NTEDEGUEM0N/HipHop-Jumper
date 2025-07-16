#ifndef ENDSTATE_HPP
#define ENDSTATE_HPP

#include "Music.hpp"
#include "State.hpp"
#include "Text.hpp"

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
    Text* nextText;
    Text* menuText;
};

#endif //ENDSTATE_HPP
