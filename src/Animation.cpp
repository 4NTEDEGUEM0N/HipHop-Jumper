#include "../include/Animation.hpp"

Animation::Animation(int frame_start, int frame_end, float frame_time, SDL_RendererFlip flip) {
    frameStart = frame_start;
    frameEnd = frame_end;
    frameTime = frame_time;
    this->flip = flip;
}

void Animation::SetCallback(int frame, std::function<void()> callback) {
    frameCallbacks[frame] = callback;
}

void Animation::CheckAndRunCallback(int currentFrame) {
    auto it = frameCallbacks.find(currentFrame);
    if (it != frameCallbacks.end() && it->second) {
        it->second();
    }
}
