#include "../include/TileMap.hpp"
#include "../include/Tile.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include "../include/Camera.hpp"
#include "../include/GameObject.hpp"
#include "../include/Collider.hpp"
#include "../include/Game.hpp"

TileMap::TileMap(GameObject& associated, string file, TileSet* tileSet) : Component(associated) {
    solidIDs = {3,4,10,11,14,15,16,17,18,21,22,23,28,29,30};
    Load(file);
    SetTileSet(tileSet);
}

void TileMap::Load(string file) {
    ifstream tileMapFile;
    tileMapFile.open(file);
    if (!tileMapFile.is_open()) {
        cerr << "Erro - TileMap::Load - Open" << endl;
        exit(1);
    }

    string linha;
    tileMapFile >> linha;
    stringstream ss(linha);

    string numero;
    int i = 0;
    while (getline(ss, numero, ',')) {
        if (i == 0)
            mapWidth = stoi(numero);
        else if (i == 1)
            mapHeight = stoi(numero);
        else if (i == 2)
            mapDepth = stoi(numero);
        i++;
    }

    int tileCount = 0;
    tileMatrix.reserve(mapWidth * mapHeight * mapDepth);
    while (tileMapFile >> linha) {
        stringstream ss(linha);

        while (getline(ss, numero, ',')) {
            tileMatrix.push_back(stoi(numero));
            tileCount++;
        }
    }

    if (tileCount != mapWidth * mapHeight * mapDepth) {
        cerr << "Erro - TileMap::Load - tileCount" << endl;
        exit(1);
    }

    tileMapFile.close();
}

void TileMap::SetTileSet(TileSet* tileSet) {
    this->tileSet.reset(tileSet);
}

int& TileMap::At(int x, int y, int z) {
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight || z < 0 || z >= mapDepth) {
        cerr << "Erro - TileMap::At - Posição Inválida" << endl;
        exit(1);
    }
    return tileMatrix[x + y * mapWidth + z * mapWidth * mapHeight];
}

void TileMap::RenderLayer(int layer) {
    int tileWidth = tileSet->GetTileWidth();
    int tileHeight = tileSet->GetTileHeight();

    float baseX = associated.box.X;
    float baseY = associated.box.Y;

    float paralaxFactor = ((-1.0f * (layer + 1))/mapDepth) + 1;
    baseX -= Camera::pos.GetX() * paralaxFactor;
    baseY -= Camera::pos.GetY() * paralaxFactor;

    //int renderCount = 0;

    int screenWidth = 1200;
    int screenHeight = 900;

    // Coordenadas do início e fim visíveis no mapa
    int startX = max(0, (int)((Camera::pos.GetX() - baseX) / tileWidth));
    int endX = min(mapWidth, (int)((Camera::pos.GetX() + screenWidth - baseX) / tileWidth + 1));
    int startY = max(0, (int)((Camera::pos.GetY() - baseY) / tileHeight));
    int endY = min(mapHeight, (int)((Camera::pos.GetY() + screenHeight - baseY) / tileHeight + 1));

    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            int index = At(x, y, layer);
            tileSet->RenderTile(index, baseX + x * tileWidth, baseY + y * tileHeight);
            //renderCount++;
#ifdef DEBUG
            if (Collider::showCollision and layer == 1 and collisionMatrix[y][x] != TileCollisionType::None) {
                SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);

                int x0 = baseX + x * tileWidth - Camera::pos.X;
                int y0 = baseY + y * tileHeight - Camera::pos.Y;
                switch (collisionMatrix[y][x]) {

                case TileCollisionType::Full: {
                    SDL_Rect rect = {x0, y0, tileWidth, tileHeight};
                    SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &rect);
                    break;
                }
                    case TileCollisionType::TriangleBottomRight: {
                        SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), x0, y0 + tileHeight, x0, y0); // esquerda inferior → esquerda superior
                        SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), x0, y0, x0 + tileWidth, y0);  // esquerda superior → direita superior
                        SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), x0 + tileWidth, y0, x0, y0 + tileHeight); // direita superior → esquerda inferior
                        break;
                    }
                    case TileCollisionType::TriangleBottomLeft: {
                        SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), x0 + tileWidth, y0 + tileHeight, x0 + tileWidth, y0); // direita inferior → direita superior
                        SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), x0 + tileWidth, y0, x0, y0); // direita superior → esquerda superior
                        SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), x0, y0, x0 + tileWidth, y0 + tileHeight); // esquerda superior → direita inferior
                        break;
                    }
                    case TileCollisionType::TriangleTopRight: {
                        SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), x0, y0, x0, y0 + tileHeight); // esquerda superior → esquerda inferior
                        SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), x0, y0 + tileHeight, x0 + tileWidth, y0 + tileHeight); // esquerda inferior → direita inferior
                        SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), x0 + tileWidth, y0 + tileHeight, x0, y0); // direita inferior → esquerda superior
                        break;
                    }
                    case TileCollisionType::TriangleTopLeft: {
                        SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), x0 + tileWidth, y0, x0 + tileWidth, y0 + tileHeight); // direita superior → direita inferior
                        SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), x0 + tileWidth, y0 + tileHeight, x0, y0 + tileHeight); // direita inferior → esquerda inferior
                        SDL_RenderDrawLine(Game::GetInstance().GetRenderer(), x0, y0 + tileHeight, x0 + tileWidth, y0); // esquerda inferior → direita superior
                        break;
                    }
                default:
                    break;
                }
#endif
            }
        }
    }
    //cerr << "Tiles renderizados: " << renderCount << endl;
}

void TileMap::Render() {
    for (int z = 0; z < mapDepth; ++z) {
        RenderLayer(z);
    }
}

int TileMap::GetWidth() {
    return mapWidth;
}

int TileMap::GetHeight() {
    return mapHeight;
}

int TileMap::GetDepth() {
    return mapDepth;
}

void TileMap::Update(float dt) {}

bool TileMap::Is(string type) {
    return type == "TileMap";
}

void TileMap::Start() {
    //SetCollisionLayer(1);
    SetCollisionMatrix(1);
}

void TileMap::SetCollisionLayer(int layer) {
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int index = At(x, y, layer);
            if (solidIDs.find(index) != solidIDs.end()) {
                int tileWidth = tileSet->GetTileWidth();
                int tileHeight = tileSet->GetTileHeight();
                //cerr << "ID: " << index << " X: " << x*tileWidth << " Y: "<< y*tileHeight << endl;

                GameObject* tileObj = new GameObject();
                tileObj->box.X = x*tileWidth;
                tileObj->box.Y = y*tileHeight;
                tileObj->box.H = tileWidth;
                tileObj->box.W = tileHeight;
                Tile* tileCpt = new Tile(*tileObj, index);
                tileObj->AddComponent(tileCpt);
                State& state = Game::GetInstance().GetState();
                state.AddObject(tileObj);
            }
        }
    }
}

void TileMap::SetCollisionMatrix(int layer) {
    collisionMatrix = vector<vector<TileCollisionType>>(mapHeight, vector<TileCollisionType>(mapWidth, TileCollisionType::None));

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int index = At(x, y, layer);
            if (solidIDs.find(index) != solidIDs.end()) {
                if (index == 14) {
                    collisionMatrix[y][x] = TileCollisionType::TriangleTopLeft;
                } else if (index == 16) {
                    collisionMatrix[y][x] = TileCollisionType::TriangleTopRight;
                } else if (index == 28) {
                    collisionMatrix[y][x] = TileCollisionType::TriangleBottomLeft;
                } else if (index == 30) {
                    collisionMatrix[y][x] = TileCollisionType::TriangleBottomRight;
                } else {
                    collisionMatrix[y][x] = TileCollisionType::Full;
                }
            }
        }
    }
}

TileMap::TileCollisionType TileMap::GetCollisionType(int x, int y) {
    if (x < 0 || y < 0 || x >= mapWidth || y >= mapHeight)
        return TileCollisionType::None;
    return collisionMatrix[y][x];
}

bool TileMap::IsColliding(Rect box) {
    int tileWidth = tileSet->GetTileWidth();
    int tileHeight = tileSet->GetTileHeight();


    int left   = box.X / tileWidth;
    int right  = (box.X + box.W) / tileWidth;
    int top    = box.Y / tileHeight;
    int bottom = (box.Y + box.H) / tileHeight;

    Vec2 topLeft = Vec2(left, top);
    Vec2 topRight = Vec2(right, top);
    Vec2 bottomLeft = Vec2(left, bottom);
    Vec2 bottomRight = Vec2(right, bottom);

    TileCollisionType type_topLeft = GetCollisionType(topLeft.X, topLeft.Y);
    TileCollisionType type_topRight = GetCollisionType(topRight.X, topRight.Y);
    TileCollisionType type_bottomLeft = GetCollisionType(bottomLeft.X, bottomLeft.Y);
    TileCollisionType type_bottomRight = GetCollisionType(bottomRight.X, bottomRight.Y);

    if (type_topLeft != TileCollisionType::None) {
        if (type_topLeft != TileCollisionType::Full) {
            if (RectCollidesTriangle(box, topLeft.X, topLeft.Y, type_topLeft, tileWidth, tileHeight)) {
                //cerr << "Colidiu Top Left" << endl;
                return true;
            }
        } else {
            //cerr << "Colidiu Top Left" << endl;
            return true;
        }
    }
    if (type_topRight != TileCollisionType::None) {
        if (type_topRight != TileCollisionType::Full) {
            if (RectCollidesTriangle(box, topRight.X, topRight.Y, type_topRight, tileWidth, tileHeight)) {
                //cerr << "Colidiu Top Right" << endl;
                return true;
            }
        } else {
            //cerr << "Colidiu Top Right" << endl;
            return true;
        }
    }
    if (type_bottomLeft != TileCollisionType::None) {
        if (type_bottomLeft != TileCollisionType::Full) {
            if (RectCollidesTriangle(box, bottomLeft.X, bottomLeft.Y, type_bottomLeft, tileWidth, tileHeight)) {
                //cerr << "Colidiu Bottom Left" << endl;
                return true;
            }
        } else {
            //cerr << "Colidiu Bottom Left" << endl;
            return true;
        }
    }
    if (type_bottomRight != TileCollisionType::None) {
        if (type_bottomRight != TileCollisionType::Full) {
            if (RectCollidesTriangle(box, bottomRight.X, bottomRight.Y, type_bottomRight, tileWidth, tileHeight)) {
                //cerr << "Colidiu Bottom Right" << endl;
                return true;
            }
        } else {
            //cerr << "Colidiu Bottom Right" << endl;
            return true;
        }
    }
    return false;
}

bool TileMap::RectCollidesTriangle(Rect box, int tileX, int tileY, TileCollisionType type, int tileW, int tileH) {
    float startX = tileX * tileW;
    float startY = tileY * tileH;

    // Vértices do triângulo
    Vec2 A, B, C;
    switch(type) {
        case TileCollisionType::TriangleBottomRight:
            A = {startX, startY + tileH};
            B = {startX, startY};
            C = {startX + tileW, startY};
        break;
        case TileCollisionType::TriangleBottomLeft:
            A = {startX + tileW, startY + tileH};
            B = {startX + tileW, startY};
            C = {startX, startY};
        break;
        case TileCollisionType::TriangleTopRight:
            A = {startX, startY};
            B = {startX, startY + tileH};
            C = {startX + tileW, startY + tileH};
        break;
        case TileCollisionType::TriangleTopLeft:
            A = {startX + tileW, startY};
            B = {startX + tileW, startY + tileH};
            C = {startX, startY + tileH};
        break;
        default:
            return false;
    }

    // Verifica se algum canto do box está dentro do triângulo
    Vec2 points[] = {
        {box.X, box.Y},
        {box.X + box.W, box.Y},
        {box.X, box.Y + box.H},
        {box.X + box.W, box.Y + box.H}
    };

    for (auto& p : points) {
        if (PointInTriangle(p, A, B, C))
            return true;
    }
    if (box.contains(A) || box.contains(B) || box.contains(C)) {
        return true;
    }

    return false;
}

bool TileMap::PointInTriangle(Vec2 pt, Vec2 v1, Vec2 v2, Vec2 v3) {
    float d1 = Sign(pt, v1, v2);
    float d2 = Sign(pt, v2, v3);
    float d3 = Sign(pt, v3, v1);

    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

float TileMap::Sign(Vec2 p1, Vec2 p2, Vec2 p3) {
    return (p1.X - p3.X) * (p2.Y - p3.Y) - (p2.X - p3.X) * (p1.Y - p3.Y);
}

int TileMap::GetTileSetWidth() {
    return tileSet->GetTileWidth();
}
int TileMap::GetTileSetHeight() {
    return tileSet->GetTileHeight();
}

