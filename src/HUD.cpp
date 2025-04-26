#include "../include/HUD.hpp"
#include "../include/Game.hpp"
#include <iostream>

HUD::HUD() : characterSprite("../Recursos/img/bonequinho.png") {
    characterSprite.cameraFollower = true;
    characterSprite.SetScale(0.6f, 0.6f);
    characterRect = new Rect(0, 900-characterSprite.GetHeight(), characterSprite.GetRealWidth(), characterSprite.GetRealHeight());

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
}

