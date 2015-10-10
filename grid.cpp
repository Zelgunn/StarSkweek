#include "grid.h"

Grid::Grid()
{

}

Grid::Grid(QDomElement elem)
{
    Q_UNUSED(elem);
    m_width = 10;
    m_height = 10;

    m_values = new int*[m_width];
    for(uint i=0; i<m_width; i++)
        m_values[i] = new int[m_height];

    for(uint i=0; i<m_width; i++)
        for(uint j=0; j<m_width; j++)
            m_values[i][j] = 1;
}

int Grid::tileAt(uint x, uint y) const
{
    Q_ASSERT(x < m_width);
    Q_ASSERT(y < m_height);

    return m_values[x][y];
}

void Grid::setTileAt(uint x, uint y, int value)
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

