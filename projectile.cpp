#include "projectile.h"

Projectile::Projectile()
{
    m_ttl = 120;
    m_speed = 0.02;

    QString dir = QApplication::applicationDirPath();
    m_models[Right] = QImage(dir + "/images/laser_right.png").scaledToHeight(15);
    m_models[Up] = QImage(dir + "/images/laser_up.png").scaledToWidth(15);
    m_models[Left] = QImage(dir + "/images/laser_left.png").scaledToHeight(15);
    m_models[Down] = QImage(dir + "/images/laser_down.png").scaledToWidth(15);
}

Projectile::Projectile(GameObject::Directions direction, int ownerID)
{
    m_ttl = 120;
    m_speed = 0.02;

    QString dir = QApplication::applicationDirPath();
    m_models[Right] = QImage(dir + "/images/laser_right.png").scaledToHeight(15);
    m_models[Up] = QImage(dir + "/images/laser_up.png").scaledToWidth(15);
    m_models[Left] = QImage(dir + "/images/laser_left.png").scaledToHeight(15);
    m_models[Down] = QImage(dir + "/images/laser_down.png").scaledToWidth(15);

    m_direction = direction;
    m_ownerID = ownerID;
}

void Projectile::move()
{
    Point delta = displacement(m_direction, m_speed);
    m_position.x += delta.x;
    m_position.y += delta.y;
    m_ttl --;
}

const QImage *Projectile::appearance() const
{
    return &m_models[m_direction];
}

int Projectile::ttl() const
{
    return m_ttl;
}
int Projectile::ownerID() const
{
    return m_ownerID;
}

void Projectile::setOwnerID(int ownerID)
{
    m_ownerID = ownerID;
}


