#ifndef PLAYER_H
#define PLAYER_H

#include <QDomElement>

#include "unit.h"
#include "weapon.h"

class Player : public Unit
{
public:
    enum Powers{
        ObiWanPower = 0,
        DarthVaderPower = 1,
        MaxPowerRessource = 102400
    };

    Player();
    Player(const QDomElement &element);
    virtual ~Player();

    virtual bool isObiWan() const;
    virtual bool isDarthVader() const;

    Tile::TileType tileType() const;
    void setTileType(Tile::TileType tileType);
    Weapon weapon() const;
    void setWeapon(const Weapon &weapon);
    QPixmap portrait() const;
    void setPortrait(const QPixmap &portrait);
    QPixmap thumbnail() const;
    void setThumbnail(const QPixmap &thumbnail);
    Powers power() const;
    void setPower(const Powers &power);

    Directions previousDirection() const;
    void setPreviousDirection(const Directions &previousDirection);
    void setDirection(const Directions &direction);

    int powerRessource() const;
    void setPowerRessource(int powerRessource);
    int powerRegeneration() const;
    void setPowerRegeneration(int powerRegeneration);
    void regeneratePower();
    void increasePowerRessource(int increase);
    bool powerAvailable() const;
    qreal powerRessourceRatio() const;
    virtual bool usePower(bool ignoreRessources = false);

    int fire();
    void takeDamage(int damage);

    bool isPlayer() const;

    static int maxPowerRessource();

protected:
    Tile::TileType m_tileType;
    Directions m_previousDirection;
    Weapon m_weapon;
    QPixmap m_portrait;
    QPixmap m_thumbnail;
    Powers m_power;
    int m_powerRessource;
    int m_powerRegeneration;
};

#endif // PLAYER_H
