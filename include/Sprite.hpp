#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <memory>
#include <string>
#include <SDL.h>

#include "Vec2.hpp"

using namespace std;

class Sprite {
private:
    shared_ptr<SDL_Texture> texture;
    int width;
    int height;
    SDL_Rect clipRect;
    int frameCountW;
    int frameCountH;
    Vec2 scale;
    SDL_RendererFlip flip;

public:
    bool cameraFollower;

    Sprite();
    Sprite(string file, int frame_count_w = 1, int frame_count_h = 1);
    ~Sprite();
    void Open(string file);
    void SetClip(int x, int y, int w, int h);
    void Render(int x, int y, int w, int h, float angle = 0);
    int GetWidth();
    int GetHeight();
    int GetRealWidth();
    int GetRealHeight();
    bool IsOpen();
    void SetFrame(int frame);
    void SetFrameCount(int frame_count_w, int frame_count_h);
    void SetScale(float scaleX, float scaleY);
    Vec2 GetScale();
    void SetFlip(SDL_RendererFlip flip);
};


#endif //SPRITE_HPP
