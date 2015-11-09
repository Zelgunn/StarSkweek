#include "darthvader.h"

DarthVader::DarthVader(const QDomElement &element)
    : Player(element)
{

}

bool DarthVader::isDarthVader() const
{
    return true;
}

bool DarthVader::usePower(bool ignoreRessources)
{
    if(!ignoreRessources)
    {
        if(m_powerRessource < MaxPowerRessource)
            return false;
        if(blackStarActive())
            return false;
    }

    m_blackStarBeam = new DeathStarBeam(m_grid, m_tileSize, m_tileType);

    if(!ignoreRessources)
        m_powerRessource -= MaxPowerRessource;
    return true;
}

void DarthVader::takeDamage(int damage)
{
    Player::takeDamage(damage);
}

bool DarthVader::blackStarActive() const
{
    return (m_blackStarBeam != Q_NULLPTR);
}

DeathStarBeam *DarthVader::blackStarBeam() const
{
    return m_blackStarBeam;
}

void DarthVader::deleteBlackStar()
{
    delete m_blackStarBeam;
    m_blackStarBeam = Q_NULLPTR;
}

QSize DarthVader::tileSize() const
{
    return m_tileSize;
}

void DarthVader::setTileSize(const QSize &tileSize)
{
    m_tileSize = tileSize;
}

