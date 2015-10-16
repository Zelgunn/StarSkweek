#include "player.h"

Player::Player()
{
    m_position.x = 0;
    m_position.y = 0;
    m_speed = 0.0025;
    m_tileType = Tile::Player1Tile;
    m_direction = InvalidDirection;
    m_previousDirection = Down;
}

Player::Player(const QPixmap &model, Tile::TileType tileType) :
    m_tileType(tileType), m_previousDirection(Down)
{
    m_position.x = 0;
    m_position.y = 0;
    m_speed = 0.0025;
    m_model = model;
    m_direction = InvalidDirection;
}

Tile::TileType Player::tileType() const
{
    return m_tileType;
}

void Player::setTileType(Tile::TileType tileType)
{
    m_tileType = tileType;
}

Weapon Player::weapon() const
{
    return m_weapon;
}

void Player::setWeapon(const Weapon &weapon)
{
    m_weapon = weapon;
}

GameObject::Directions Player::previousDirection() const
{
    return m_previousDirection;
}

void Player::setPreviousDirection(const Directions &previousDirection)
{
    m_previousDirection = previousDirection;
}

void Player::setDirection(const GameObject::Directions &direction)
{
    if(direction != InvalidDirection) m_previousDirection = direction;
    m_direction = direction;
}

bool Player::fire(Projectile *projectile)
{
    return m_weapon.fire(projectile);
}




