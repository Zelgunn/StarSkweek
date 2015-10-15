#ifndef WEAPON_H
#define WEAPON_H

#include <QTime>
#include "projectile.h"

class Weapon
{
public:
    Weapon();

    Projectile ammunition() const;
    void setAmmunition(const Projectile &ammunition);
    bool fire(Projectile *projectile);

private:
    Projectile m_ammunition;
    double m_reloadTime;
    QTime m_lastUse;

};

#endif // WEAPON_H
