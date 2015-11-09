#ifndef DARTHVADER_H
#define DARTHVADER_H

#include "gameobjets/player.h"
#include "animations/deathstarbeam.h"

class DarthVader : public Player
{
public:
    DarthVader(const QDomElement &element);

    virtual bool isDarthVader() const;
    virtual bool usePower(bool ignoreRessources = false);
    void takeDamage(int damage);
    bool blackStarActive() const;
    DeathStarBeam *blackStarBeam() const;
    void deleteBlackStar();

    QSize tileSize() const;
    void setTileSize(const QSize &tileSize);

private:
    DeathStarBeam *m_blackStarBeam;
    QSize m_tileSize;
};

#endif // DARTHVADER_H
