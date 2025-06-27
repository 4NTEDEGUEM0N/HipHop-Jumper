cmake_minimum_required(VERSION 3.28)
project(HipHop-Jumper)

set(CMAKE_CXX_STANDARD 14)

if(WIN32)
    set(CMAKE_GENERATOR_PLATFORM x64)
    include_directories(SYSTEM "C:/SDL2/include")
    include_directories(SYSTEM "C:/SDL2/include/SDL2")
    link_directories("C:/SDL2/lib")
    set(SDL2_PATH "C:/SDL2")
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

set(MAIN main.cpp)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.hpp)
add_executable(HipHop-Jumper ${MAIN} ${SOURCES} ${HEADERS})

if (WIN32)
    target_link_libraries(HipHop-Jumper
            SDL2
            SDL2main
            SDL2_image
            SDL2_mixer
            SDL2_ttf
            -lmingw32
            imm32
            winmm
            setupapi
            version
            cfgmgr32
            rpcrt4
            shell32
            user32
            gdi32
            ole32
            oleaut32
            ws2_32
            "${SDL2_PATH}/lib/libSDL2_ttf.a"
            "${SDL2_PATH}/lib/libSDL2_mixer.a"
            "${SDL2_PATH}/lib/libSDL2_image.a"
            "${SDL2_PATH}/lib/libSDL2.a"
            "${SDL2_PATH}/lib/libSDL2main.a"
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