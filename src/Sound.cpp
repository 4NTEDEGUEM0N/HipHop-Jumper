#include "../include/Sound.hpp"
#include <iostream>
#include "../include/Resources.hpp"

int Sound::SOUND_VOLUME = 3;

Sound::Sound() {
    chunk = nullptr;
    channel = -1;
}

Sound::Sound(string file): Sound() {
    Open(file);
}

void Sound::Play(int times) {
    channel = Mix_PlayChannel(-1, chunk.get(), times - 1);
    if (channel == -1) {
        cerr << "Erro - Mix_PlayChannel: " << SDL_GetError() << endl;
        exit(1);
    }
    Mix_Volume(channel, SOUND_VOLUME*10);
}

void Sound::Stop() {
    if (chunk != nullptr && channel != -1) {
        Mix_HaltChannel(channel);
    }
}

void Sound::Open(string file) {
    if (chunk != nullptr) {
        chunk = nullptr;
    }
    chunk = Resources::GetSound(file);
    if (chunk == nullptr) {
        cerr << "Erro - Mix_LoadWAV: " << SDL_GetError() << endl;
        exit(1);
    }
}

Sound::~Sound() {
    if (chunk != nullptr && channel != -1) {
        Mix_HaltChannel(channel);
        chunk = nullptr;
    }
}

bool Sound::IsOpen() {
    return chunk != nullptr;
}
