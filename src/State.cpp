#include "../include/State.hpp"
#include "../include/Collider.hpp"
#include "../include/Collision.hpp"

#include <algorithm>
#include <iostream>
#include <math.h>

State::State() {
    popRequested = false;
    quitRequested = false;
    started = false;
}

State::~State() {
    objectArray.clear();
}

weak_ptr<GameObject> State::AddObject(GameObject* go) {
    shared_ptr<GameObject> goPtr(go);
    objectArray.push_back(goPtr);

    if (started) {
        go->Start();
    }

    return weak_ptr<GameObject>(goPtr);
}

weak_ptr<GameObject> State::GetObjectPtr(GameObject* go) {
    for (int i = 0; i < objectArray.size(); i++) {
        if (objectArray[i].get() == go) {
            return weak_ptr<GameObject>(objectArray[i]);
        }
    }

    return weak_ptr<GameObject>();
}

bool State::PopRequested() {
    return popRequested;
}

bool State::QuitRequested() {
    return quitRequested;
}

void State::StartArray() {
    LoadAssets();

    for (int i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Start();
    }

    started = true;
}

void State::UpdateArray(float dt) {
    for(int i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Update(dt);
    }

    for(int i = 0; i < objectArray.size(); i++) {
        if (objectArray[i]->IsDead()) {
            objectArray.erase(objectArray.begin() + i);
            i--;
        }
    }

    for(int i = 0; i < objectArray.size(); i++) {
        Collider* colliderA = dynamic_cast<Collider*>(objectArray[i]->GetComponent("Collider"));
        if (colliderA != nullptr) {
            for (int j = i + 1; j < objectArray.size(); j++) {
                Collider* colliderB = dynamic_cast<Collider*>(objectArray[j]->GetComponent("Collider"));
                if (colliderB != nullptr) {
                    bool collision = Collision::IsColliding(colliderA->box, colliderB->box, (objectArray[i]->angleDeg * M_PI)/180, (objectArray[j]->angleDeg * M_PI)/180);
                    if (collision) {
                        objectArray[i]->NotifyCollision(*objectArray[j]);
                        objectArray[j]->NotifyCollision(*objectArray[i]);
                    }
                }
            }
        }
    }
}

bool YZ_Sort(shared_ptr<GameObject> a, shared_ptr<GameObject> b) {
    if (a->box.Z == b->box.Z)
        return a->box.Y < b->box.Y;
    return a->box.Z < b->box.Z;
}
void State::RenderArray() {
    sort(objectArray.begin(), objectArray.end(), YZ_Sort);
    for (int i = 0; i < objectArray.size(); i++) {
        objectArray[i]->Render();
    }
}




