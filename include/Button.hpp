#ifndef BUTTON_HPP
#define BUTTON_HPP
#include <functional>

#include "Component.hpp"
#include "Sound.hpp"

using namespace std;

class Button : public Component {
private:
    bool clickable;
public:
    Button(GameObject& associated);
    void Update(float dt);
    void Render();
    bool Is(string type);

    function<void()> Click;
    function<void()> Hover;
    function<void()> None;
    
    Sound clickSound;
    Sound hoverSound;
    
    bool hoverSoundPlayed;

    void SetClickFunction(function<void()> func);
    void SetHoverFunction(function<void()> func);
    void SetNoneFunction(function<void()> func);
};

#endif //BUTTON_HPP
