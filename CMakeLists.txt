cmake_minimum_required(VERSION 3.28)
project(HipHop-Jumper)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
    set(CMAKE_GENERATOR_PLATFORM x64)
    include_directories(SYSTEM "C:/SDL2/include")
    include_directories(SYSTEM "C:/SDL2/include/SDL2")
    link_directories("C:/SDL2/lib")

    add_definitions(-DSDL_MAIN_HANDLED)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")

elseif (UNIX)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(SDL2_ttf REQUIRED)
endif()


add_compile_definitions(DEBUG)

add_executable(HipHop-Jumper
        main.cpp
        include/Game.hpp
        src/Game.cpp
        include/StageState.hpp
        src/StageState.cpp
        include/Sprite.hpp
        src/Sprite.cpp
        include/Music.hpp
        src/Music.cpp
        include/Vec2.hpp
        src/Vec2.cpp
        include/Rect.hpp
        src/Rect.cpp
        include/Component.hpp
        include/GameObject.hpp
        src/GameObject.cpp
        include/Animation.hpp
        src/Animation.cpp
        include/SpriteRenderer.hpp
        src/SpriteRenderer.cpp
        src/Component.cpp
        include/Zombie.hpp
        src/Zombie.cpp
        include/Animator.hpp
        src/Animator.cpp
        include/TileSet.hpp
        src/TileSet.cpp
        include/TileMap.hpp
        src/TileMap.cpp
        include/Sound.hpp
        src/Sound.cpp
        include/Resources.hpp
        src/Resources.cpp
        include/InputManager.hpp
        src/InputManager.cpp
        include/Timer.hpp
        src/Timer.cpp
        include/Camera.hpp
        src/Camera.cpp
        include/Character.hpp
        include/Gun.hpp
        src/Character.cpp
        src/Gun.cpp
        include/PlayerController.hpp
        src/PlayerController.cpp
        include/Bullet.hpp
        src/Bullet.cpp
        include/Collider.hpp
        src/Collider.cpp
        include/Collision.hpp
        include/WaveSpawner.hpp
        include/Wave.hpp
        src/Wave.cpp
        src/WaveSpawner.cpp
        include/AIController.hpp
        src/AIController.cpp
        include/State.hpp
        src/State.cpp
        include/TitleState.hpp
        src/TitleState.cpp
        include/Text.hpp
        src/Text.cpp
        include/EndState.hpp
        include/GameData.hpp
        src/EndState.cpp
        src/GameData.cpp
        include/Tile.hpp
        src/Tile.cpp
        include/HUD.hpp
        src/HUD.cpp
)

if (WIN32)
    target_link_libraries(HipHop-Jumper
            SDL2
            SDL2main
            SDL2_image
            SDL2_mixer
            SDL2_ttf
            imm32
            winmm
            setupapi
            version
            cfgmgr32
            rpcrt4
    )

    add_custom_command(TARGET HipHop-Jumper POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            C:/SDL2/bin $<TARGET_FILE_DIR:HipHop-Jumper>/bin)

elseif (UNIX)
        target_link_libraries(HipHop-Jumper
                SDL2::SDL2
                SDL2_image::SDL2_image
                SDL2_mixer::SDL2_mixer
                SDL2_ttf::SDL2_ttf
        )
endif ()