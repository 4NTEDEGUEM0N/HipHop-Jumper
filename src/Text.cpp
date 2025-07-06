#include "../include/Text.hpp"

#include <iostream>
#include <sstream>

#include "../include/GameObject.hpp"
#include "../include/Resources.hpp"
#include "../include/Camera.hpp"
#include "../include/Game.hpp"

Text::Text(GameObject& associated, string fontFile, int fontSize, TextStyle style, string text, SDL_Color color, bool cameraFollower) : Component(associated){
    texture = nullptr;
    this->text = text;
    this->style = style;
    this->fontFile = fontFile;
    this->fontSize = fontSize;
    this->color = color;
    this->cameraFollower = cameraFollower;

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
        if (!cameraFollower) {
            dstRect.x = associated.box.X - Camera::pos.GetX();
            dstRect.y = associated.box.Y - Camera::pos.GetY();
        } else {
            dstRect.x = associated.box.X;
            dstRect.y = associated.box.Y;
        }

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

    if (text.find('\n') == string::npos) {
        SDL_Surface* surface = nullptr;
        switch (style) {
            case TextStyle::SOLID:
                surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
            break;
            case TextStyle::SHADED:
                surface = TTF_RenderUTF8_Shaded(font, text.c_str(), color, {0, 0, 0});
            break;
            case TextStyle::BLENDED:
                surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
            break;
        }

        texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
        SDL_FreeSurface(surface);

        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
        associated.box.W = width;
        associated.box.H = height;
    } else {
        std::stringstream ss(text);
        std::string line;
        std::vector<SDL_Surface*> line_surfaces;

        int max_width = 0;
        int total_height = 0;

        // 1. Renderizar cada linha em uma superfície separada
        while (std::getline(ss, line)) {
            if (line.empty()) { // Trata linhas em branco como um espaço vertical
                line = " ";
            }

            SDL_Surface* surface = nullptr;
            switch (style) {
                case TextStyle::SOLID:
                    surface = TTF_RenderUTF8_Solid(font, line.c_str(), color);
                break;
                case TextStyle::SHADED:
                    // Para SHADED, a cor de fundo precisa ser definida
                        surface = TTF_RenderUTF8_Shaded(font, line.c_str(), color, {0, 0, 0, 0});
                break;
                case TextStyle::BLENDED:
                    surface = TTF_RenderUTF8_Blended(font, line.c_str(), color);
                break;
            }

            if (surface) {
                line_surfaces.push_back(surface);
                if (surface->w > max_width) {
                    max_width = surface->w;
                }
                total_height += surface->h;
            }
        }

        // Se nenhuma superfície foi criada (ex: texto contém apenas '\n'), sair
        if (line_surfaces.empty()) {
            associated.box.W = 0;
            associated.box.H = 0;
            return;
        }

        // 2. Criar a textura final que servirá de tela
        texture = SDL_CreateTexture(Game::GetInstance().GetRenderer(), SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET, max_width, total_height);

        // Habilitar blending para a textura alvo
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(Game::GetInstance().GetRenderer(), texture);

        // Limpar a textura alvo (deixa transparente)
        SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 0, 0, 0, 0);
        SDL_RenderClear(Game::GetInstance().GetRenderer());

        int current_y = 0;
        // 3. Copiar cada superfície de linha para a textura final
        for (SDL_Surface* line_surface : line_surfaces) {
            SDL_Texture* line_texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), line_surface);

            SDL_Rect dstRect = {0, current_y, line_surface->w, line_surface->h};
            SDL_RenderCopy(Game::GetInstance().GetRenderer(), line_texture, nullptr, &dstRect);

            current_y += line_surface->h;

            // Limpeza da textura e superfície da linha
            SDL_DestroyTexture(line_texture);
            SDL_FreeSurface(line_surface);
        }

        // Resetar o alvo de renderização para a tela principal
        SDL_SetRenderTarget(Game::GetInstance().GetRenderer(), nullptr);

        // Atualizar as dimensões do GameObject associado
        associated.box.W = max_width;
        associated.box.H = total_height;
    }
}


bool Text::Is(string type) {
    return type == "Text";
}




