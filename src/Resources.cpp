#include "../include/Resources.hpp"
#include "../include/Game.hpp"

#include <iostream>
#include <SDL_image.h>

using namespace std;

unordered_map<string, shared_ptr<SDL_Texture>> Resources::imageTable;
unordered_map<string, shared_ptr<Mix_Music>> Resources::musicTable;
unordered_map<string, shared_ptr<Mix_Chunk>> Resources::soundTable;
unordered_map<string, shared_ptr<TTF_Font>> Resources::fontTable;
unordered_map<string, shared_ptr<SDL_Texture>> Resources::textureTable;

void SDL_Texture_Deleter(SDL_Texture* texture) {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
}
shared_ptr<SDL_Texture> Resources::GetImage(string file) {
    auto search = imageTable.find(file);
    if (search == imageTable.end()) {
        SDL_Texture* texture = IMG_LoadTexture(Game::GetInstance().GetRenderer(), file.c_str());
        if (texture == nullptr) {
            cerr << "Erro - IMG_LoadTexture: " << SDL_GetError() << endl;
            exit(1);
        }
        shared_ptr<SDL_Texture> texturePtr(texture, SDL_Texture_Deleter);
        imageTable[file] = texturePtr;
        return imageTable[file];

    }
    return search->second;
}

void Resources::ClearImages() {
    for (auto it = imageTable.begin(); it != imageTable.end(); ) {
        if (it->second.use_count() == 1) {
            it = imageTable.erase(it);
        } else {
            it++;
        }
    }
}

shared_ptr<SDL_Texture> Resources::GetTexture(SDL_Texture* texture, string key) {
    auto search = textureTable.find(key);
    if (search == textureTable.end()) {
        shared_ptr<SDL_Texture> texturePtr(texture, SDL_Texture_Deleter);
        textureTable[key] = texturePtr;
        return textureTable[key];

    }
    return search->second;
}

void Resources::ClearTexture() {
    for (auto it = textureTable.begin(); it != textureTable.end(); ) {
        if (it->second.use_count() == 1) {
            it = textureTable.erase(it);
        } else {
            it++;
        }
    }
}


void SDL_Music_Deleter(Mix_Music* music) {
    if (music != nullptr) {
        Mix_FreeMusic(music);
    }
}
shared_ptr<Mix_Music> Resources::GetMusic(string file) {
    Mix_Music* music = nullptr;
    auto search = musicTable.find(file);
    if (search == musicTable.end()) {
        music = Mix_LoadMUS(file.c_str());
        if (music == nullptr) {
            cerr << "Erro - Mix_LoadMUS: " << SDL_GetError() << endl;
            exit(1);
        }
        shared_ptr<Mix_Music> musicPtr(music, SDL_Music_Deleter);
        musicTable[file] = musicPtr;
        return musicTable[file];
    }
    return search->second;
}

void Resources::ClearMusics() {
    for (auto it = musicTable.begin(); it != musicTable.end(); ) {
        if (it->second.use_count() == 1) {
            it = musicTable.erase(it);
        } else {
            it++;
        }
    }
}

void SDL_Sound_Deleter(Mix_Chunk* chunk) {
    if (chunk != nullptr) {
        Mix_FreeChunk(chunk);
    }
}
shared_ptr<Mix_Chunk> Resources::GetSound(string file) {
    Mix_Chunk* chunk = nullptr;
    auto search = soundTable.find(file);
    if (search == soundTable.end()) {
        chunk = Mix_LoadWAV(file.c_str());
        if (chunk == nullptr) {
            cerr << "Erro - Mix_LoadWAV: " << SDL_GetError() << endl;
            exit(1);
        }
        shared_ptr<Mix_Chunk> chunkPtr(chunk, SDL_Sound_Deleter);
        soundTable[file] = chunkPtr;
        return soundTable[file];
    }
    return search->second;
}

void Resources::ClearSounds() {
    for (auto it = soundTable.begin(); it != soundTable.end(); ) {
        if (it->second.use_count() == 1) {
            it = soundTable.erase(it);
        } else {
            it++;
        }
    }
}

void TTF_Font_Deleter(TTF_Font* font) {
    if (font != nullptr) {
        TTF_CloseFont(font);
    }
}
shared_ptr<TTF_Font> Resources::GetFont(string file, int size) {
    string key = file + "_" + to_string(size);
    auto search = fontTable.find(key);
    if (search == fontTable.end()) {
        TTF_Font* font = TTF_OpenFont(file.c_str(), size);
        if (font == nullptr) {
            cerr << "Erro - TTF_OpenFont: " << SDL_GetError() << endl;
            exit(1);
        }
        shared_ptr<TTF_Font> fontPtr(font, TTF_Font_Deleter);
        fontTable[key] = fontPtr;
        return fontTable[key];

    }
    return search->second;
}

void Resources::ClearFonts() {
    for (auto it = fontTable.begin(); it != fontTable.end(); ) {
        if (it->second.use_count() == 1) {
            it = fontTable.erase(it);
        } else {
            it++;
        }
    }
}

