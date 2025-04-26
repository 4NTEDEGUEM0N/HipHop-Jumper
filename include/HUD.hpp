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
    Rect* characterRect;

public:
    HUD();
    void Render();
    void RenderAbility(int abilityNumber, bool active, int x, int y);
};

#endif //HUD_HPP
