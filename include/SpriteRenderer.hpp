#ifndef SPRITERENDERER_HPP
#define SPRITERENDERER_HPP

#include "Component.hpp"
#include "Sprite.hpp"

class SpriteRenderer: public Component {
private:
    Sprite sprite;
public:
    SpriteRenderer(GameObject& associated);
    SpriteRenderer(GameObject& associated, string file, int frame_count_w = 1, int frame_count_h = 1);
    void Open(string file);
    void SetFrameCount(int frame_count_w, int frame_count_h);
    void Update(float dt);
    void Render();
    bool Is(string type);
    void SetFrame(int frame);
    void SetCameraFollower(bool cameraFollower);
    void SetScale(float scaleX, float scaleY);
    void SetFlip(SDL_RendererFlip flip);
};

#endif //SPRITERENDERER_HPP
