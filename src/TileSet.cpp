#include "../include/TileSet.hpp"

#include <iostream>

TileSet::TileSet(string file, int tile_width, int tile_height) : tileSet(file){
    tileWidth = tile_width;
    tileHeight = tile_height;
    tileSet.Open(file);
    if (tileSet.IsOpen()) {
        int textureWidth = tileSet.GetWidth();
        int textureHeight = tileSet.GetHeight();

        int columns = textureWidth / tileWidth;
        int rows = textureHeight / tileHeight;

        tileCount = columns * rows;
        tileSet.SetFrameCount(columns, rows);
    }
}

void TileSet::RenderTile(unsigned index, float x, float y) {
    if (index < tileCount) {
        tileSet.SetFrame(index);
        tileSet.Render(x, y, tileWidth, tileHeight);
    }
}

int TileSet::GetTileWidth() {
    return tileWidth;
}

int TileSet::GetTileHeight() {
    return tileHeight;
}