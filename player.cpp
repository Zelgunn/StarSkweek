#include "player.h"

Player::Player()
{
    m_position.x = 0;
    m_position.y = 0;
    m_speed.append(4);
    m_tileType = Tile::Player1Tile;
    m_direction = InvalidDirection;
    m_previousDirection = Down;
}

Player::Player(const QDomElement &element) :
    m_previousDirection(Down)
{
    QDomNode node = element.firstChild();
    QDomElement elem;
    QString dir = QApplication::applicationDirPath();

    while(!node.isNull())
    {
        elem = node.toElement();

        if(elem.tagName() == "Model")
        {
            m_models[Right] = QPixmap(dir + elem.attribute("right"));
            m_models[Up] = QPixmap(dir + elem.attribute("up"));
            m_models[Left] = QPixmap(dir + elem.attribute("left"));
            m_models[Down] = QPixmap(dir + elem.attribute("down"));
        }

        if(elem.tagName() == "Tile")
        {
            m_tileType = (Tile::TileType)(elem.attribute("type").toLatin1().at(0) - 'a');
        }

        if(elem.tagName() == "Portrait")
        {
            m_portrait = QPixmap(dir + elem.attribute("complete"));
            m_thumbnail = QPixmap(dir + elem.attribute("thumbnail")).scaled(256,256);
        }

        node = node.nextSibling();
    }

    m_position.x = 0;
    m_position.y = 0;
    m_speed.append(4);
    m_direction = InvalidDirection;
}

Player::~Player()
{

}

Tile::TileType Player::tileType() const
{
    return m_tileType;
}

void Player::setTileType(Tile::TileType tileType)
{
    m_tileType = tileType;
}

Weapon Player::weapon() const
{
    return m_weapon;
}

void Player::setWeapon(const Weapon &weapon)
{
    m_weapon = weapon;
}

QPixmap Player::portrait() const
{
    return m_portrait;
}
QPixmap Player::thumbnail() const
{
    return m_thumbnail;
}

GameObject::Directions Player::previousDirection() const
{
    return m_previousDirection;
}

void Player::setPreviousDirection(const Directions &previousDirection)
{
    m_previousDirection = previousDirection;
}

void Player::setDirection(const GameObject::Directions &direction)
{
    if(direction != InvalidDirection) m_previousDirection = direction;
    m_direction = direction;
}

int Player::fire()
{
    if(m_dead) return -1;
    return m_weapon.fire();
}

bool Player::isPlayer() const
{
    return true;
}

