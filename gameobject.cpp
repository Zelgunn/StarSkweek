#include "gameobject.h"

GameObject::GameObject()
    :  m_position(QPoint(0,0)), m_upstairs(false)
{

}

const QPixmap *GameObject::model() const
{
    if(m_direction == InvalidDirection)
        return &m_models[Down];
    return &m_models[m_direction];
}

void GameObject::setModel(const QPixmap &model, Directions direction)
{
    m_models[direction] = model;
}

QPoint GameObject::position() const
{
    return m_position;
}

void GameObject::setPosition(const QPoint &position)
{
    m_position = position;
}

void GameObject::setPosition(int x, int y)
{
    m_position = QPoint(x, y);
}

void GameObject::setSpeed(qreal speed)
{
    if(m_speed.size() < 1) m_speed.append(speed);
    else m_speed.replace(0, speed);
}

void GameObject::setSpeed(const QList<qreal> &speed)
{
    m_speed = speed;
}

qreal GameObject::speed() const
{
    return m_speed.first();
}

GameObject::Directions GameObject::direction() const
{
    return m_direction;
}

void GameObject::setDirection(const Directions &direction)
{
    m_direction = direction;
}

int GameObject::faction() const
{
    return m_faction;
}

void GameObject::setFaction(int faction)
{
    m_faction = faction;
}

bool GameObject::isUpstairs() const
{
    return m_upstairs;
}

void GameObject::setUpstairs(bool upstairs)
{
    m_upstairs = upstairs;
}

Grid *GameObject::grid() const
{
    return m_grid;
}

void GameObject::setGrid(Grid *grid)
{
    m_grid = grid;
}

QString GameObject::name() const
{
    return m_name;
}

void GameObject::setName(const QString &name)
{
    m_name = name;
}

bool GameObject::isUnit() const
{
    return false;
}

bool GameObject::isPlayer() const
{
    return false;
}

bool GameObject::isMonster() const
{
    return false;
}

bool GameObject::isProjectile() const
{
    return false;
}

QPoint GameObject::displacement(GameObject::Directions direction, qreal speed)
{
    QPoint res(0,0);
    switch (direction) {
    case Right:
        res.rx() = (int)speed;
        break;
    case Up:
        res.ry() = - (int)speed;
        break;
    case Left:
        res.rx() = - (int)speed;
        break;
    case Down:
        res.ry() = (int)speed;
        break;
    case InvalidDirection:
        break;
    }

    return res;
}

double GameObject::euclidianDistance(const QPoint &p1, const QPoint &p2)
{
    return qSqrt(qPow((p1.x() - p2.x()) ,2) + qPow(p1.y() - p2.y() ,2));
}
