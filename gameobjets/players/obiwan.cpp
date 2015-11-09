#include "obiwan.h"

ObiWan::ObiWan(const QDomElement &element)
    : Player(element), m_inGhostForm(false), m_ghostFormTimer(QTime::currentTime())
{

}

bool ObiWan::isObiWan() const
{
    return true;
}

bool ObiWan::inGhostForm()
{
    if(ghostFormTimeLeft() <= 0)
        m_inGhostForm = false;
    return m_inGhostForm;
}

int ObiWan::ghostFormTimeLeft() const
{
    return qMax((2000 - m_ghostFormTimer.msecsTo(QTime::currentTime())), 0);
}

void ObiWan::takeDamage(int damage)
{
    if(inGhostForm())
        Unit::takeDamage(damage/2);
    else
        Unit::takeDamage(damage);
}

void ObiWan::usePower()
{
    if(m_powerRessource < MaxPowerRessource/5)
    {
        return;
    }

    m_inGhostForm = true;
    m_ghostFormTimer = QTime::currentTime();
    m_powerRessource -= MaxPowerRessource/5;
}

bool ObiWan::isObiWanAndGhost(Player *player)
{
    if(player->isObiWan())
    {
        ObiWan *obiWan = (ObiWan*) player;
        if(obiWan->inGhostForm())
            return true;
    }
    return false;
}