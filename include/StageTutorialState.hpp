#ifndef STAGETUTORIALSTATE_HPP
#define STAGETUTORIALSTATE_HPP

#include "HUD.hpp"
#include "Music.hpp"
#include "State.hpp"
#include "TileSet.hpp"

using namespace std;

class StageTutorialState : public State {
private:
    TileSet* tileSet;
    Music backgroundMusic;
    HUD* hud;

    Timer* endTimer;
    void End(GameObject* playerObject);

public:
    StageTutorialState();
    ~StageTutorialState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();


    GameObject* tileMapObject;
    GameObject* GetTileMapObject();
};


#endif //STAGETUTORIALSTATE_HPP
