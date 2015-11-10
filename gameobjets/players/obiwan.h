#ifndef OBIWAN_H
#define OBIWAN_H

#include "gameobjets/player.h"

class ObiWan : public Player
{
public:
    ObiWan(const QDomElement &element);

    virtual bool isObiWan() const;
    virtual const QPixmap *model() const;

    bool inGhostForm();
    int ghostFormTimeLeft() const;

    virtual qreal speed() const;

    void takeDamage(int damage);

    virtual bool usePower(bool ignoreRessources = false);

    static bool isObiWanAndGhost(Player *player);

private:
    bool m_inGhostForm;
    QTime m_ghostFormTimer;
    QPixmap m_ghostModels[4];
};

#endif // OBIWAN_H
