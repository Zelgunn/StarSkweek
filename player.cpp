#include "player.h"

Player::Player()
{
    m_position = QPoint(0,0);
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
            m_models[Right] = QPixmap(dir + elem.attribute("right")).scaledToHeight(250, Qt::SmoothTransformation);
            m_models[Up] = QPixmap(dir + elem.attribute("up")).scaledToHeight(250, Qt::SmoothTransformation);
            m_models[Left] = QPixmap(dir + elem.attribute("left")).scaledToHeight(250, Qt::SmoothTransformation);
            m_models[Down] = QPixmap(dir + elem.attribute("down")).scaledToHeight(250, Qt::SmoothTransformation);
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

    m_position = QPoint(0,0);
    m_speed.append(4);
    m_direction = InvalidDirection;
}

Player::~Player()
{

}

Player *Player::clone() const
{
    Player *player = new Player;

    // Player
    player->setTileType(m_tileType);
    player->setPreviousDirection(m_previousDirection);
    player->setWeapon(m_weapon);
    player->setPortrait(m_portrait);
    player->setThumbnail(m_thumbnail);
    player->setlastValidPosition(m_lastValidPosition);

    // Unit
    player->setLife(m_life);
    player->setMaxLife(m_maxLife);
    player->setLifeAnim(m_lifeAnim);
    player->setDead(m_dead);
    player->setDeathDuration(m_deathDuration);
    player->setInvulnerable(m_invulnerable);
    player->setInvulnerabilityDuration(m_invulnerabilityDuration);

    // GameObject
    player->setGrid(m_grid);
    player->setPosition(m_position);
    player->setSpeed(m_speed);
    player->setDirection(m_direction);
    player->setFaction(m_faction);
    player->setUpstairs(m_upstairs);
    player->setModel(m_models[Right], Right);
    player->setModel(m_models[Up], Up);
    player->setModel(m_models[Left], Left);
    player->setModel(m_models[Down], Down);

    return player;
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

void Player::setPortrait(const QPixmap &portrait)
{
    m_portrait = portrait;
}

QPixmap Player::thumbnail() const
{
    return m_thumbnail;
}

void Player::setThumbnail(const QPixmap &thumbnail)
{
    m_thumbnail = thumbnail;
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
