#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <memory>
#include <set>
#include <vector>
#include "Component.hpp"
#include "TileSet.hpp"
#include "Rect.hpp"

using namespace std;

class TileMap : public Component {
private:
    vector<int> tileMatrix;
    unique_ptr<TileSet> tileSet;
    int mapWidth;
    int mapHeight;
    int mapDepth;
    set<int> solidIDs;

public:
    TileMap(GameObject& associated, string file, TileSet* tileSet);
    void Load(string file);
    void SetTileSet(TileSet* tileSet);
    int& At(int x, int y, int z = 0);
    void Render();
    void RenderLayer(int layer);
    void Start();
    int GetWidth();
    int GetHeight();
    int GetDepth();
    void Update(float dt);
    bool Is(string type);
    void SetCollisionLayer(int layer);
    void SetCollisionMatrix(int layer);
    bool IsSolid(int x, int y);
    bool IsColliding(Rect box);

    vector<vector<bool>> collisionMatrix;
};

#endif //TILEMAP_HPP
