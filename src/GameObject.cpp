#include "../include/GameObject.hpp"


GameObject::GameObject(bool isStatic) {
    isDead = false;
    started = false;
    angleDeg = 0;
    this->isStatic = isStatic;
}

GameObject::~GameObject() {
    for (int i = components.size() - 1; i >= 0; i--) {
        delete components[i];
    }
    components.clear();
}

void GameObject::Update(float dt) {
    for (int i = 0; i < components.size(); i++) {
        components[i]->Update(dt);
    }
}

void GameObject::Render() {
    for (int i = 0; i < components.size(); i++) {
        components[i]->Render();
    }
}

bool GameObject::IsDead() {
    return isDead;
}

void GameObject::RequestDelete() {
    isDead = true;
}

void GameObject::AddComponent(Component* cpt) {
    components.push_back(cpt);

    if (started) {
        cpt->Start();
    }
}

void GameObject::RemoveComponent(Component* cpt) {
    for (int i = components.size() - 1; i >= 0; i--) {
        if (components[i] == cpt) {
            delete components[i];
            components.erase(components.begin() + i);
            break;
        }
    }
}

Component* GameObject::GetComponent(string type) {
    for (int i = 0; i < components.size(); i++) {
        if (components[i]->Is(type)) {
            return components[i];
        }
    }
    return nullptr;
}

void GameObject::Start() {
    for (int i = 0; i < components.size(); i++) {
        components[i]->Start();
    }

    started = true;
}

void GameObject::NotifyCollision(GameObject& other) {
    for (int i = 0; i < components.size(); i++) {
        components[i]->NotifyCollision(other);
    }
}
