#ifndef STAGES2TATE_HPP
#define STAGES2TATE_HPP

#include "HUD.hpp"
#include "Music.hpp"
#include "State.hpp"
#include "TileSet.hpp"

using namespace std;

class Stage2State : public State {
private:
    TileSet* tileSet;
    Music backgroundMusic;
    HUD* hud;

    Timer* endTimer;
    void End(GameObject* playerObject);

public:
    Stage2State();
    ~Stage2State();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();


    GameObject* tileMapObject;
    GameObject* GetTileMapObject();
};


#endif //STAGE2STATE_HPP
