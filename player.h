#ifndef PLAYER_H
#define PLAYER_H

#include <QDomElement>

#include "unit.h"
#include "weapon.h"

class Player : public Unit
{
public:
    Player();
    Player(const QDomElement &element);
    virtual ~Player();

    Tile::TileType tileType() const;
    void setTileType(Tile::TileType tileType);

    Weapon weapon() const;
    void setWeapon(const Weapon &weapon);

    Directions previousDirection() const;
    void setPreviousDirection(const Directions &previousDirection);
    void setDirection(const Directions &direction);

    int fire();

    bool isPlayer() const;

private:
    Tile::TileType m_tileType;
    Directions m_previousDirection;
    Weapon m_weapon;
};

#endif // PLAYER_H
