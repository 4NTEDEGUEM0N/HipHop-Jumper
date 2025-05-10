#ifndef CADERNOSTATE_HPP
#define CADERNOSTATE_HPP
#include <SDL_render.h>
#include "State.hpp"

class CadernoState : public State {
public:
    CadernoState();
    ~CadernoState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();

    void CreateColorButton(string cor, SDL_Color color, int n);
    void RenderMouseBrush();
    void ClearCanvas();

private:
    GameObject* cadernoObj;
    SDL_Texture* canvasTexture;
    int width, height;
    int lastX, lastY;
    bool drawing;
    SDL_Color currentColor;
    int brushSize;
};


#endif //CADERNOSTATE_HPP
