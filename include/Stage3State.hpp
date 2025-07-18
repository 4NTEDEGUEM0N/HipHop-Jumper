#ifndef STAGES3TATE_HPP
#define STAGES3TATE_HPP

#include "HUD.hpp"
#include "Music.hpp"
#include "State.hpp"
#include "TileSet.hpp"

using namespace std;

class Stage3State : public State {
private:
    TileSet* tileSet;
    Music backgroundMusic;
    HUD* hud;

    Timer* endTimer;
    void End(GameObject* playerObject);

public:
    Stage3State();
    ~Stage3State();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();


    GameObject* tileMapObject;
    GameObject* GetTileMapObject();
};


#endif //STAGE3STATE_HPP
