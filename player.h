#ifndef PLAYER_H
#define PLAYER_H

#include "unit.h"

class Player : public Unit
{
public:
    Player();

    Tile::TileType tileType() const;
    void setTileType(Tile::TileType tileType);

private:
    Tile::TileType m_tileType;
};

#endif // PLAYER_H
