#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <memory>
#include <vector>
#include "Component.hpp"
#include "TileSet.hpp"

using namespace std;

class TileMap : public Component {
private:
    vector<int> tileMatrix;
    unique_ptr<TileSet> tileSet;
    int mapWidth;
    int mapHeight;
    int mapDepth;

public:
    TileMap(GameObject& associated, string file, TileSet* tileSet);
    void Load(string file);
    void SetTileSet(TileSet* tileSet);
    int& At(int x, int y, int z = 0);
    void Render();
    void RenderLayer(int layer);
    int GetWidth();
    int GetHeight();
    int GetDepth();
    void Update(float dt);
    bool Is(string type);
};

#endif //TILEMAP_HPP
