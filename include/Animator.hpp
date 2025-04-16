#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include <string>
#include <unordered_map>
#include "Animation.hpp"
#include "GameObject.hpp"

using namespace std;

class Animator: public Component {
private:
    int frameStart;
    int frameEnd;
    float frameTime;
    int currentFrame;
    float timeElapsed;
    string currentAnimation;
    unordered_map<string,Animation> animations;

public:
    Animator(GameObject& associated);
    void Update(float dt);
    void Render();
    bool Is(string type);
    void SetAnimation(string name);
    void AddAnimation(string name, Animation anim);
    string GetAnimation();
};

#endif //ANIMATOR_HPP
