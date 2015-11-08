#include "bombtile.h"

Bombtile::Bombtile(Grid *grid, QPoint position, QSize tileSize)
    : Animation(position), m_grid(grid), m_tileSize(tileSize)
{

}

QPixmap Bombtile::nextFrame()
{
    QSize frameSize = m_tileSize*3;
    QPixmap frame(frameSize);
    if(m_frame > 240)
    {
        frame.fill(Qt::transparent);
    }
    else
    {
        frame.fill(QColor(50,50,50,255*m_frame/240));
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

    foreach(Bombtile *bomb, m_triggeredBombs)
        bomb->nextFrame();

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
