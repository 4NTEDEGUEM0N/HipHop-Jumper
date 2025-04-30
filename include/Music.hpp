#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <memory>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

using namespace std;

class Music {
private:
    shared_ptr<Mix_Music> music;

public:
    Music();
    Music(string file);
    ~Music();
    void Play(int times = -1);
    void Stop(int msToStop = 1500);
    void Open(string file);
    bool IsOpen();
};

#endif //MUSIC_HPP
