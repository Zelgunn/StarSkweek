#include "deathstarbeam.h"


DeathStarBeam::DeathStarBeam(Grid *grid, QSize tileSize, int player)
    : m_grid(grid), m_tileSize(tileSize), m_player(player)
{
    m_position.setX(qrand()%(m_grid->width() * m_tileSize.width()));
    m_position.setY(qrand()%(m_grid->height() * m_tileSize.height()));

    m_target = m_position;
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
        }
        while(vector.manhattanLength() > 5);

        return m_targetPixmap;
    }
    else
    {
        QPixmap pixmap;
        QPainter painter(&pixmap);
        QPen pen(QColor(0,255,0));
        pen.setWidth(m_frame/2);

        painter.drawLine(QPoint(m_target.x(), 0), m_target);

        m_frame++;
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

    return (m_frame >= 60);
}

bool DeathStarBeam::targetReached() const
{
    return (m_target == m_position);

}

bool DeathStarBeam::targetValid() const
{
    return false;
}
