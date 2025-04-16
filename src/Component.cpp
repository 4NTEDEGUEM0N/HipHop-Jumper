#include "../include/Component.hpp"

Component::Component(GameObject& associated):associated(associated) {};

Component::~Component() = default;

void Component::Start() {};

void Component::NotifyCollision(GameObject &other) {};
