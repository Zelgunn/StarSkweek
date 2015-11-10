#include "deathstarbeam.h"


DeathStarBeam::DeathStarBeam(Grid *grid, QSize tileSize, Tile::TileType playerTile)
    :  m_tileSize(tileSize), m_grid(grid), m_pixmapAngle(0), m_playerTileType(playerTile), m_targetLocked(false)
{
    m_targetPixmap = QPixmap(QApplication::applicationDirPath() + "/images/Target.png").scaled(128,128);
    m_position.setX(qrand()%(m_grid->width() * m_tileSize.width()));
    m_position.setY(qrand()%(m_grid->height() * m_tileSize.height()));

    m_target = m_position;
    m_frame = 0;

    // Valide uniquement pour deux joueurs.
    if(m_playerTileType == Tile::Player1Tile)
    {
        m_targetType = Tile::Player2Tile;
    }
    else
    {
        m_targetType = Tile::Player1Tile;
    }
}

QPixmap DeathStarBeam::nextFrame()
{
    QPoint vector;
    if(!targetReached())
    {
        vector = m_target - m_position;

        if(vector.x() < 0) m_position.rx() -= TARGET_SPEED;
        if(vector.x() > 0) m_position.rx() += TARGET_SPEED;
        if(vector.y() < 0) m_position.ry() -= TARGET_SPEED;
        if(vector.y() > 0) m_position.ry() += TARGET_SPEED;

        if(qAbs(vector.x()) <= TARGET_SPEED) m_position.rx() = m_target.x();
        if(qAbs(vector.y()) <= TARGET_SPEED) m_position.ry() = m_target.y();

        // Anneau extérieur
        QTransform transform, transformInsideHalo;
        transform.rotate(m_pixmapAngle++);

        QPixmap animatedTarget = m_targetPixmap.transformed(transform, Qt::SmoothTransformation);

        // Anneau central
        QPainter painter(&animatedTarget);
        transformInsideHalo.rotate(-m_pixmapAngle);

        QRect insideHalo(QPoint(0,0), animatedTarget.size()*2/3);
        insideHalo.moveCenter(animatedTarget.rect().center());

        painter.drawPixmap(insideHalo, m_targetPixmap.transformed(transformInsideHalo, Qt::SmoothTransformation));

        // Point central
        painter.setBrush(QBrush(Qt::red));
        insideHalo = QRect(0,0, 32,32);
        insideHalo.moveCenter(animatedTarget.rect().center());
        painter.drawEllipse(insideHalo);

        return animatedTarget;
    }

    if(!targetValid())
    {
        int selectionMode = qrand()%3;
        bool selectionValid = false;
        do
        {
            m_target.setX(qrand()%(m_grid->width() * m_tileSize.width()));
            m_target.setY(qrand()%(m_grid->height() * m_tileSize.height()));

            if(selectionMode == 0)
            {
                selectionValid = (m_grid->tileAt(targetTile()) == m_targetType);
            }
            else
            {
                vector = m_target - m_position;
                vector.rx() /= m_tileSize.width();
                vector.ry() /= m_tileSize.height();
                selectionValid = (vector.manhattanLength() > 5);
            }
        } while(!selectionValid);

        m_target = targetTile();
        m_target.rx() *= m_tileSize.width();
        m_target.rx() += m_tileSize.width()/2;
        m_target.ry() *= m_tileSize.height();
        m_target.ry() += m_tileSize.height()/2;

        return m_targetPixmap;
    }
    else
    {
        m_targetLocked = true;

        int size = qMin(m_tileSize.width(), m_tileSize.height()) * 3 ;
        int radius = size * m_frame / 30;

        QPixmap pixmap(size, size);
        pixmap.fill(QColor(0,0,0,0));
        QPainter painter(&pixmap);
        QRadialGradient gradient(QPoint(size/2, size/2), radius);
        gradient.setColorAt(0, QColor(0,255,0,255));
        gradient.setColorAt(1, QColor(0,0,0,0));
        painter.setBrush(QBrush(gradient));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(pixmap.rect());

        switch (m_frame) {
        case 20:
            turnInto(targetTile());
            break;
        case 40:
            turnInto(targetTile() + QPoint(-1,0));
            turnInto(targetTile() + QPoint(1,0));
            turnInto(targetTile() + QPoint(0,-1));
            turnInto(targetTile() + QPoint(0,1));
            break;
        case 60:
            turnInto(targetTile() + QPoint(-1,-1));
            turnInto(targetTile() + QPoint(1,1));
            turnInto(targetTile() + QPoint(1,-1));
            turnInto(targetTile() + QPoint(-1,1));
            break;
        }
        m_frame++;

        return pixmap;
    }
}

bool DeathStarBeam::animationDone() const
{
    if(!targetReached())
    {
        return false;
    }
    if(!targetValid())
    {
        return false;
    }

    return (m_frame > 60);
}

bool DeathStarBeam::targetReached() const
{
    return ((m_target == m_position) || m_targetLocked);
}

bool DeathStarBeam::targetValid() const
{
    return ((m_grid->tileAt(targetTile()) == m_targetType) || m_targetLocked);
}

QPoint DeathStarBeam::targetTile() const
{
    QPoint res(m_target.x() / m_tileSize.width(), m_target.y() / m_tileSize.height());
    return res;
}

void DeathStarBeam::turnInto(const QPoint &point)
{
    Tile::TileType selectedTileType = m_grid->tileAt(point);
    if(selectedTileType == m_targetType)
    {
        m_grid->setTileAt(point, m_playerTileType);
    }
}
QPoint DeathStarBeam::target() const
{
    return m_target;
}

void DeathStarBeam::setTarget(const QPoint &target)
{
    m_target = target;
}

