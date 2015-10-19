#include "weaponlist.h"

WeaponList::WeaponList()
{

}

WeaponList::WeaponList(const QDomElement &element)
{
    QDomNode node = element.firstChild();

    while(!node.isNull())
    {
        m_weapons.append(Weapon(node.toElement()));

        node = node.nextSibling();
    }
}

Weapon WeaponList::at(int i) const
{
    return m_weapons.at(i);
}

