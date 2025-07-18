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
    vector<TileSet*> tileSets;
    int mapWidth;
    int mapHeight;
    int mapDepth;
    set<int> solidIDs;
    int collisionLayer;

public:
    TileMap(GameObject& associated, string file, vector<TileSet*>& tileSets, set<int> solidIDs, int collisionLayer);
    void Load(string file);
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
    int GetTileSetWidth(int layer = 0);
    int GetTileSetHeight(int layer = 0);
    set<int> TriangleTopLeftIDs;
    set<int> TriangleTopRightIDs;
    set<int> TriangleBottomLeftIDs;
    set<int> TriangleBottomRightIDs;

    enum class CollisionCorner {
        TopLeft, TopRight, BottomLeft, BottomRight, CenterBottom, CenterTop, CenterLeft, CenterRight,
    };
    enum class TileCollisionType {
        None,
        Full,
        TriangleTopLeft,
        TriangleTopRight,
        TriangleBottomLeft,
        TriangleBottomRight
    };
    struct CollisionInfo {
        Vec2 tilePos;
        TileCollisionType type;
    };
    TileCollisionType GetCollisionType(int x, int y);
    vector<CollisionInfo> IsColliding(Rect boxOG, Vec2 scale={1,1});
    vector<vector<TileCollisionType>> collisionMatrix;
    bool RectCollidesTriangle(Rect box, int tileX, int tileY, TileCollisionType type, int tileW, int tileH);
    bool PointInTriangle(Vec2 pt, Vec2 v1, Vec2 v2, Vec2 v3);
    float Sign(Vec2 p1, Vec2 p2, Vec2 p3);
};

#endif //TILEMAP_HPP
