#include "level.h"

Level::Level()
{

}

Level::Level(QDomElement elem)
{
    // TMP
    m_grid = Grid(elem);
}

const Grid *Level::grid() const
{
    return &m_grid;
}

const Player *Level::player() const
{
    return &m_player;
}

void Level::movePlayer(GameObject::Directions direction)
{
    Point displacement = GameObject::displacement(direction, m_player.speed());
    displacement.x += m_player.position().x;
    displacement.y += m_player.position().y;

    if(displacement.x < 0) displacement.x = 0;
    if(displacement.y < 0) displacement.y = 0;
    if(displacement.x > 1) displacement.x = 1;
    if(displacement.y > 1) displacement.y = 1;

    m_player.setPosition(displacement);

    uint x = displacement.x * m_grid.width();
    uint y = displacement.y * m_grid.height();
    if(m_grid.tileAt(x, y) == 1)
        m_grid.setTileAt(x, y, 0);
}

