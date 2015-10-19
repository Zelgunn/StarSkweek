#include "projectile.h"

Projectile::Projectile()
{
    m_ttl = 120;
    m_speed = 0.02;

    QString dir = QApplication::applicationDirPath();
    m_models[Right] = QPixmap(dir + "/images/laser_right.png").scaledToHeight(15);
    m_models[Up] = QPixmap(dir + "/images/laser_up.png").scaledToWidth(15);
    m_models[Left] = QPixmap(dir + "/images/laser_left.png").scaledToHeight(15);
    m_models[Down] = QPixmap(dir + "/images/laser_down.png").scaledToWidth(15);
}

Projectile::Projectile(GameObject::Directions direction, int faction)
{
    m_ttl = 120;
    m_speed = 0.002;

    QString dir = QApplication::applicationDirPath();
    m_models[Right] = QPixmap(dir + "/images/laser_right.png").scaledToHeight(15);
    m_models[Up] = QPixmap(dir + "/images/laser_up.png").scaledToWidth(15);
    m_models[Left] = QPixmap(dir + "/images/laser_left.png").scaledToHeight(15);
    m_models[Down] = QPixmap(dir + "/images/laser_down.png").scaledToWidth(15);

    m_direction = direction;
    m_faction = faction;
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
            m_speed = elem.attribute("speed").toDouble();
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

void Projectile::move(double ratio)
{
    Point delta = displacement(m_direction, m_speed, ratio);
    m_position.x += delta.x;
    m_position.y += delta.y;
    m_ttl --;
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



