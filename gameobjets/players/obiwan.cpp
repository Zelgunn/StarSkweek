#include "obiwan.h"

ObiWan::ObiWan(const QDomElement &element)
    : Player(element), m_inGhostForm(false), m_ghostFormTimer(QTime::currentTime())
{
    QString dir = QApplication::applicationDirPath() + "/images/Obiwan_Ghost_";

    m_ghostModels[Right] = QPixmap(dir + "Right.png").scaledToHeight(250, Qt::SmoothTransformation);
    m_ghostModels[Up] = QPixmap(dir + "Up.png").scaledToHeight(250, Qt::SmoothTransformation);
    m_ghostModels[Left] = QPixmap(dir + "Left.png").scaledToHeight(250, Qt::SmoothTransformation);
    m_ghostModels[Down] = QPixmap(dir + "Down.png").scaledToHeight(250, Qt::SmoothTransformation);
}

bool ObiWan::isObiWan() const
{
    return true;
}

const QPixmap *ObiWan::model() const
{
    if(m_inGhostForm)
    {
        if(m_direction == InvalidDirection)
            return &m_ghostModels[Down];
        return &m_ghostModels[m_direction];
    }
    else
    {
        return Player::model();
    }
}

bool ObiWan::inGhostForm()
{
    if(ghostFormTimeLeft() <= 0)
        m_inGhostForm = false;
    return m_inGhostForm;
}

int ObiWan::ghostFormTimeLeft() const
{
    return qMax((4000 - m_ghostFormTimer.msecsTo(QTime::currentTime())), 0);
}

qreal ObiWan::speed() const
{
    if(m_inGhostForm)
    {
        return Player::speed() * 1.5;
    }
    return Player::speed();
}

void ObiWan::takeDamage(int damage)
{
    if(inGhostForm())
        Unit::takeDamage(damage/2);
    else
        Unit::takeDamage(damage);
}

bool ObiWan::usePower(bool ignoreRessources)
{
    if(!ignoreRessources)
    {
        if(m_powerRessource < MaxPowerRessource*2/5)
        {
            return false;
        }
    }

    m_inGhostForm = true;
    m_ghostFormTimer = QTime::currentTime();

    if(!ignoreRessources)
        m_powerRessource -= MaxPowerRessource*2/5;

    return true;
}

bool ObiWan::isObiWanAndGhost(Player *player)
{
    if(player->isObiWan())
    {
        ObiWan *obiWan = (ObiWan*) player;
        if(obiWan->inGhostForm())
            return true;
    }
    return false;
}
