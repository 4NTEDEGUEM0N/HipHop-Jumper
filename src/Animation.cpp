#include "../include/Animation.hpp"

Animation::Animation(int frame_start, int frame_end, float frame_time, SDL_RendererFlip flip) {
    frameStart = frame_start;
    frameEnd = frame_end;
    frameTime = frame_time;
    this->flip = flip;
}