#include "../include/AmbientManager.hpp"
#include <unordered_set>

AmbientManager::AmbientManager() {}

void AmbientManager::AddRegion(Rect area, const std::string& soundFile) {
    AmbientRegion region;
    region.area = area;
    region.soundFile = soundFile;
    region.ambientSound.Open(soundFile);
    regions.push_back(region);
}

void AmbientManager::Update(const Vec2& playerPosition) {
    std::unordered_set<AmbientRegion*> newActiveRegions;

    for (auto& region : regions) {
        if (region.area.contains(playerPosition)) {
            newActiveRegions.insert(&region);
            if (activeRegions.find(&region) == activeRegions.end()) {
                region.ambientSound.Play(-1);
            }
        }
    }

    // Stop sounds for regions that are no longer active
    for (auto* region : activeRegions) {
        if (newActiveRegions.find(region) == newActiveRegions.end()) {
            region->ambientSound.Stop();  // just exited this region
        }
    }

    // Update the set of active regions
    activeRegions = std::move(newActiveRegions);
}

void AmbientManager::StopAll() {
    for (auto& region : regions) {
        region.ambientSound.Stop();
    }
    activeRegions.clear();
}

void AmbientManager::PauseAll() {
    for (auto& region : regions) {
        region.ambientSound.Pause();
    }
}

void AmbientManager::ResumeAll() {
    for (auto& region : regions) {
        region.ambientSound.Resume();
        region.ambientSound.UpdateVolume();
    }
}
