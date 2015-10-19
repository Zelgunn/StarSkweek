#ifndef WEAPON_H
#define WEAPON_H

#include <QTime>
#include "projectile.h"

class Weapon
{
public:
    Weapon();

    int ammunition() const;
    void setAmmunition(int ammunitionType);
    int fire();

private:
    int m_ammunitionType;
    double m_reloadTime;
    QTime m_lastUse;

};

#endif // WEAPON_H
