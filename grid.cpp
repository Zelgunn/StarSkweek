#include "grid.h"

Grid::Grid()
{
    m_width = 20;
    m_height = 20;

    m_values = new Tile::TileType*[m_width];
    for(uint i=0; i<m_width; i++)
        m_values[i] = new Tile::TileType[m_height];

    for(uint i=0; i<m_width; i++)
        for(uint j=0; j<m_height; j++)
            m_values[i][j] = (Tile::TileType)(1 + (j*2)/m_height);
}

Tile::TileType Grid::tileAt(uint x, uint y) const
{
    Q_ASSERT(x < m_width);
    Q_ASSERT(y < m_height);

    return m_values[x][y];
}

void Grid::setTileAt(uint x, uint y, Tile::TileType value)
{
    Q_ASSERT(x < m_width);
    Q_ASSERT(y < m_height);

    m_values[x][y] = value;
}

uint Grid::width() const
{
    return m_width;
}

uint Grid::height() const
{
    return m_height;
}

