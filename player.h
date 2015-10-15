#ifndef PLAYER_H
#define PLAYER_H

#include "unit.h"
#include "weapon.h"

class Player : public Unit
{
public:
    Player();

    Tile::TileType tileType() const;
    void setTileType(Tile::TileType tileType);

    Weapon weapon() const;
    void setWeapon(const Weapon &weapon);

    Directions previousDirection() const;
    void setPreviousDirection(const Directions &previousDirection);
    void setDirection(const Directions &direction);

    bool fire(Projectile *projectile);

private:
    Tile::TileType m_tileType;
    Directions m_previousDirection;
    Weapon m_weapon;
};

#endif // PLAYER_H
