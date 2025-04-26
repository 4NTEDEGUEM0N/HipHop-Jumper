#include "../include/Text.hpp"

#include <iostream>
#include <ostream>

#include "../include/GameObject.hpp"
#include "../include/Resources.hpp"
#include "../include/Camera.hpp"
#include "../include/Game.hpp"

Text::Text(GameObject& associated, string fontFile, int fontSize, TextStyle style, string text, SDL_Color color) : Component(associated){
    texture = nullptr;
    this->text = text;
    this->style = style;
    this->fontFile = fontFile;
    this->fontSize = fontSize;
    this->color = color;

    font = Resources::GetFont(fontFile, fontSize).get();
    RemakeTexture();
}

Text::~Text() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
    }
}

void Text::Update(float dt) {}

void Text::Render() {
    if (texture != nullptr) {
        SDL_Rect dstRect;
        dstRect.x = associated.box.X - Camera::pos.GetX();
        dstRect.y = associated.box.Y - Camera::pos.GetY();
        dstRect.w = associated.box.W;
        dstRect.h = associated.box.H;

        SDL_Rect clipRect = {0, 0, dstRect.w, dstRect.h};

        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &clipRect, &dstRect, 0, nullptr, SDL_FLIP_NONE);
    }
}

void Text::SetText(string text) {
    this->text = text;
    RemakeTexture();
}

void Text::SetColor(SDL_Color color) {
    this->color = color;
    RemakeTexture();
}

void Text::SetStyle(TextStyle style) {
    this->style = style;
    RemakeTexture();
}


void Text::SetFontSize(int fontSize) {
    this->fontSize = fontSize;
    RemakeTexture();
}

void Text::SetFontFile(string fontFile) {
    this->fontFile = fontFile;
    RemakeTexture();
}

void Text::RemakeTexture() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    font = Resources::GetFont(fontFile, fontSize).get();
    if (font == nullptr) {
        cerr << "Erro ao carregar fonte" << endl;
        exit(1);
    }

    SDL_Surface* surface = nullptr;
    switch (style) {
        case TextStyle::SOLID:
            surface = TTF_RenderText_Solid(font, text.c_str(), color);
        break;
        case TextStyle::SHADED:
            surface = TTF_RenderText_Shaded(font, text.c_str(), color, {0, 0, 0});
        break;
        case TextStyle::BLENDED:
            surface = TTF_RenderText_Blended(font, text.c_str(), color);
        break;
    }

    texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
    SDL_FreeSurface(surface);

    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    associated.box.W = width;
    associated.box.H = height;
}

bool Text::Is(string type) {
    return type == "Text";
}




