#include "../include/HUD.hpp"
#include "../include/Game.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>

#include "../include/GameData.hpp"

HUD::HUD() : characterSprite("../Recursos/img/hud_avatar.png",1,4), dashSprite("../Recursos/img/hud_dash.png") {
    characterSprite.cameraFollower = true;
    characterSprite.SetFrame(0);
    dashSprite.cameraFollower = true;
    characterRect = new Rect(0, Game::VirtualScreenHeight-characterSprite.GetHeight(), characterSprite.GetWidth(), characterSprite.GetHeight());

    player = Character::player;
    hpRect = new Rect(177, Game::VirtualScreenHeight - 46, 303, 25);
    GameObject* hpTextObj = new GameObject(true);
    hpTextObj->box.X = hpRect->X + 15;
    hpTextObj->box.Y = hpRect->Y + hpRect->H/2 - 8;
    hpText = new Text(*hpTextObj, "../Recursos/font/neodgm.ttf", 20, Text::SOLID, "", {255, 255, 255, 255}, true);

    dashRect = new Rect(276,Game::VirtualScreenHeight - 22 - 10,196,10);

    GameObject* levelTimerObj = new GameObject(true);
    levelTimer = new Timer();
    levelTimerText = new Text(*levelTimerObj, "../Recursos/font/neodgm.ttf", 30, Text::SOLID, "0.0", {255, 255, 255, 255}, true);
    levelTimerObj->box.X = (Game::VirtualScreenWidth / 2) - (levelTimerObj->box.W/2);
    levelTimerObj->box.Y = 10;

    GameObject* fpsObj = new GameObject(true);
    fpsText = new Text(*fpsObj, "../Recursos/font/neodgm.ttf", 20, Text::SOLID, "FPS: 0.0", {255, 255, 255, 255}, true);
    fpsObj->box.X = 10;
    fpsObj->box.Y = 10;
}

void HUD::Render() {
    if (Character::player != nullptr) {
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
        characterSprite.Render(characterRect->X, characterRect->Y, characterRect->W, characterRect->H);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect dashRect = { (int)this->dashRect->X, (int)this->dashRect->Y, (int)this->dashRect->W, (int)this->dashRect->H };
        SDL_RenderFillRect(renderer, &dashRect);
    }

    if (player != nullptr)
        characterSprite.SetFrame(player->ammo);
    levelTimerText->Render();
    fpsText->Render();
}


void HUD::Update(float dt) {
    if (GameData::ended == false)
        levelTimer->Update(dt);

    ostringstream timer;
    timer << fixed << setprecision(2) << levelTimer->Get();
    levelTimerText->SetText(timer.str());

    ostringstream fps;
    float delta = Game::GetInstance().GetDeltaTime();
    fps << fixed << setprecision(2) << 1/delta;
    fpsText->SetText("FPS: " + fps.str());

    float dashTimer = player->dashTimer.Get();
    if (dashTimer > 4) {
        dashTimer = 4;
    }
    float percentage = dashTimer/4;
    dashRect->X = 277 + percentage * 196;
    dashRect->W = 196 - percentage * 196;
}

float HUD::GetLevelTimer() {
    return levelTimer->Get();
}
