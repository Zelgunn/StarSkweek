#include "projectile.h"

Projectile::Projectile()
{
    m_ttl = 120;
    m_speed.append(10);
}

Projectile::Projectile(const QDomElement &element)
{
    QDomNode node = element.firstChild();
    QDomElement elem;
    QString dir = QApplication::applicationDirPath();

    m_type = element.attribute("type").toLatin1().at(0) - 'a';

    int modelWidth = 1, modelHeight = 1;

    while(!node.isNull())
    {
        elem = node.toElement();

        if(elem.tagName() == "Spec")
        {
            m_ttl = elem.attribute("ttl").toInt();
            m_damage = elem.attribute("damage").toInt();
        }

        if(elem.tagName() == "ModelSize")
        {
            modelWidth = elem.attribute("width").toInt();
            modelHeight = elem.attribute("height").toInt();
        }

        if(elem.tagName() == "Model")
        {
            m_models[Right] = QPixmap(dir + elem.attribute("right"));
            m_models[Up] = QPixmap(dir + elem.attribute("up"));
            m_models[Left] = QPixmap(dir + elem.attribute("left"));
            m_models[Down] = QPixmap(dir + elem.attribute("down"));
        }

        if(elem.tagName() == "Speed")
        {
            m_speed.append(elem.attribute("value").toDouble());
        }

        if(elem.tagName() == "Angle")
        {
            m_angle.append(qDegreesToRadians(elem.attribute("value").toDouble()));
        }

        node = node.nextSibling();
    }

    m_models[Right] = m_models[Right].scaled(modelWidth, modelHeight);
    m_models[Up] = m_models[Up].scaled(modelHeight, modelWidth);
    m_models[Left] = m_models[Left].scaled(modelWidth, modelHeight);
    m_models[Down] = m_models[Down].scaled(modelHeight, modelWidth);
}

Projectile::~Projectile()
{

}

void Projectile::move()
{
    Point delta;
    if(m_angle.size()>0)
    {
        delta = displacement(m_direction, m_speed.first(), m_angle.first());
    }
    else
    {
        delta = displacement(m_direction, m_speed.first(), 0);
    }

    m_position.x += delta.x;
    m_position.y += delta.y;
    m_ttl --;
    for(int i=1; i<m_speed.size(); i++)
    {
        m_speed.replace(i-1, m_speed.at(i) + m_speed.at(i - 1));
    }

    for(int i=1; i<m_angle.size(); i++)
    {
        m_angle.replace(i-1, m_angle.at(i) + m_angle.at(i - 1));
    }
}

const QPixmap *Projectile::model() const
{
    return &m_models[m_direction];
}

int Projectile::ttl() const
{
    return m_ttl;
}

int Projectile::type() const
{
    return m_type;
}

void Projectile::setType(int type)
{
    m_type = type;
}

bool Projectile::isProjectile() const
{
    return true;
}
int Projectile::damage() const
{
    return m_damage;
}

void Projectile::setDamage(int damage)
{
    m_damage = damage;
}

Point Projectile::displacement(GameObject::Directions direction, qreal speed, qreal angle)
{
    Point res;
    qreal angleX = qCos(angle);
    qreal angleY = qSin(angle);

    switch (direction) {
    case Right:
        res.x = (int)speed * angleX;
        res.y = (int)speed * angleY;
        break;
    case Up:
        res.x = - (int)speed * angleY;
        res.y = - (int)speed * angleX;
        break;
    case Left:
        res.x = - (int)speed * angleX;
        res.y = - (int)speed * angleY;
        break;
    case Down:
        res.x = (int)speed * angleY;
        res.y = (int)speed * angleX;
        break;
    default:
        res.x = 0;
        res.y = 0;
    }

    return res;
}




