#include "../include/MusicManager.hpp"

MusicManager::MusicManager()
    : playing(false), introFinished(false), requestEnd(false), hasEndTrack(false) {}

MusicManager& MusicManager::GetInstance() {
    static MusicManager instance;
    return instance;
}

void MusicManager::PlayMusic(const std::string& intro, const std::string& loop, const std::string& end) {
    StopMusic(false);

    introTrack.Open(intro);
    loopTrack.Open(loop);

    hasEndTrack = !end.empty();
    if (hasEndTrack) {
        endTrack.Open(end);
    }

    introTrack.Play(1);

    introFinished = false;
    requestEnd = false;
    playing = true;
}

void MusicManager::StopMusic(bool playEnd) {
    introTrack.Stop();
    loopTrack.Stop();
    endTrack.Stop();

    if (playEnd && hasEndTrack) {
        endTrack.Play(1);
    }

    playing = false;
    introFinished = false;
    requestEnd = false;
}

void MusicManager::Update(float dt) {
    if (!introFinished && !introTrack.IsPlaying()) {
        loopTrack.Play(-1);
        introFinished = true;
    }

    if (requestEnd) {
        loopTrack.Stop();
        if (hasEndTrack) {
            endTrack.Play(1);
        }
        requestEnd = false;
        playing = false;
    }
}
