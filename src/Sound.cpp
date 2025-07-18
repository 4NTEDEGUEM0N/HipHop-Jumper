#include "../include/Sound.hpp"
#include <iostream>
#include "../include/Resources.hpp"

int Sound::SOUND_VOLUME = 1;

Sound::Sound() {
    chunk = nullptr;
    channel = -1;
}

Sound::Sound(string file): Sound() {
    Open(file);
}

void Sound::Play(int times) {
    // If we're trying to loop, but it's already playing, do nothing
    if (times == -1 && isLooping && Mix_Playing(channel)) {
        return;
    }

    // Actually play the sound
    channel = Mix_PlayChannel(-1, chunk.get(), times - 1);
    if (channel == -1) {
        cerr << "Erro - Mix_PlayChannel: " << SDL_GetError() << endl;
        exit(1);
    }
    

    // Set volume
    Mix_Volume(channel, SOUND_VOLUME * 10);

    // Track looping state
    isLooping = (times == -1);
    
    //std::cout << "channel: " << channel << std::endl;
}

void Sound::Stop() {
    if (chunk != nullptr && channel != -1) {
        Mix_HaltChannel(channel);
        channel = -1;
    }
    isLooping = false;
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

bool Sound::IsPlaying() const {
    return channel != -1 && Mix_Playing(channel);
}

void Sound::Pause() {
    if (channel != -1) {
        Mix_Pause(channel);
    }
}

void Sound::Resume() {
    if (channel != -1) {
        Mix_Resume(channel);
    }
}

void Sound::UpdateVolume() {
    Mix_Volume(channel, SOUND_VOLUME*10);
}
