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

    Sprite characterSprite;
    Sprite characterAbilities;
    Sprite characterItems;
    Rect* characterRect;

    Timer* levelTimer;
    Text* levelTimerText;

    Text* fpsText;

public:
    HUD();
    void Render();
    void Update(float dt);
    void RenderAbility(int abilityNumber, bool active, int x, int y);
    float GetLevelTimer();
};

#endif //HUD_HPP
