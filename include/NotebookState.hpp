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

    void UpdatePageContent();
    void RenderCollectables(int level);

private:
    GameObject* cadernoObj;
    int currentPage;
    Text* currentPageText;
    vector<GameObject*> pageItems;
    bool contentNeedsUpdate = false;
};

#endif //NOTEBOOKSTATE_HPP
