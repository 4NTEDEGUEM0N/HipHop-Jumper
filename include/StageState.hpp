#ifndef STAGESTATE_HPP
#define STAGESTATE_HPP

#include "HUD.hpp"
#include "Music.hpp"
#include "State.hpp"
#include "TileSet.hpp"

using namespace std;

class StageState : public State {
private:
    TileSet* tileSet;
    Music backgroundMusic;
    HUD* hud;

    Timer* endTimer;
    void End(GameObject* playerObject);

public:
    StageState();
    ~StageState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();


    GameObject* tileMapObject;
    GameObject* GetTileMapObject();
};


#endif //STAGESTATE_HPP
