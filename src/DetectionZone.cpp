#include "../include/DetectionZone.hpp"

DetectionZone::DetectionZone(GameObject &associated): Component(associated) {
    isCollidingThisFrame = false;
    wasCollidingLastFrame = false;
    collider = new Collider(associated);
    associated.AddComponent(collider);
}

DetectionZone::~DetectionZone() = default;

void DetectionZone::Update(float dt) {
    if (isCollidingThisFrame == false and wasCollidingLastFrame == true) {
        if (None) {
            None();
        }
    }
    wasCollidingLastFrame = isCollidingThisFrame;
    isCollidingThisFrame = false;
}
void DetectionZone::Render() {}
bool DetectionZone::Is(string type) {
    return type == "DetectionZone";
}
void DetectionZone::Start() {}

void DetectionZone::NotifyCollision(GameObject &other) {
    if (wasCollidingLastFrame == false and isCollidingThisFrame == false) {
        if (Detect) {
            Detect();
        }
    }
    isCollidingThisFrame = true;
}

void DetectionZone::SetDetectFunction(function<void()> func) {
    Detect = func;
}

void DetectionZone::SetNoneFunction(function<void()> func) {
    None = func;
}
