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

public:
    HUD();
    void Render();
    void Update(float dt);
    void RenderAbility(int abilityNumber, bool active, int x, int y);
    void RenderItem(int itemNumber, int x, int y);
};

#endif //HUD_HPP
