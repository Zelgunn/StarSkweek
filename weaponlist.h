#ifndef WEAPONLIST_H
#define WEAPONLIST_H

#include "weapon.h"

class WeaponList
{
public:
    WeaponList();
    WeaponList(const QDomElement &element);

    Weapon at(int i) const;

private:
    QList<Weapon> m_weapons; /** @brief Liste des prototypes de projectiles instanciables. */
};

#endif // WEAPONLIST_H
