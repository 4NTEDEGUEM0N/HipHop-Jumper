#ifndef HUD_HPP
#define HUD_HPP
#include "Character.hpp"
#include "Rect.hpp"
#include "Sprite.hpp"
#include "Text.hpp"


class HUD {
private:
    Character* player;
    Rect* hpRect;
    Text* hpText;
    Rect* dashRect;

    Sprite characterSprite;
    Sprite dashSprite;
    Rect* characterRect;

    Timer* levelTimer;
    Text* levelTimerText;

    Text* fpsText;

public:
    HUD();
    void Render();
    void Update(float dt);
    float GetLevelTimer();
};

#endif //HUD_HPP
