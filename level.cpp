#include "level.h"

Level::Level(const QDomElement &element, QList<Player> *characters)
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
            m_projectiles = ProjectileList(elem);
        }

        node = node.nextSibling();
    }

    m_players[0] = new Player(m_characters->at(0));
    m_players[0]->setPosition(0.5, 0.25);
    m_players[0]->setFaction(0);

    m_players[1] = new Player(m_characters->at(1));
    m_players[1]->setPosition(0.5, 0.75);
    m_players[1]->setFaction(1);

    m_projectiles.appendCollision(m_players[0]);
    m_projectiles.appendCollision(m_players[1]);
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

const Player *Level::player() const
{
    if(m_myPlayer == 0)
        return m_players[0];
    else
        return m_players[1];
}

const Player *Level::player2() const
{
    if(m_myPlayer == 0)
        return m_players[1];
    else
        return m_players[0];
}

ProjectileList Level::projectiles() const
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

bool Level::movePlayer(int playerNumber, GameObject::Directions direction)
{
    Q_ASSERT((playerNumber == 0) || (playerNumber == 1));
    Player *player = m_players[playerNumber], *player2 = m_players[!playerNumber];

    double speedRatio = (double)(m_tileSize.width() * m_grid->width())/ (double)(m_tileSize.height() * m_grid->height());

    Point displacement = GameObject::displacement(direction, player->speed(), speedRatio);
    displacement.x += player->position().x;
    displacement.y += player->position().y;

    if((displacement.x < 0) ||
       (displacement.y < 0)  ||
       (displacement.x > 1)  ||
       (displacement.y > 1)) return false;

    if(displacement.x < 0) displacement.x = 0;
    if(displacement.y < 0) displacement.y = 0;
    if(displacement.x > 1) displacement.x = 1;
    if(displacement.y > 1) displacement.y = 1;

    uint x = displacement.x * m_grid->width();
    uint y = displacement.y * m_grid->height();

    if(m_grid->tileAt(x, y) == Tile::Void)
        return false;

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
        m_projectiles.append(type, playerId, player->previousDirection(), player->position());

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
    double speedRatio = (double)(m_tileSize.width() * m_grid->width())/ (double)(m_tileSize.height() * m_grid->height());

    m_projectiles.moveProjectiles(speedRatio);

    movePlayer1(m_players[0]->direction());
    movePlayer2(m_players[1]->direction());
}
