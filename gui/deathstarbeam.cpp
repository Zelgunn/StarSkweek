#include "deathstarbeam.h"


DeathStarBeam::DeathStarBeam(const Grid *grid, QSize tileSize, int player)
    :  m_tileSize(tileSize), m_grid(grid), m_player(player), m_targetLocked(false)
{
    m_targetPixmap = QPixmap(QApplication::applicationDirPath() + "/images/Portrait_Random.png").scaled(128,128);
    m_position.setX(qrand()%(m_grid->width() * m_tileSize.width()));
    m_position.setY(qrand()%(m_grid->height() * m_tileSize.height()));

    m_target = m_position;
    m_frame = 0;
    //m_grid->tileAt(m_position.x()/m_tileSize.width(), m_position.y()/m_tileSize.height());
}

QPixmap DeathStarBeam::nextFrame()
{
    QPoint vector;
    if(!targetReached())
    {
        vector = m_target - m_position;

        if(vector.x() < 0) m_position.rx()--;
        if(vector.x() > 0) m_position.rx()++;
        if(vector.y() < 0) m_position.ry()--;
        if(vector.y() > 0) m_position.ry()++;

        return m_targetPixmap;
    }

    if(!targetValid())
    {
        do
        {
            m_target.setX(qrand()%(m_grid->width() * m_tileSize.width()));
            m_target.setY(qrand()%(m_grid->height() * m_tileSize.height()));

            vector = m_target - m_position;
            vector.rx() /= m_tileSize.width();
            vector.ry() /= m_tileSize.height();
        } while(vector.manhattanLength() > 5);

        return m_targetPixmap;
    }
    else
    {
        m_targetLocked = true;
        m_position.setY(0);

        QPixmap pixmap(m_frame/2, m_target.y());
        pixmap.fill(QColor(0,255,0,255));

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

    return m_frame > 60;
}

bool DeathStarBeam::targetReached() const
{
    return ((m_target == m_position) || m_targetLocked);
}

bool DeathStarBeam::targetValid() const
{
    return ((m_grid->tileAt(m_target.x() / m_tileSize.width(), m_target.y() / m_tileSize.height()) == Tile::Player2Tile) || m_targetLocked);
}
