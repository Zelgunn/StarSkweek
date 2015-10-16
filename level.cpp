#include "level.h"

Level::Level()
{
    m_grid = Grid();
    m_myPlayer = 0;

    m_players = new Player*[2];
    m_players[0] = new Player(QPixmap(QApplication::applicationDirPath().append("/images/player1.png")),
                              Tile::Player1Tile);
    m_players[0]->setPosition(0.5, 0.25);

    m_players[1] = new Player(QPixmap(QApplication::applicationDirPath().append("/images/player2.png")),
                              Tile::Player2Tile);
    m_players[1]->setPosition(0.5, 0.75);
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
    return &m_grid;
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

bool Level::movePlayer(int playerNumber, GameObject::Directions direction)
{
    Q_ASSERT((playerNumber == 0) || (playerNumber == 1));
    Player *player = m_players[playerNumber], *player2 = m_players[!playerNumber];

    Point displacement = GameObject::displacement(direction, player->speed());
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

    uint x = displacement.x * m_grid.width();
    uint y = displacement.y * m_grid.height();

    if(m_grid.tileAt(x, y) == Tile::Void)
        return false;

    player->setPosition(displacement);
    player->setDirection(direction);

    if(m_grid.tileAt(x, y) == player2->tileType())
        m_grid.setTileAt(x, y, player->tileType());

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

    Projectile *projectile = new Projectile;
    if(player->fire(projectile))
    {
        projectile->setPosition(player->position());
        projectile->setDirection(player->previousDirection());
        projectile->setOwnerID(playerId);
        m_projectiles.append(projectile);

        return true;
    }

    delete projectile;
    return false;
}

double Level::playerTileRatio() const
{
    double res = 0, count = 0;
    int tmp;

    for(uint i=0; i<m_grid.width(); i++)
    {
        for(uint j=0; j<m_grid.height(); j++)
        {
            tmp = m_grid.tileAt(i,j);
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
    Projectile *projectile;
    Player *player;

    for(int i=0; i<m_projectiles.size(); i++)
    {
        projectile = m_projectiles.at(i);
        projectile->move();
        if(projectile->ttl() < 0)
        {
            m_projectiles.removeAt(i);
            delete projectile;
            i--;
        }
        else
        {
            player = m_players[!projectile->ownerID()];
            if(GameObject::euclidianDistance(projectile->position(), player->position()) < 0.025)
            {
                m_projectiles.removeAt(i);
                delete projectile;
                i--;
            }
        }
    }

    movePlayer1(m_players[0]->direction());
    movePlayer2(m_players[1]->direction());
}
QList<Projectile *> Level::projectiles() const
{
    return m_projectiles;
}

