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
    m_previousDirection(Down), m_powerRessource(MaxPowerRessource/2), m_ghostForm(false)
{
    m_name = element.attribute("name");
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

        if(elem.tagName() == "Power")
        {
            m_power = (Powers)(elem.attribute("type").at(0).toLatin1() - 'a');
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
    player->setPower(m_power);
    player->setPowerRessource(m_powerRessource);
    player->setGhostForm(m_ghostForm);

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
    player->setName(m_name);

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

Player::Powers Player::power() const
{
    return m_power;
}

void Player::setPower(const Player::Powers &power)
{
    m_power = power;
}

int Player::powerRessource() const
{
    return m_powerRessource;
}

void Player::setPowerRessource(int powerRessource)
{
    m_powerRessource = powerRessource;
}

void Player::increasePowerRessource(int increase)
{
    m_powerRessource += increase;
    if(m_powerRessource > MaxPowerRessource) m_powerRessource = MaxPowerRessource;
}

bool Player::powerAvailable() const
{
    return (m_powerRessource == MaxPowerRessource);
}

qreal Player::powerRessourceRatio() const
{
    return (qreal)m_powerRessource/(qreal)MaxPowerRessource;
}

bool Player::ghostForm()
{
    if(ghostFormTimeLeft() <= 0)
        m_ghostForm = false;
    return m_ghostForm;
}

void Player::setGhostForm(bool ghostForm)
{
    m_ghostForm = ghostForm;
    if(ghostForm)
    {
        m_ghostFormTimer = QTime::currentTime();
    }
}

int Player::ghostFormTimeLeft() const
{
    return (2000 - m_ghostFormTimer.msecsTo(QTime::currentTime()));
}

int Player::fire()
{
    if(m_dead) return -1;
    return m_weapon.fire();
}

void Player::takeDamage(int damage)
{
    if(ghostForm())
        Unit::takeDamage(damage/2);
    else
        Unit::takeDamage(damage);
}

bool Player::isPlayer() const
{
    return true;
}

int Player::maxPowerRessource()
{
    return (int)MaxPowerRessource;
}
