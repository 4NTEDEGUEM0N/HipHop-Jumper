#ifndef CADERNOSTATE_HPP
#define CADERNOSTATE_HPP
#include <SDL_render.h>
#include "State.hpp"
#include "Text.hpp"

class DrawState : public State {
public:
    DrawState();
    ~DrawState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();

    void CreateColorButton(string cor, SDL_Color color, int n);
    void RenderMouseBrush();
    void ClearCanvas();
    void FloodFill(int startX, int startY);

private:
    GameObject* cadernoObj;
    SDL_Texture* canvasTexture;
    int width, height;
    int lastX, lastY;
    bool drawing;
    SDL_Color currentColor;
    int brushSize;
    bool brush;
    Text* currentTool;
    Text* currentColorText;
    Text* currentSizeText;
};


#endif //CADERNOSTATE_HPP
