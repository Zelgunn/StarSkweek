#include "weapon.h"

Weapon::Weapon()
{
    m_reloadTime = 500;
    m_lastUse = QTime::currentTime();
    m_ammunitionType = 0;
}

int Weapon::ammunition() const
{
    return m_ammunitionType;
}

void Weapon::setAmmunition(int ammunitionType)
{
    m_ammunitionType = ammunitionType;
}

int Weapon::fire()
{
    int type = -1;
    if(m_lastUse.msecsTo(QTime::currentTime()) >= m_reloadTime)
    {
        m_lastUse = QTime::currentTime();
        type =  m_ammunitionType;
    }

    return type;
}
