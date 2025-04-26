#ifndef TILESET_HPP
#define TILESET_HPP

#include "Sprite.hpp"
#include <string>

class TileSet {
private:
    Sprite tileSet;
    int tileWidth;
    int tileHeight;
    int tileCount;

public:
    TileSet(string file, int tile_width, int tile_height);
    void RenderTile(unsigned index, float x, float y);
    int GetTileWidth();
    int GetTileHeight();
};

#endif //TILESET_HPP
