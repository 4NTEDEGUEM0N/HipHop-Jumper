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
            if (Collider::showCollision and layer == 1 and collisionMatrix[y][x]) {
                SDL_Rect rect;
                rect.x = baseX + x * tileWidth - Camera::pos.X;
                rect.y = baseY + y * tileHeight - Camera::pos.Y;
                rect.w = tileWidth;
                rect.h = tileHeight;

                SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &rect);
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
    collisionMatrix = vector<vector<bool>>(mapHeight, vector<bool>(mapWidth, false));

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int index = At(x, y, layer);
            collisionMatrix[y][x] = (solidIDs.count(index) > 0);
        }
    }
}

bool TileMap::IsSolid(int x, int y) {
    if (x < 0 || y < 0 || x >= mapWidth || y >= mapHeight)
        return false;
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

    if (IsSolid(topLeft.X, topLeft.Y)) {
        //cerr << "Colidiu Top Left" << endl;
        return true;
    }
    if (IsSolid(topRight.X, topRight.Y)) {
        //cerr << "Colidiu Top Right" << endl;
        return true;
    }
    if (IsSolid(bottomLeft.X, bottomLeft.Y)) {
        //cerr << "Colidiu Bottom Left" << endl;
        return true;
    }
    if (IsSolid(bottomRight.X, bottomRight.Y)) {
        //cerr << "Colidiu Bottom Right" << endl;
        return true;
    }
    return false;
}


