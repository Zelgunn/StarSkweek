#include "level.h"

Level::Level(const QDomElement &element, QList<Player *> *characters)
    : m_characters(characters), m_myPlayer(0)
{
    m_players = new Player*[2];
    m_tiles = new Tile[Tile::TypeCount];
    int tileIndex;
    QString filename;
    QDomElement elem;
    QDomNode node = element.firstChild();

    while(!node.isNull())
    {
        elem = node.toElement();

        if(elem.tagName() == "Grid")
        {
            m_grid = new Grid(elem);
        }

        if(elem.tagName() == "TileSize")
        {
            m_tileSize.setHeight(elem.attribute("height").toInt());
            m_tileSize.setWidth(elem.attribute("width").toInt());
        }

        if(elem.tagName() == "Tile")
        {
            tileIndex = elem.attribute("type").toLatin1().at(0) - 'a';
            filename = QApplication::applicationDirPath() + elem.attribute("filename");
            m_tiles[tileIndex] = Tile(filename, m_tileSize, (Tile::TileType)tileIndex);
        }

        if(elem.tagName() == "Projectiles")
        {
            m_projectiles = new ProjectileList(elem);
        }

        if(elem.tagName() == "Weapons")
        {
            m_weapons = WeaponList(elem);
        }

        node = node.nextSibling();
    }

    m_players[0] = m_characters->at(0);
    m_players[0]->setPosition(width()/2, height()/4);
    m_players[0]->setFaction(0);
    m_players[0]->setWeapon(m_weapons.at(1));

    m_players[1] = m_characters->at(1);
    m_players[1]->setPosition(width()/2, height()*3/4 - 5);
    m_players[1]->setFaction(1);
    m_players[1]->setWeapon(m_weapons.at(0));

    m_projectiles->appendCollision(m_players[0]);
    m_projectiles->appendCollision(m_players[1]);

    QObject::connect(m_projectiles, SIGNAL(hitPlayer(GameObject*,int)), this, SLOT(onPlayerHit(GameObject*,int)));
}

void Level::setMyPlayer(int playerNumber)
{
    Q_ASSERT((playerNumber == 0) || (playerNumber == 1));
    if(m_myPlayer != playerNumber)
    {
        Player *tmp = m_players[0];
        m_players[0] = m_players[1];
        m_players[1] = tmp;
        m_myPlayer = playerNumber;
    }
}

const Grid *Level::grid() const
{
    return m_grid;
}

const Player *Level::player(int index) const
{
    return m_players[index];
}

const Player *Level::player() const
{
    return m_players[0];
}

const Player *Level::player2() const
{
    return m_players[1];
}

const ProjectileList *Level::projectiles() const
{
    return m_projectiles;
}

QSize Level::tileSize() const
{
    return m_tileSize;
}

Tile *Level::tiles() const
{
    return m_tiles;
}

int Level::width() const
{
    return m_tileSize.width() * m_grid->width();
}

int Level::height() const
{
    return m_tileSize.height() * m_grid->height();
}

bool Level::movePlayer(int playerNumber, GameObject::Directions direction)
{
    Q_ASSERT((playerNumber == 0) || (playerNumber == 1));
    Player *player = m_players[playerNumber], *player2 = m_players[!playerNumber];
    if(player->dead())
        return false;

    Point displacement = GameObject::displacement(direction, player->speed());
    displacement.x += player->position().x;
    displacement.y += player->position().y;

    int w = m_tileSize.width(), h = m_tileSize.height();

    if((displacement.x < 0)         ||
       (displacement.y < 0)         ||
       (displacement.x >= width())  ||
       (displacement.y >= height())) return false;

    uint x = displacement.x / w;
    uint y = displacement.y / h;

    if(m_grid->tileAt(x, y) == Tile::Void)
    {
        if(m_grid->tileAt(player->position().x / w, player->position().y / h) == Tile::Void)
            return false;
        player->takeDamage(999);
    }

    if(m_grid->tileAt(x,y) == Tile::ArrowTileDown)
        displacement.y += 2;
    if(m_grid->tileAt(x,y) == Tile::ArrowTileLeft)
        displacement.x -= 2;
    if(m_grid->tileAt(x,y) == Tile::ArrowTileRight)
        displacement.x += 2;
    if(m_grid->tileAt(x,y) == Tile::ArrowTileUp)
        displacement.y -= 2;

    player->setPosition(displacement);
    player->setDirection(direction);

    if(m_grid->tileAt(x, y) == player2->tileType())
        m_grid->setTileAt(x, y, player->tileType());

    return true;
}

bool Level::movePlayer1(GameObject::Directions direction)
{
    return movePlayer(0, direction);
}

bool Level::movePlayer2(GameObject::Directions direction)
{
    return movePlayer(1, direction);
}

void Level::setPlayerDirection(int playerId, GameObject::Directions direction)
{
    m_players[playerId]->setDirection(direction);
}

bool Level::playerFires(int playerId)
{
    Player *player = m_players[playerId];
    int type = player->fire();

    if(type >= 0)
    {
        m_projectiles->append(type, playerId, player->previousDirection(), player->position());

        return true;
    }

    return false;
}

double Level::playerTileRatio() const
{
    double res = 0, count = 0;
    int tmp;

    for(uint i=0; i<m_grid->width(); i++)
    {
        for(uint j=0; j<m_grid->height(); j++)
        {
            tmp = m_grid->tileAt(i,j);
            if(tmp == m_players[0]->tileType())
            {
                count ++;
                res ++;
            }
            else if(tmp == m_players[1]->tileType())
                count ++;
        }
    }

    return res/count;
}

void Level::nextFrame()
{
    m_projectiles->moveProjectiles();

    movePlayer1(m_players[0]->direction());
    movePlayer2(m_players[1]->direction());

    m_players[0]->updateLifeAnim();
    m_players[1]->updateLifeAnim();
}

void Level::onPlayerHit(GameObject *player, int type)
{
    Player *p = (Player*) player;
    p->takeDamage(m_projectiles->prototype(type).damage());
}
