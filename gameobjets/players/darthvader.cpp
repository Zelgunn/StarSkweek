#include "darthvader.h"

DarthVader::DarthVader(const QDomElement &element)
    : Player(element)
{

}

bool DarthVader::isDarthVader() const
{
    return true;
}

void DarthVader::usePower()
{
    if(m_powerRessource < MaxPowerRessource)
    {
        return;
    }

    m_powerRessource -= MaxPowerRessource;
    emit blackStarBeamActivated();
}

void DarthVader::takeDamage(int damage)
{
    Player::takeDamage(damage);
}
