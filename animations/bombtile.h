#ifndef BOMBTILE_H
#define BOMBTILE_H

#include "animations/animation.h"
#include "grid.h"

class Bombtile : public Animation
{
public:
    Bombtile(Grid* grid, QPoint position, QSize tileSize);

    virtual QPixmap nextFrame();
    virtual bool animationDone() const;

    QPoint targetTile() const;
    void explodeTile(const QPoint &tile);

private:
    Grid *m_grid;
    QSize m_tileSize;
    QList<Bombtile *> m_triggeredBombs;
};

#endif // BOMBTILE_H
