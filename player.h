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
    Player *clone() const;

    Tile::TileType tileType() const;
    void setTileType(Tile::TileType tileType);

    Weapon weapon() const;
    void setWeapon(const Weapon &weapon);

    QPixmap portrait() const;
    void setPortrait(const QPixmap &portrait);
    QPixmap thumbnail() const;
    void setThumbnail(const QPixmap &thumbnail);

    Directions previousDirection() const;
    void setPreviousDirection(const Directions &previousDirection);
    void setDirection(const Directions &direction);

    int fire();

    bool isPlayer() const;

private:
    Tile::TileType m_tileType;
    Directions m_previousDirection;
    Weapon m_weapon;
    QPixmap m_portrait;
    QPixmap m_thumbnail;
};

#endif // PLAYER_H
