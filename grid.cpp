#include "grid.h"

Grid::Grid()
{
    QFile file(QApplication::applicationDirPath() + "/grid.txt");
    file.open(QIODevice::ReadOnly);
    QByteArray text = file.readAll();

    m_width = 32;
    m_height = 16;

    m_values = new Tile::TileType*[m_width];
    for(uint i=0; i<m_width; i++)
        m_values[i] = new Tile::TileType[m_height];

    for(uint i=0; i<m_width; i++)
        for(uint j=0; j<m_height; j++)
            m_values[i][j] = (Tile::TileType) (text.at(i + j*(m_width+2)) - '0');
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

