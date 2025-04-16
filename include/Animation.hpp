#ifndef ANIMATION_HPP
#define ANIMATION_HPP
#include <SDL_render.h>

using namespace std;

class Animation {
public:
    int frameStart;
    int frameEnd;
    float frameTime;
    SDL_RendererFlip flip;

    Animation(int frame_start=0, int frame_end=0, float frame_time=0, SDL_RendererFlip flip = SDL_FLIP_NONE);
};

#endif //ANIMATION_HPP
