#include "bombtile.h"

Bombtile::Bombtile(Grid *grid, QPoint position, QSize tileSize)
    : Animation(position), m_grid(grid), m_tileSize(tileSize)
{

}

QPixmap Bombtile::nextFrame()
{
    int size = qMin(m_tileSize.width(), m_tileSize.height()) * 3 ;
    int radius = size * m_frame/360;

    QPixmap frame(size, size);
    frame.fill(Qt::transparent);

    if(m_frame<240)
    {
        QPainter painter(&frame);
        QRadialGradient gradient(QPoint(size/2, size/2), radius);
        gradient.setColorAt(0, QColor(255,0,0,255));
        gradient.setColorAt(1, QColor(0,0,0,0));
        painter.setBrush(QBrush(gradient));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(frame.rect());
        painter.end();
    }

    m_frame++;
    switch (m_frame) {
    case 120:
        explodeTile(targetTile());
        break;
    case 180:
        explodeTile(targetTile() + QPoint(-1,0));
        explodeTile(targetTile() + QPoint(1,0));
        explodeTile(targetTile() + QPoint(0,-1));
        explodeTile(targetTile() + QPoint(0,1));
        break;
    case 240:
        explodeTile(targetTile() + QPoint(-1,-1));
        explodeTile(targetTile() + QPoint(1,1));
        explodeTile(targetTile() + QPoint(1,-1));
        explodeTile(targetTile() + QPoint(-1,1));
        break;
    }

    return frame;
}

bool Bombtile::animationDone() const
{
    foreach(Bombtile *bomb, m_triggeredBombs)
        if(!bomb->animationDone()) return false;

    return (m_frame > 240);
}

QPoint Bombtile::targetTile() const
{
    QPoint res(m_position.x() / m_tileSize.width(), m_position.y() / m_tileSize.height());
    return res;
}

void Bombtile::explodeTile(const QPoint &tile)
{
    Tile::TileType newType;
    QPoint triggeredBombTile;
    switch(m_grid->tileAt(tile))
    {
    case Tile::BombTile:
        triggeredBombTile.setX(tile.x()*m_tileSize.width() + m_tileSize.width()/2);
        triggeredBombTile.setY(tile.y()*m_tileSize.height() + m_tileSize.height()/2);
        m_triggeredBombs.append(new Bombtile(m_grid, triggeredBombTile, m_tileSize));
        newType = Tile::ExplodingTile;
        break;
    default:
        newType = Tile::Void;
        break;
    }

    m_grid->setTileAt(tile, newType);
}
QList<Bombtile *> Bombtile::triggeredBombs() const
{
    return m_triggeredBombs;
}

