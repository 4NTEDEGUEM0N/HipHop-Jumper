#include "../include/AmbientManager.hpp"

AmbientManager::AmbientManager() : currentRegion(nullptr) {}

void AmbientManager::AddRegion(Rect area, const std::string& soundFile) {
    AmbientRegion region;
    region.area = area;
    region.soundFile = soundFile;
    region.ambientSound.Open(soundFile);
    regions.push_back(region);
}

void AmbientManager::Update(Vec2& playerPosition) {
    for (auto& region : regions) {
        if (region.area.contains(playerPosition)) {
            if (&region != currentRegion) {
                if (currentRegion)
                    currentRegion->ambientSound.Stop();
                region.ambientSound.Play(-1);
                currentRegion = &region;
            }
            return;
        }
    }

    if (currentRegion) {
        currentRegion->ambientSound.Stop();
        currentRegion = nullptr;
    }
}

void AmbientManager::StopAll() {
    for (auto& region : regions) {
        region.ambientSound.Stop();
    }
    currentRegion = nullptr;
}


