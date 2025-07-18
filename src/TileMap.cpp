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

TileMap::TileMap(GameObject& associated, string file, vector<TileSet*>& tileSets, set<int> solidIDs, int collisionLayer) : Component(associated) {
    this->solidIDs = solidIDs;
    Load(file);
    this->tileSets = tileSets;
    this->collisionLayer = collisionLayer;
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

int& TileMap::At(int x, int y, int z) {
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight || z < 0 || z >= mapDepth) {
        cerr << "Erro - TileMap::At - Posição Inválida : " << endl;
        cerr << "x: " << x << endl;
        cerr << "y: " << y << endl;
        cerr << "z: " << z << endl;
        cerr << endl;
        cerr << "mapWidth: " << mapWidth << endl;
        cerr << "mapHeight: " << mapHeight << endl;
        cerr << "mapDepth: " << mapDepth << endl;
        exit(1);
    }
    return tileMatrix[x + y * mapWidth + z * mapWidth * mapHeight];
}

void TileMap::RenderLayer(int layer) {
    TileSet* currentTileSet = tileSets[layer];

    int tileWidth = currentTileSet->GetTileWidth();
    int tileHeight = currentTileSet->GetTileHeight();

    float baseX = associated.box.X;
    float baseY = associated.box.Y;

    float paralaxFactor = 0;// = ((-1.0f * (layer + 1))/mapDepth) + 1;
    if (layer == 0)
        paralaxFactor = 0;
    baseX -= Camera::pos.GetX() * paralaxFactor;
    baseY -= Camera::pos.GetY() * paralaxFactor;

    //int renderCount = 0;

    // Coordenadas do início e fim visíveis no mapa
    int startX = max(0, (int)((Camera::pos.GetX() - baseX) / tileWidth));
    int endX = min(mapWidth, (int)((Camera::pos.GetX() + Game::VirtualScreenWidth - baseX) / tileWidth + 1));
    int startY = max(0, (int)((Camera::pos.GetY() - baseY) / tileHeight));
    int endY = min(mapHeight, (int)((Camera::pos.GetY() + Game::VirtualScreenHeight - baseY) / tileHeight + 1));

    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            int index = At(x, y, layer);
            currentTileSet->RenderTile(index, baseX + x * tileWidth, baseY + y * tileHeight);
            //renderCount++;
#ifdef DEBUG
            if (Collider::showCollision and layer == 0 and collisionMatrix[y][x] != TileCollisionType::None) {
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
    for (int z = mapDepth-1; z >= 0; --z) {
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
    //SetCollisionLayer(collisionLayer);
    SetCollisionMatrix(collisionLayer);
}

void TileMap::SetCollisionLayer(int layer) {
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int index = At(x, y, layer);
            if (solidIDs.find(index) != solidIDs.end()) {
                int tileWidth = tileSets[layer]->GetTileWidth();
                int tileHeight = tileSets[layer]->GetTileHeight();
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
                if (index == 15 or index == 166 or index == 205) {
                    collisionMatrix[y][x] = TileCollisionType::TriangleTopLeft;
                } else if (index == 16 or index == 134 or index == 206 or index == 215) {
                    collisionMatrix[y][x] = TileCollisionType::TriangleTopRight;
                } else if (index == 18 or index == 246) {
                    collisionMatrix[y][x] = TileCollisionType::TriangleBottomLeft;
                } else if (index == 19 or index == 125 or index == 216 or index == 247) {
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

vector<TileMap::CollisionInfo> TileMap::IsColliding(Rect boxOG, Vec2 scale) {
    vector<CollisionInfo> collisions;
    Rect box;
    box.W = boxOG.W * scale.GetX();
    box.H = boxOG.H * scale.GetY();
    Vec2 box_center = boxOG.center();
    box.X = box_center.X - box.W / 2;
    box.Y = box_center.Y - box.H / 2;

    // 1. Calcula a área de tiles que a "box" ocupa
    int tileWidth = GetTileSetWidth(); // Usando o layer de colisão padrão
    int tileHeight = GetTileSetHeight();

    // Arredonda para baixo para o tile inicial e para cima para o final
    int startX = static_cast<int>(box.X) / tileWidth;
    int endX = static_cast<int>(box.X + box.W) / tileWidth;
    int startY = static_cast<int>(box.Y) / tileHeight;
    int endY = static_cast<int>(box.Y + box.H) / tileHeight;

    // 2. Itera APENAS sobre os tiles relevantes
    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {

            TileCollisionType type = GetCollisionType(x, y);

            // 3. Se o tile for sólido, verifica a colisão
            if (type != TileCollisionType::None) {
                bool collided = false;

                // Para tiles retangulares, a colisão é garantida se chegamos aqui
                if (type == TileCollisionType::Full) {
                    collided = true;
                }
                // Para tiles triangulares, fazemos a verificação mais detalhada
                else {
                    if (RectCollidesTriangle(box, x, y, type, tileWidth, tileHeight)) {
                        collided = true;
                    }
                }

                if (collided) {
                    // 4. Adiciona a informação da colisão
                    // Nota: O 'CollisionCorner' perde um pouco o sentido aqui.
                    // É mais útil saber qual tile colidiu e seu tipo.
                    // Podemos adaptar ou simplificar a struct CollisionInfo no futuro.
                    CollisionInfo info;
                    info.tilePos = Vec2(x, y);
                    info.type = type;
                    // Você pode adicionar uma lógica para determinar o "canto" se ainda precisar,
                    // mas geralmente não é necessário com essa abordagem.
                    collisions.push_back(info);
                }
            }
        }
    }

    return collisions;
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

int TileMap::GetTileSetWidth(int layer) {
    return tileSets[layer]->GetTileWidth();
}

int TileMap::GetTileSetHeight(int layer) {
    return tileSets[layer]->GetTileHeight();
}

