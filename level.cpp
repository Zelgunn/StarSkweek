#include "level.h"

Level::Level()
{
    m_grid = Grid();
    m_myPlayer = 0;
    m_player.setAppearance(QImage(QApplication::applicationDirPath().append("/images/player1.png")));
    m_player2.setAppearance(QImage(QApplication::applicationDirPath().append("/images/player2.png")));

    m_player.setPosition(0.5, 0.25);
    m_player2.setPosition(0.5, 0.75);

    m_player.setTileType(Tile::Player1Tile);
    m_player2.setTileType(Tile::Player2Tile);
}

void Level::setMyPlayer(int playerNumber)
{
    Q_ASSERT((playerNumber == 0) || (playerNumber == 1));
    if(m_myPlayer != playerNumber)
    {
        Player tmp = m_player;
        m_player = m_player2;
        m_player2= tmp;
        m_myPlayer = playerNumber;
    }
}

const Grid *Level::grid() const
{
    return &m_grid;
}

const Player *Level::player() const
{
    return &m_player;
}

const Player *Level::player2() const
{
    return &m_player2;
}

bool Level::movePlayer(int playerNumber, GameObject::Directions direction)
{
    Q_ASSERT((playerNumber == 0) || (playerNumber == 1));
    Player *player;
    Tile::TileType playerTile, player2Tile;

    if(playerNumber == m_myPlayer)
    {
        player = &m_player;
        playerTile = m_player.tileType();
        player2Tile = m_player2.tileType();
    }
    else
    {
        player = &m_player2;
        player2Tile = m_player.tileType();
        playerTile = m_player2.tileType();
    }

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

    player->setPosition(displacement);

    uint x = displacement.x * m_grid.width();
    uint y = displacement.y * m_grid.height();

    if(m_grid.tileAt(x, y) == player2Tile)
        m_grid.setTileAt(x, y, playerTile);

    return true;
}

bool Level::movePlayer1(GameObject::Directions direction)
{
    return movePlayer(m_myPlayer, direction);
}

bool Level::movePlayer2(GameObject::Directions direction)
{
    return movePlayer(!m_myPlayer, direction);
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
            if(tmp == m_player.tileType())
            {
                count ++;
                res ++;
            }
            else if(tmp == m_player2.tileType())
                count ++;
        }
    }

    return res/count;
}
