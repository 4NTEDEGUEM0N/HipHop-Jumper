//
//  AmbientManager.hpp
//  hhj
//
//  Created by Pedro Venzi on 15/07/25.
//

#ifndef AmbientManager_hpp
#define AmbientManager_hpp

#pragma once
#include <stdio.h>
#include <vector>
#include <unordered_set>
#include <functional>
#include "GameObject.hpp"
#include "Sound.hpp"

class AmbientRegion {
    
public:
    
    Rect area;
    std::string soundFile;
    Sound ambientSound;
};

class AmbientManager {
    
public:
    
    static AmbientManager& GetInstance() {
        static AmbientManager instance;
        return instance;
    }

    AmbientManager();
    ~AmbientManager() = default;

    AmbientManager(const AmbientManager&) = delete;
    AmbientManager& operator=(const AmbientManager&) = delete;

    void AddRegion(Rect area, const std::string& soundFile);
    void Update(const Vec2& playerPosition);
    void StopAll();

private:
    std::vector<AmbientRegion> regions;
    std::unordered_set<AmbientRegion*> activeRegions;
};

#endif /* AmbientManager_hpp */
