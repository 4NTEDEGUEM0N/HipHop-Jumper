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

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int index = At(x, y, layer);
            tileSet->RenderTile(index, baseX + x * tileWidth, baseY + y * tileHeight);
        }
    }
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

void TileMap::Start() {
    SetCollisionLayer(1);
}

