#include "../include/OptionsState.hpp"

#include <iostream>
#include <ostream>

#include "../include/SpriteRenderer.hpp"
#include "../include/Game.hpp"
#include "../include/InputManager.hpp"
#include "../include/Sound.hpp"
#include "../include/Music.hpp"
#include "../include/Button.hpp"

KeyBindingManager::GameAction OptionsState::actionToRebind = KeyBindingManager::GameAction::JUMP;

OptionsState::OptionsState() {
    GameObject* bgObject = new GameObject();
    AddObject(bgObject);
    SpriteRenderer* bg = new SpriteRenderer(*bgObject,  "../Recursos/img/CADERNO BRABO.png");
    bg->SetCameraFollower(true);
    bgObject->AddComponent(bg);
    float scaleX = Game::VirtualScreenWidth  / (bgObject->box.W - 70);
    float scaleY = Game::VirtualScreenHeight / (bgObject->box.H - 30);
    bg->SetScale(scaleX, scaleY);
    bgObject->box.X = -70;
    bgObject->box.Y = -30;

    GameObject* audioObj = new GameObject();
    AddObject(audioObj);
    Text* audioText = new Text(*audioObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Audio", {0, 0, 0, 255}, true);
    audioObj->AddComponent(audioText);
    audioObj->box.X = Game::VirtualScreenWidth/4 - audioObj->box.W/2;
    audioObj->box.Y = 20;

    GameObject* soundObj = new GameObject();
    AddObject(soundObj);
    soundText = new Text(*soundObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 70, Text::BLENDED, "Sound: " + to_string(Sound::SOUND_VOLUME) + " ", {64, 64, 64, 255}, true);
    soundObj->AddComponent(soundText);
    soundObj->box.X = Game::VirtualScreenWidth/4 - soundObj->box.W/2;
    soundObj->box.Y = audioObj->box.Y + 130;

    GameObject* soundPlusObj = new GameObject();
    Button* soundPlusButton = new Button(*soundPlusObj);
    soundPlusObj->AddComponent(soundPlusButton);
    SpriteRenderer* soundPlusbuttonSprite = new SpriteRenderer(*soundPlusObj, "../Recursos/img/pixel.png");
    soundPlusbuttonSprite->SetCameraFollower(true);
    soundPlusObj->AddComponent(soundPlusbuttonSprite);
    Text* soundTextPlus = new Text(*soundPlusObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 70, Text::BLENDED, " + ", {255, 255, 255, 255}, true);
    soundPlusObj->AddComponent(soundTextPlus);
    soundPlusObj->box.X = soundObj->box.X + soundObj->box.W + 10;
    soundPlusObj->box.Y = soundObj->box.Y;
    AddObject(soundPlusObj);

    soundPlusButton->SetClickFunction([this]() {
        if (Sound::SOUND_VOLUME < 10) {
            Sound::SOUND_VOLUME++;
            if (Sound::SOUND_VOLUME > 9)
                soundText->SetText("Sound: " + to_string(Sound::SOUND_VOLUME));
            else
                soundText->SetText("Sound: " + to_string(Sound::SOUND_VOLUME) + " ");
        }
    });

    GameObject* soundMinusObj = new GameObject();
    Button* soundMinusButton = new Button(*soundMinusObj);
    soundMinusObj->AddComponent(soundMinusButton);
    SpriteRenderer* soundMinusButtonSprite = new SpriteRenderer(*soundMinusObj, "../Recursos/img/pixel.png");
    soundMinusButtonSprite->SetCameraFollower(true);
    soundMinusObj->AddComponent(soundMinusButtonSprite);
    Text* soundTextMinus = new Text(*soundMinusObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 70, Text::SOLID, " - ", {255, 255, 255, 255}, true);
    soundMinusObj->AddComponent(soundTextMinus);
    soundMinusObj->box.X = soundPlusObj->box.X + soundPlusObj->box.W + 5;
    soundMinusObj->box.Y = soundPlusObj->box.Y;
    AddObject(soundMinusObj);

    soundMinusButton->SetClickFunction([this]() {
        if (Sound::SOUND_VOLUME > 1) {
            Sound::SOUND_VOLUME--;
            soundText->SetText("Sound: " + to_string(Sound::SOUND_VOLUME) + " ");
        }
    });

    GameObject* MusicObj = new GameObject();
    AddObject(MusicObj);
    musicText = new Text(*MusicObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 70, Text::BLENDED, "Music: " + to_string(Music::MUSIC_VOLUME) + " ", {64, 64, 64, 255}, true);
    MusicObj->AddComponent(musicText);
    MusicObj->box.X = Game::VirtualScreenWidth/4 - MusicObj->box.W/2;
    MusicObj->box.Y = soundObj->box.Y + 130;

    GameObject* musicPlusObj = new GameObject();
    Button* musicPlusButton = new Button(*musicPlusObj);
    musicPlusObj->AddComponent(musicPlusButton);
    SpriteRenderer* musicPlusbuttonSprite = new SpriteRenderer(*musicPlusObj, "../Recursos/img/pixel.png");
    musicPlusbuttonSprite->SetCameraFollower(true);
    musicPlusObj->AddComponent(musicPlusbuttonSprite);
    Text* musicTextPlus = new Text(*musicPlusObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 70, Text::BLENDED, " + ", {255, 255, 255, 255}, true);
    musicPlusObj->AddComponent(musicTextPlus);
    musicPlusObj->box.X = MusicObj->box.X + MusicObj->box.W + 10;
    musicPlusObj->box.Y = MusicObj->box.Y;
    AddObject(musicPlusObj);

    musicPlusButton->SetClickFunction([this]() {
        if (Music::MUSIC_VOLUME < 10) {
            Music::MUSIC_VOLUME++;
            if (Music::MUSIC_VOLUME > 9)
                musicText->SetText("Music: " + to_string(Music::MUSIC_VOLUME));
            else
                musicText->SetText("Music: " + to_string(Music::MUSIC_VOLUME) + " ");
        }
    });

    GameObject* musicMinusObj = new GameObject();
    Button* musicMinusButton = new Button(*musicMinusObj);
    musicMinusObj->AddComponent(musicMinusButton);
    SpriteRenderer* musicMinusButtonSprite = new SpriteRenderer(*musicMinusObj, "../Recursos/img/pixel.png");
    musicMinusButtonSprite->SetCameraFollower(true);
    musicMinusObj->AddComponent(musicMinusButtonSprite);
    Text* musictextMinus = new Text(*musicMinusObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 70, Text::SOLID, " - ", {255, 255, 255, 255}, true);
    musicMinusObj->AddComponent(musictextMinus);
    musicMinusObj->box.X = musicPlusObj->box.X + musicPlusObj->box.W + 5;
    musicMinusObj->box.Y = musicPlusObj->box.Y;
    AddObject(musicMinusObj);

    musicMinusButton->SetClickFunction([this]() {
        if (Music::MUSIC_VOLUME > 1) {
            Music::MUSIC_VOLUME--;
            musicText->SetText("Music: " + to_string(Music::MUSIC_VOLUME) + " ");
        }
    });

    GameObject* keyObj = new GameObject();
    AddObject(keyObj);
    Text* keyText = new Text(*keyObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 100, Text::BLENDED, "Controls", {0, 0, 0, 255}, true);
    keyObj->AddComponent(keyText);
    keyObj->box.X = (3 * Game::VirtualScreenWidth)/4 - keyObj->box.W/2;
    keyObj->box.Y = 20;

    KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
    GameObject* jumpObj = new GameObject();
    Button* jumpButton = new Button(*jumpObj);
    jumpObj->AddComponent(jumpButton);
    AddObject(jumpObj);
    jumpText = new Text(*jumpObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 50, Text::BLENDED, "Jump: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::JUMP))), {64, 64, 64, 255}, true);
    jumpObj->AddComponent(jumpText);
    jumpObj->box.X = (3 * Game::VirtualScreenWidth)/4 - jumpObj->box.W/2;
    jumpObj->box.Y = keyObj->box.Y + 130;

    jumpButton->SetClickFunction([this]() {
        actionToRebind = KeyBindingManager::GameAction::JUMP;
        KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
        GameObject* pressObj = new GameObject();
        Text* pressText = new Text(*pressObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 200, Text::SHADED, "PRESS ANY KEY", {254, 88, 224, 255}, true);
        pressObj->AddComponent(pressText);
        pressObj->box.X = Game::VirtualScreenWidth/2 - pressObj->box.W/2;
        pressObj->box.Y = Game::VirtualScreenHeight/2 - pressObj->box.H/2;
        pressText->Render();
        SDL_RenderPresent(Game::GetInstance().GetRenderer());
        SDL_Keycode newKey = keybinder.GetNextKeyPressed();
        keybinder.SetKeyForAction(actionToRebind, newKey);
        jumpText->SetText("Jump: " + string(SDL_GetKeyName(newKey)));
        delete pressObj;
    });
    jumpButton->SetHoverFunction([this]() {
        jumpText->SetColor({26, 160, 251, 255});
    });
    jumpButton->SetNoneFunction([this]() {
        jumpText->SetColor({64, 64, 64, 255});
    });

    GameObject* leftObj = new GameObject();
    Button* leftButton = new Button(*leftObj);
    leftObj->AddComponent(leftButton);
    AddObject(leftObj);
    leftText = new Text(*leftObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 50, Text::BLENDED, "Left: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::MOVE_LEFT))), {64, 64, 64, 255}, true);
    leftObj->AddComponent(leftText);
    leftObj->box.X = (3 * Game::VirtualScreenWidth)/4 - leftObj->box.W/2;
    leftObj->box.Y = jumpObj->box.Y + leftObj->box.H + 10;

    leftButton->SetClickFunction([this]() {
        actionToRebind = KeyBindingManager::GameAction::MOVE_LEFT;
        KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
        GameObject* pressObj = new GameObject();
        Text* pressText = new Text(*pressObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 200, Text::SHADED, "PRESS ANY KEY", {254, 88, 224, 255}, true);
        pressObj->AddComponent(pressText);
        pressObj->box.X = Game::VirtualScreenWidth/2 - pressObj->box.W/2;
        pressObj->box.Y = Game::VirtualScreenHeight/2 - pressObj->box.H/2;
        pressText->Render();
        SDL_RenderPresent(Game::GetInstance().GetRenderer());
        SDL_Keycode newKey = keybinder.GetNextKeyPressed();
        keybinder.SetKeyForAction(actionToRebind, newKey);
        leftText->SetText("Left: " + string(SDL_GetKeyName(newKey)));
        delete pressObj;
    });
    leftButton->SetHoverFunction([this]() {
        leftText->SetColor({26, 160, 251, 255});
    });
    leftButton->SetNoneFunction([this]() {
        leftText->SetColor({64, 64, 64, 255});
    });

    GameObject* rightObj = new GameObject();
    Button* rightButton = new Button(*rightObj);
    rightObj->AddComponent(rightButton);
    AddObject(rightObj);
    rightText = new Text(*rightObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 50, Text::BLENDED, "Right: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::MOVE_RIGHT))), {64, 64, 64, 255}, true);
    rightObj->AddComponent(rightText);
    rightObj->box.X = (3 * Game::VirtualScreenWidth)/4 - rightObj->box.W/2;
    rightObj->box.Y = leftObj->box.Y + rightObj->box.H + 10;

    rightButton->SetClickFunction([this]() {
        actionToRebind = KeyBindingManager::GameAction::MOVE_RIGHT;
        KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
        GameObject* pressObj = new GameObject();
        Text* pressText = new Text(*pressObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 200, Text::SHADED, "PRESS ANY KEY", {254, 88, 224, 255}, true);
        pressObj->AddComponent(pressText);
        pressObj->box.X = Game::VirtualScreenWidth/2 - pressObj->box.W/2;
        pressObj->box.Y = Game::VirtualScreenHeight/2 - pressObj->box.H/2;
        pressText->Render();
        SDL_RenderPresent(Game::GetInstance().GetRenderer());
        SDL_Keycode newKey = keybinder.GetNextKeyPressed();
        keybinder.SetKeyForAction(actionToRebind, newKey);
        rightText->SetText("Right: " + string(SDL_GetKeyName(newKey)));
        delete pressObj;
    });
    rightButton->SetHoverFunction([this]() {
        rightText->SetColor({26, 160, 251, 255});
    });
    rightButton->SetNoneFunction([this]() {
        rightText->SetColor({64, 64, 64, 255});
    });

    GameObject* dashObj = new GameObject();
    Button* dashButton = new Button(*dashObj);
    dashObj->AddComponent(dashButton);
    AddObject(dashObj);
    dashText = new Text(*dashObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 50, Text::BLENDED, "Dash: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::DASH))), {64, 64, 64, 255}, true);
    dashObj->AddComponent(dashText);
    dashObj->box.X = (3 * Game::VirtualScreenWidth)/4 - dashObj->box.W/2;
    dashObj->box.Y = rightObj->box.Y + dashObj->box.H + 10;

    dashButton->SetClickFunction([this]() {
        actionToRebind = KeyBindingManager::GameAction::DASH;
        KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
        GameObject* pressObj = new GameObject();
        Text* pressText = new Text(*pressObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 200, Text::SHADED, "PRESS ANY KEY", {254, 88, 224, 255}, true);
        pressObj->AddComponent(pressText);
        pressObj->box.X = Game::VirtualScreenWidth/2 - pressObj->box.W/2;
        pressObj->box.Y = Game::VirtualScreenHeight/2 - pressObj->box.H/2;
        pressText->Render();
        SDL_RenderPresent(Game::GetInstance().GetRenderer());
        SDL_Keycode newKey = keybinder.GetNextKeyPressed();
        keybinder.SetKeyForAction(actionToRebind, newKey);
        dashText->SetText("Dash: " + string(SDL_GetKeyName(newKey)));
        delete pressObj;
    });
    dashButton->SetHoverFunction([this]() {
        dashText->SetColor({26, 160, 251, 255});
    });
    dashButton->SetNoneFunction([this]() {
        dashText->SetColor({64, 64, 64, 255});
    });

    GameObject* notebookObj = new GameObject();
    Button* notebookButton = new Button(*notebookObj);
    notebookObj->AddComponent(notebookButton);
    AddObject(notebookObj);
    notebookText = new Text(*notebookObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 50, Text::BLENDED, "Notebook: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::NOTEBOOK))), {64, 64, 64, 255}, true);
    notebookObj->AddComponent(notebookText);
    notebookObj->box.X = (3 * Game::VirtualScreenWidth)/4 - notebookObj->box.W/2;
    notebookObj->box.Y = dashObj->box.Y + notebookObj->box.H + 10;

    notebookButton->SetClickFunction([this]() {
        actionToRebind = KeyBindingManager::GameAction::NOTEBOOK;
        KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
        GameObject* pressObj = new GameObject();
        Text* pressText = new Text(*pressObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 200, Text::SHADED, "PRESS ANY KEY", {254, 88, 224, 255}, true);
        pressObj->AddComponent(pressText);
        pressObj->box.X = Game::VirtualScreenWidth/2 - pressObj->box.W/2;
        pressObj->box.Y = Game::VirtualScreenHeight/2 - pressObj->box.H/2;
        pressText->Render();
        SDL_RenderPresent(Game::GetInstance().GetRenderer());
        SDL_Keycode newKey = keybinder.GetNextKeyPressed();
        keybinder.SetKeyForAction(actionToRebind, newKey);
        notebookText->SetText("Notebook: " + string(SDL_GetKeyName(newKey)));
        delete pressObj;
    });
    notebookButton->SetHoverFunction([this]() {
        notebookText->SetColor({26, 160, 251, 255});
    });
    notebookButton->SetNoneFunction([this]() {
        notebookText->SetColor({64, 64, 64, 255});
    });

    GameObject* graffitiObj = new GameObject();
    Button* graffitiButton = new Button(*graffitiObj);
    graffitiObj->AddComponent(graffitiButton);
    AddObject(graffitiObj);
    graffitiText = new Text(*graffitiObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 50, Text::BLENDED, "Graffiti: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::GRAFFITI))), {64, 64, 64, 255}, true);
    graffitiObj->AddComponent(graffitiText);
    graffitiObj->box.X = (3 * Game::VirtualScreenWidth)/4 - graffitiObj->box.W/2;
    graffitiObj->box.Y = notebookObj->box.Y + graffitiObj->box.H + 10;

    graffitiButton->SetClickFunction([this]() {
        actionToRebind = KeyBindingManager::GameAction::GRAFFITI;
        KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
        GameObject* pressObj = new GameObject();
        Text* pressText = new Text(*pressObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 200, Text::SHADED, "PRESS ANY KEY", {254, 88, 224, 255}, true);
        pressObj->AddComponent(pressText);
        pressObj->box.X = Game::VirtualScreenWidth/2 - pressObj->box.W/2;
        pressObj->box.Y = Game::VirtualScreenHeight/2 - pressObj->box.H/2;
        pressText->Render();
        SDL_RenderPresent(Game::GetInstance().GetRenderer());
        SDL_Keycode newKey = keybinder.GetNextKeyPressed();
        keybinder.SetKeyForAction(actionToRebind, newKey);
        graffitiText->SetText("Graffiti: " + string(SDL_GetKeyName(newKey)));
        delete pressObj;
    });
    graffitiButton->SetHoverFunction([this]() {
        graffitiText->SetColor({26, 160, 251, 255});
    });
    graffitiButton->SetNoneFunction([this]() {
        graffitiText->SetColor({64, 64, 64, 255});
    });
    
    GameObject* stungleeObj = new GameObject();
    Button* stungleeButton = new Button(*stungleeObj);
    stungleeObj->AddComponent(stungleeButton);
    AddObject(stungleeObj);
    stungleeText = new Text(*stungleeObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 50, Text::BLENDED, "Stun: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::STUNGLEE))), {64, 64, 64, 255}, true);
    stungleeObj->AddComponent(stungleeText);
    stungleeObj->box.X = (3 * Game::VirtualScreenWidth)/4 - stungleeObj->box.W/2;
    stungleeObj->box.Y = graffitiObj->box.Y + stungleeObj->box.H + 10;
    
    stungleeButton->SetClickFunction([this]() {
        actionToRebind = KeyBindingManager::GameAction::STUNGLEE;
        KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
        GameObject* pressObj = new GameObject();
        Text* pressText = new Text(*pressObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 200, Text::SHADED, "PRESS ANY KEY", {254, 88, 224, 255}, true);
        pressObj->AddComponent(pressText);
        pressObj->box.X = Game::VirtualScreenWidth/2 - pressObj->box.W/2;
        pressObj->box.Y = Game::VirtualScreenHeight/2 - pressObj->box.H/2;
        pressText->Render();
        SDL_RenderPresent(Game::GetInstance().GetRenderer());
        SDL_Keycode newKey = keybinder.GetNextKeyPressed();
        keybinder.SetKeyForAction(actionToRebind, newKey);
        stungleeText->SetText("Stun: " + string(SDL_GetKeyName(newKey)));
        delete pressObj;
    });
    stungleeButton->SetHoverFunction([this]() {
        stungleeText->SetColor({26, 160, 251, 255});
    });
    stungleeButton->SetNoneFunction([this]() {
        stungleeText->SetColor({64, 64, 64, 255});
    });

    GameObject* restoreObj = new GameObject();
    Button* restoreButton = new Button(*restoreObj);
    restoreObj->AddComponent(restoreButton);
    AddObject(restoreObj);
    restoreText = new Text(*restoreObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 50, Text::BLENDED, "Reset to Default", {128,0,0,255}, true);
    restoreObj->AddComponent(restoreText);
    restoreObj->box.X = (3 * Game::VirtualScreenWidth)/4 - restoreObj->box.W/2;
    restoreObj->box.Y = stungleeObj->box.Y + restoreObj->box.H + 30;

    restoreButton->SetClickFunction([this]() {
        KeyBindingManager& keybinder = KeyBindingManager::GetInstance();
        keybinder.LoadDefaultBindings();
        jumpText->SetText("Jump: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::JUMP))));
        leftText->SetText("Left: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::MOVE_LEFT))));
        rightText->SetText("Right: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::MOVE_RIGHT))));
        dashText->SetText("Dash: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::DASH))));
        notebookText->SetText("Notebook: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::NOTEBOOK))));
        graffitiText->SetText("Graffiti: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::GRAFFITI))));
        stungleeText->SetText("Stun: " + string(SDL_GetKeyName(keybinder.GetKeyForAction(KeyBindingManager::GameAction::STUNGLEE))));
    });
    restoreButton->SetHoverFunction([this]() {
        restoreText->SetColor({26, 160, 251, 255});
    });
    restoreButton->SetNoneFunction([this]() {
        restoreText->SetColor({128,0,0,255});
    });

    GameObject* saveObj = new GameObject();
    Button* saveButton = new Button(*saveObj);
    saveObj->AddComponent(saveButton);
    AddObject(saveObj);
    saveText = new Text(*saveObj, "../Recursos/font/GraffitiYouth-Regular.ttf", 50, Text::BLENDED, "Save", {0,128,0,255}, true);
    saveObj->AddComponent(saveText);
    saveObj->box.X = Game::VirtualScreenWidth/4 - saveObj->box.W/2;
    saveObj->box.Y = restoreObj->box.Y;

    saveButton->SetClickFunction([this]() {
        popRequested = true;
    });
    saveButton->SetHoverFunction([this]() {
        saveText->SetColor({26, 160, 251, 255});
    });
    saveButton->SetNoneFunction([this]() {
        saveText->SetColor({0,128,0,255});
    });
}

void OptionsState::Update(float dt) {
    UpdateArray(dt);
    InputManager& input = InputManager::GetInstance();

    if (input.KeyPress(SDLK_ESCAPE)) {
        popRequested = true;
    }
    quitRequested = input.QuitRequested();
}

void OptionsState::Render() {
    RenderArray();
}

void OptionsState::Start() {
    StartArray();
}
void OptionsState::Pause() {}
void OptionsState::Resume() {}
void OptionsState::LoadAssets() {}
OptionsState::~OptionsState() = default;

