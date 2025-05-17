#include "../include/HUD.hpp"
#include "../include/Game.hpp"
#include <iostream>

HUD::HUD() : characterSprite("../Recursos/img/bonequinho.png"), characterAbilities("../Recursos/img/habilidades.png",2,3) {
    characterSprite.cameraFollower = true;
    characterAbilities.cameraFollower = true;
    characterSprite.SetScale(0.6f, 0.6f);
    characterRect = new Rect(0, 900-characterSprite.GetHeight(), characterSprite.GetWidth(), characterSprite.GetHeight());

    player = Character::player;
    hpRect = new Rect(0, 900 - characterSprite.GetHeight() - 20, characterSprite.GetWidth(), 20);
    GameObject* hpTextObj = new GameObject(true);
    hpTextObj->box.X = hpRect->X + hpRect->W/2 - 25;
    hpTextObj->box.Y = hpRect->Y + hpRect->H/2 - 8;
    hpText = new Text(*hpTextObj, "../Recursos/font/neodgm.ttf", 20, Text::SOLID, "", {255, 255, 255, 255}, true);
}

void HUD::Render() {
    characterSprite.Render(characterRect->X, characterRect->Y, characterRect->W, characterRect->H);

    float hpPercent = (float)player->GetHP() / 100;
    int currentBarWidth = (int)(hpRect->W * hpPercent);

    SDL_Renderer* renderer = Game::GetInstance().GetRenderer();

    SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
    SDL_Rect bgRect = { (int)this->hpRect->X, (int)this->hpRect->Y, (int)this->hpRect->W, (int)this->hpRect->H };
    SDL_RenderFillRect(renderer, &bgRect);


    SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
    SDL_Rect hpRect = { (int)this->hpRect->X, (int)this->hpRect->Y, currentBarWidth, (int)this->hpRect->H };
    SDL_RenderFillRect(renderer, &hpRect);

    hpText->SetText(to_string(player->GetHP()) + "/100");
    hpText->Render();

    RenderAbility(0, player->CanJump(), characterRect->X + characterSprite.GetWidth() - 37, characterRect->Y + 0*32 + 1*5);
    RenderAbility(1, player->CanDoubleJump(), characterRect->X + characterSprite.GetWidth() - 37, characterRect->Y + 1*32 + 2*5);
    RenderAbility(2, player->CanDash(), characterRect->X + characterSprite.GetWidth() - 37, characterRect->Y + 2*32 + 3*5);

}

void HUD::RenderAbility(int abilityNumber, bool active, int x, int y) {
    int frame = abilityNumber * 2 + (active ? 0 : 1);
    characterAbilities.SetFrame(frame);
    characterAbilities.Render(x, y, characterAbilities.GetWidth(), characterAbilities.GetHeight());
}

//void HUD::RenderItem(int itemNumber,)
