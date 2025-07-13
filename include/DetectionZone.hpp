#ifndef DETECTIONZONE_HPP
#define DETECTIONZONE_HPP
#include "Component.hpp"
#include <functional>

#include "Collider.hpp"

class DetectionZone : public Component {
private:
    bool isCollidingThisFrame;
    bool wasCollidingLastFrame;
public:
    DetectionZone(GameObject& associated);
    ~DetectionZone();

    void Update(float dt);
    void Render();
    bool Is(string type);
    void Start();
    void NotifyCollision(GameObject& other);

    Collider* collider;
    function<void()> Detect;
    function<void()> None;
    void SetDetectFunction(function<void()> func);
    void SetNoneFunction(function<void()> func);
};

#endif //DETECTIONZONE_HPP
