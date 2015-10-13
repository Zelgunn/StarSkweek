#include "player.h"

Player::Player()
{
    m_position.x = 0;
    m_position.y = 0;
    m_speed = 0.005;
    m_tileType = Tile::Player1Tile;
}
Tile::TileType Player::tileType() const
{
    return m_tileType;
}

void Player::setTileType(Tile::TileType tileType)
{
    m_tileType = tileType;
}


