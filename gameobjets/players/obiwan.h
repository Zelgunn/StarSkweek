#ifndef OBIWAN_H
#define OBIWAN_H

#include "gameobjets/player.h"

class ObiWan : public Player
{
public:
    ObiWan(const QDomElement &element);

    virtual bool isObiWan() const;

    bool inGhostForm();
    int ghostFormTimeLeft() const;

    void takeDamage(int damage);

    virtual void usePower();

    static bool isObiWanAndGhost(Player *player);

private:
    bool m_inGhostForm;
    QTime m_ghostFormTimer;
};

#endif // OBIWAN_H
