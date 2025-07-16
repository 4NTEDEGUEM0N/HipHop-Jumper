#pragma once
#include <string>
#include "Sound.hpp"

class MusicManager {
public:
    static MusicManager& GetInstance();

    void PlayMusic(const std::string& intro, const std::string& loop, const std::string& end = "");
    void StopMusic(bool playEnd = true);
    void Update(float dt);

private:
    MusicManager();
    MusicManager(const MusicManager&) = delete;
    MusicManager& operator=(const MusicManager&) = delete;

    Sound introTrack;
    Sound loopTrack;
    Sound endTrack;

    bool playing;
    bool introFinished;
    bool requestEnd;
    bool hasEndTrack;
};
