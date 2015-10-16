#include "gameobject.h"

GameObject::GameObject()
{
    m_position.x = 0;
    m_position.y = 0;
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

Point GameObject::position() const
{
    return m_position;
}

void GameObject::setPosition(const Point &position)
{
    m_position = position;
}

void GameObject::setPosition(double x, double y)
{
    m_position.x = x;
    m_position.y = y;
}

double GameObject::speed() const
{
    return m_speed;
}

GameObject::Directions GameObject::direction() const
{
    return m_direction;
}

void GameObject::setDirection(const Directions &direction)
{
    m_direction = direction;
}


void GameObject::setSpeed(double speed)
{
    m_speed = speed;
}

Point GameObject::displacement(GameObject::Directions direction, double speed)
{
    Point res;
    switch (direction) {
    case Right:
        res.x = speed;
        res.y = 0;
        break;
    case Up:
        res.x = 0;
        res.y = -speed*1.61;
        break;
    case Left:
        res.x = -speed;
        res.y = 0;
        break;
    case Down:
        res.x = 0;
        res.y = speed*1.61;
        break;
    default:
        res.x = 0;
        res.y = 0;
    }

    return res;
}

double GameObject::euclidianDistance(const Point &p1, const Point &p2)
{
    return qSqrt(qPow((p1.x - p2.x) ,2) + qPow(p1.y - p2.y ,2));
}
