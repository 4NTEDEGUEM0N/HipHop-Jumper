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
    Rect* characterRect;

public:
    HUD();
    void Render();
};

#endif //HUD_HPP
