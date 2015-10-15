#include "weapon.h"

Weapon::Weapon()
{
    m_reloadTime = 500;
    m_lastUse = QTime::currentTime();
}

Projectile Weapon::ammunition() const
{
    return m_ammunition;
}

void Weapon::setAmmunition(const Projectile &ammunition)
{
    m_ammunition = ammunition;
}

bool Weapon::fire(Projectile *projectile)
{
    if(m_lastUse.msecsTo(QTime::currentTime()) >= m_reloadTime)
    {
        *projectile = m_ammunition;
        m_lastUse = QTime::currentTime();
        return true;
    }
    return false;
}


