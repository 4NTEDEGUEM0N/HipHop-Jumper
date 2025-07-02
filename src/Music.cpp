#include "../include/Music.hpp"
#include <iostream>

#include "../include/Resources.hpp"

using namespace std;

int Music::MUSIC_VOLUME = 3;

Music::Music() {
    music = nullptr;
}

Music::Music(string file) {
    music = nullptr;
    Open(file);
}

void Music::Play(int times) {
    if (music != nullptr) {
        Mix_VolumeMusic(MUSIC_VOLUME*10);
        bool mix_playmusic = Mix_PlayMusic(music.get(), times);
        if (mix_playmusic == -1) {
            cerr << "Erro - Mix_PlayMusic: " << SDL_GetError() << endl;
            exit(1);
        }
    }
}

void Music::Stop(int msToStop) {
    bool mix_fadeoutmusic = Mix_FadeOutMusic(msToStop);
    if (mix_fadeoutmusic == 0) {
        cerr << "Erro - Mix_FadeOutMusic: " << SDL_GetError() << endl;
        exit(1);
    }
}

void Music::Pause() {
    if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
        Mix_PauseMusic();
    }
}

void Music::Resume() {
    if (Mix_PlayingMusic() && Mix_PausedMusic()) {
        Mix_ResumeMusic();
    }
}

void Music::UpdateVolume() {
    Mix_VolumeMusic(MUSIC_VOLUME);
}


void Music::Open(string file) {
    if (music != nullptr) {
        music = nullptr;
    }

    music = Resources::GetMusic(file);
    if (music == nullptr) {
        cerr << "Erro - Mix_LoadMUS: " << SDL_GetError() << endl;
        exit(1);
    }
}

bool Music::IsOpen() {
    return music != nullptr;
}

Music::~Music() {
    if (music != nullptr) {
        Stop();
        music = nullptr;
    }
}