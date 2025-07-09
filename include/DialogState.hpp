#ifndef DIALOGSTATE_HPP
#define DIALOGSTATE_HPP

#include "Game.hpp"
#include "Text.hpp"

class DialogState : public State {
public:
    DialogState(vector<string> dialogs, string npcSprite, string npcName);
    ~DialogState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();


private:
    vector<string> dialogs;
    string npcSprite;
    string npcName;
    Text* dialogText;
    int currrentDialog;
};

#endif //DIALOGSTATE_HPP
