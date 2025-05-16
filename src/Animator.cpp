#include "../include/Animator.hpp"
#include "../include/SpriteRenderer.hpp"
#include <iostream>

Animator::Animator(GameObject& associated):Component(associated) {
    frameStart = 0;
    frameEnd = 0;
    frameTime = 0;
    currentFrame = 0;
    timeElapsed = 0;
    currentAnimation = "";
}

void Animator::Update(float dt) {
    if (frameTime != 0) {
        timeElapsed += dt;
    }
    if (timeElapsed > frameTime) {
        currentFrame++;
        timeElapsed -= frameTime;
        if (currentFrame > frameEnd) {
            currentFrame = frameStart;
        }
        Component* component = associated.GetComponent("SpriteRenderer");
        SpriteRenderer* spriteRenderer = dynamic_cast<SpriteRenderer*>(component);
        SDL_RendererFlip flip = animations[currentAnimation].flip;
        if (spriteRenderer) {
            spriteRenderer->SetFrame(currentFrame);
            spriteRenderer->SetFlip(flip);
        } else {
            cerr << "Erro - Animator Update - SpriteRenderer não encontrado!" << endl;
        }
    }
}

void Animator::Render() {}

bool Animator::Is(string type) {
    return type == "Animator";
}

void Animator::SetAnimation(string name) {
    auto search = animations.find(name);
    if (search != animations.end() && name != currentAnimation) {
        Animation anim = search->second;
        frameStart = anim.frameStart;
        frameEnd = anim.frameEnd;
        frameTime = anim.frameTime;
        currentFrame = frameStart;
        timeElapsed = 0;
        currentAnimation = name;

        Component* component = associated.GetComponent("SpriteRenderer");
        SpriteRenderer* spriteRenderer = dynamic_cast<SpriteRenderer*>(component);
        SDL_RendererFlip flip = animations[currentAnimation].flip;
        if (spriteRenderer) {
            spriteRenderer->SetFrame(currentFrame);
            spriteRenderer->SetFlip(flip);
        } else {
            cerr << "Erro - SetAnimation - SpriteRenderer não encontrado!" << endl;
        }
    }
}

void Animator::AddAnimation(string name, Animation anim) {
    auto search = animations.find(name);
    if (search == animations.end()) {
        animations[name] = anim;
    }
}

string Animator::GetAnimation() {
    return currentAnimation;
}
