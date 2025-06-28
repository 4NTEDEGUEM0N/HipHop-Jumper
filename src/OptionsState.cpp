#include "../include/OptionsState.hpp"
#include "../include/SpriteRenderer.hpp"
#include "../include/Game.hpp"
#include "../include/InputManager.hpp"
#include "../include/Sound.hpp"
#include "../include/Music.hpp"
#include "../include/Button.hpp"

OptionsState::OptionsState() {
    GameObject* bgObject = new GameObject();
    AddObject(bgObject);
    SpriteRenderer* bg = new SpriteRenderer(*bgObject,  "../Recursos/img/caderno.png");
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
    keyObj->box.X = Game::VirtualScreenWidth/2 + keyObj->box.W/2;
    keyObj->box.Y = 20;
}

void OptionsState::Update(float dt) {
    UpdateArray(dt);

    if (InputManager::GetInstance().KeyPress(SDLK_ESCAPE)) {
        popRequested = true;
    }
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

