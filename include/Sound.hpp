#ifndef SOUND_HPP
#define SOUND_HPP
#include <SDL_mixer.h>
#include <string>
#include <memory>

using namespace std;

class Sound {
private:
    shared_ptr<Mix_Chunk> chunk;
    int channel;

public:
    Sound();
    Sound(string file);
    ~Sound();
    void Play(int times = 1);
    void Stop();
    void Open(string file);
    bool IsOpen();
    bool IsPlaying() const;
    static int SOUND_VOLUME;
    
    bool isLooping = false;
};


#endif //SOUND_HPP
