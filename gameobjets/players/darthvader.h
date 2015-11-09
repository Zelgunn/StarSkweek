#ifndef DARTHVADER_H
#define DARTHVADER_H

#include "gameobjets/player.h"

class DarthVader : public Player
{
    Q_OBJECT
public:
    DarthVader(const QDomElement &element);

    virtual bool isDarthVader() const;
    virtual void usePower();
    void takeDamage(int damage);

signals:
    void blackStarBeamActivated();
};

#endif // DARTHVADER_H
