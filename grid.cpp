#include "grid.h"

Grid::Grid(const QDomElement &element)
{
    QDomNode node = element.firstChild();
    QDomElement elem;
    QString values;

    while(!node.isNull())
    {
        elem = node.toElement();
        if(elem.tagName() == "Size")
        {
            m_width = elem.attribute("width").toInt();
            m_height = elem.attribute("height").toInt();
        }

        if(elem.tagName() == "Tiles")
        {
            values = elem.attribute("values");
        }

        node = node.nextSibling();
    }

    m_values = new Tile::TileType*[m_width];
    for(uint i=0; i<m_width; i++)
        m_values[i] = new Tile::TileType[m_height];

    for(uint i=0; i<m_width; i++)
        for(uint j=0; j<m_height; j++)
            m_values[i][j] = (Tile::TileType) (values.at(i + j*m_width).toLatin1() - 'a');
}

Tile::TileType Grid::tileAt(uint x, uint y) const
{
    if((x >= m_width) || (y >= m_height))
        return Tile::Void;
    return m_values[x][y];
}

Tile::TileType Grid::tileAt(const QPoint &tile) const
{
    return tileAt(tile.x(), tile.y());
}

void Grid::setTileAt(uint x, uint y, Tile::TileType value)
{
    if((x >= m_width)||(y >= m_height)) return;
    m_values[x][y] = value;
}

void Grid::setTileAt(const QPoint &tile, Tile::TileType value)
{
    setTileAt(tile.x(), tile.y(), value);
}

uint Grid::width() const
{
    return m_width;
}

uint Grid::height() const
{
    return m_height;
}

void Grid::swapTiles(Tile::TileType source, Tile::TileType dest)
{
    for(uint i=0; i<m_width; i++)
    {
        for(uint j=0; j<m_height; j++)
        {
            if(m_values[i][j] == source)
                m_values[i][j] = dest;
        }
    }
}

