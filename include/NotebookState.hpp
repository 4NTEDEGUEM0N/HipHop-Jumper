#ifndef NOTEBOOKSTATE_HPP
#define NOTEBOOKSTATE_HPP

#include <SDL_render.h>
#include "Game.hpp"
#include "Text.hpp"

class NotebookState : public State {
public:
    NotebookState();
    ~NotebookState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();

    void RenderGraffitis();

private:
    GameObject* cadernoObj;
    int currentPage;
    Text* currentPageText;
};

#endif //NOTEBOOKSTATE_HPP
