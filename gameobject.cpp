#include "gameobject.h"

GameObject::GameObject()
{
    m_position.x = 0;
    m_position.y = 0;
}

const QImage *GameObject::appearance() const
{
    return &m_appearance;
}

void GameObject::setAppearance(const QImage &image)
{
    m_appearance = image;
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
        res.y = -speed;
        break;
    case Left:
        res.x = -speed;
        res.y = 0;
        break;
    case Down:
        res.x = 0;
        res.y = speed;
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
