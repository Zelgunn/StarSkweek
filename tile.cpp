#include "tile.h"

Tile::Tile()
    : m_type(InvalidTile)
{

}

Tile::Tile(const QString &filename, const QSize &size, TileType type)
    : m_type(type)
{
    m_texture = QPixmap(filename).scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

void Tile::resize(QSize size)
{
    m_texture = m_texture.scaled(size);
}

Tile::TileType Tile::type() const
{
    return m_type;
}

QPixmap Tile::texture() const
{
    return m_texture;
}
