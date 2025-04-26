#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "GameObject.hpp"

using namespace std;

class Camera {
private:
    static GameObject* focus;

public:
    static Vec2 pos;
    static Vec2 speed;
    static int speedMultiplier;

    static void Follow(GameObject* newFocus);
    static void Unfollow();
    static void Update(float dt);
};


#endif //CAMERA_HPP
