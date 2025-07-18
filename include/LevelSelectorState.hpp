#ifndef LEVELSELECTORSTATE_HPP
#define LEVELSELECTORSTATE_HPP

#include "State.hpp"
#include "Text.hpp"

class LevelSelectorState : public State {
public:
    LevelSelectorState();
    ~LevelSelectorState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();

    static void LoadLevel();
    static void NextLevel();
    static int currentLevel;
private:
    Text* tutorialText;
    Text* level1Text;
    Text* level2Text;
    Text* level3Text;

};

#endif //LEVELSELECTORSTATE_HPP
