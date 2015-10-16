#ifndef GRID_H
#define GRID_H

#include <QFile>
#include <QIODevice>
#include <QDomElement>

#include "tile.h"

class Grid
{
public:
    Grid(const QDomElement &element);

    Tile::TileType tileAt(uint x, uint y) const;
    void setTileAt(uint x, uint y, Tile::TileType value);
    uint width() const;
    uint height() const;

private:
    uint m_width;
    uint m_height;
    Tile::TileType **m_values;
};

#endif // GRID_H
