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
        return;
    if(blackStarActive())
        return;

    m_blackStarBeam = new DeathStarBeam(m_grid, m_tileSize, m_tileType);

    m_powerRessource -= MaxPowerRessource;
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

